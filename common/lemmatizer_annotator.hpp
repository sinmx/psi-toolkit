#ifndef LEMMATIZER_ANNOTATOR_HDR
#define LEMMATIZER_ANNOTATOR_HDR

#include "annotator.hpp"
#include "annotator_factory.hpp"
#include "lattice_worker.hpp"
#include "layer_tag_collection.hpp"

template<typename L>
class LemmatizerAnnotator : public Annotator {

private:
    L lemmatizer_;

public:

    class Factory : public AnnotatorFactory {
    private:
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options) {

            return new LemmatizerAnnotator<L>();
        }

        virtual boost::program_options::options_description doOptionsHandled() {
            return boost::program_options::options_description();
        }

        virtual std::string doGetName() {
            return L::getName();
        }

        virtual std::list<std::list<std::string> > doRequiredLayerTags() {
            return std::list<std::list<std::string> >();
        }

        virtual std::list<std::list<std::string> > doOptionalLayerTags() {
            return std::list<std::list<std::string> >();
        }

        virtual std::list<std::string> doProvidedLayerTags() {
            std::list<std::string> layerTags;
            layerTags.push_back("lemma");
            return layerTags;
        }
    };

    class Worker : public LatticeWorker {

    private:
        Processor& processor_;
        LayerTagMask tokenMask_;

        class WorkerOutputIterator : public LemmatizerOutputIterator {
        private:
            Lattice& lattice_;
            LayerTagCollection lemmaTag_;
            LayerTagCollection formTag_;
            Lattice::EdgeDescriptor tokenEdge_;
            Lattice::EdgeDescriptor lastCorrection_;
            Lattice::EdgeDescriptor lastNormalization_;
            Lattice::EdgeDescriptor lastLemma_;

        public:
            WorkerOutputIterator(Lattice& lattice, Lattice::EdgeDescriptor tokenEdge)
                :lattice_(lattice),
                 lemmaTag_(lattice.getLayerTagManager().createSingletonTagCollection("lemma")),
                 formTag_(lattice.getLayerTagManager().createSingletonTagCollection("form")),
                 tokenEdge_(tokenEdge),
                 lastCorrection_(tokenEdge),
                 lastNormalization_(tokenEdge),
                 lastLemma_(tokenEdge) {
            }

        private:
            virtual void doAddCorrection(
                const std::string correction,
                Lattice::Score score,
                int ruleId) {
                ;
            }

            virtual void doAddNormalization(
                const std::string normalization,
                Lattice::Score score,
                int ruleId) {
                ;
            }

            virtual void doAddLemma(
                const AnnotationItem& item,
                Lattice::Score score,
                int ruleId) {

                Lattice::EdgeSequence::Builder seqBuilder;
                seqBuilder.addEdge(lastNormalization_);

                lastLemma_ =
                    lattice_.addEdge(
                        lattice_.getEdgeSource(lastNormalization_),
                        lattice_.getEdgeTarget(lastNormalization_),
                        item,
                        lemmaTag_,
                        seqBuilder.build(),
                        score,
                        ruleId);
            }

            virtual void doAddForm(
                const AnnotationItem& item,
                Lattice::Score score,
                int ruleId) {

                Lattice::EdgeSequence::Builder seqBuilder;
                seqBuilder.addEdge(lastLemma_);

                lattice_.addEdge(
                    lattice_.getEdgeSource(lastNormalization_),
                    lattice_.getEdgeTarget(lastNormalization_),
                    item,
                    formTag_,
                    seqBuilder.build(),
                    score,
                    ruleId);
            }
        };

    public:
        Worker(Processor& processor, Lattice& lattice)
            :LatticeWorker(lattice),
             processor_(processor),
             tokenMask_(lattice.getLayerTagManager().getMask("token")) {
        }

   private:
        virtual void doRun() {
            Lattice::EdgesSortedByTargetIterator edgeIterator = lattice_.edgesSortedByTarget(tokenMask_);

            while (edgeIterator.hasNext()) {
                Lattice::EdgeDescriptor edge = edgeIterator.next();

                WorkerOutputIterator outputIterator(lattice_, edge);
                dynamic_cast<LemmatizerAnnotator&>(processor_).lemmatizer_.lemmatize(
                    lattice_.getEdgeText(edge),
                    lattice_.getAnnotationItemManager(),
                    outputIterator);
            }
        }

    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice) {
        return new Worker(*this, lattice);
    }

    virtual std::string doInfo() {
        return "bla";
    }

};


#endif