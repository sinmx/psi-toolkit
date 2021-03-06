#include "lattice_iter_writer.hpp"

void LatticeIterWriter::run() {

    Lattice::VertexIterator vi(lattice_);
    Lattice::VertexDescriptor vd = lattice_.getFirstVertex();
    Lattice::EdgeDescriptor edge;
    std::queue<Lattice::EdgeDescriptor> basicTagEdges;

    std::map<std::string, Lattice::VertexDescriptor> targets;

    std::map<std::string, bool> groupOpened;
    BOOST_FOREACH(std::string tag, handledTags_) {
        groupOpened[tag] = false;
    }
    bool alternativeOpened = false;

    bool printElements = true;

    while (linear_ ? true : vi.hasNext()) {

        if (!linear_) {
            vd = vi.next();
        }

        Lattice::InOutEdgesIterator oei
            = lattice_.outEdges(vd, lattice_.getLayerTagManager().anyTag());

        // gather information about edges
        while (oei.hasNext()) {
            edge = oei.next();
            std::list<std::string> tags
                = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
            BOOST_FOREACH(std::string tag, tags) {
                if (
                    basicTag_ == tag &&
                    (withBlank_ || !boost::algorithm::trim_copy(lattice_.getEdgeText(edge)).empty())
                ) {
                    basicTagEdges.push(edge);
                }
                if (isHandledTag_(tag) && targets[tag] == vd) {
                    targets[tag] = lattice_.getEdgeTarget(edge);
                    if (groupOpened[tag]) {
                        outputIterator_.closeGroup(tag);
                    }
                    outputIterator_.openGroup(tag);
                    groupOpened[tag] = true;
                }
            }
        }

        // check if there is need for printing anything
        // (prevents from printing edges not covered by --spec-ified tags)
        if (!handledTags_.empty()) {
            printElements = false;
            BOOST_FOREACH(std::string tag, handledTags_) {
                if (groupOpened[tag]) {
                    printElements = true;
                    break;
                }
            }
        }

        // print edges
        while (!basicTagEdges.empty()) {
            Lattice::EdgeDescriptor basicTagEdge = basicTagEdges.front();

            if (printElements) {
                if (!alternativeOpened) {
                    outputIterator_.openAlternative();
                    alternativeOpened = true;
                }
                outputIterator_.putElement(lattice_.getEdgeAnnotationItem(basicTagEdge));
            } else {
                WARN("Edge ("
                    << lattice_.getEdgeAnnotationItem(basicTagEdges.front()).getText()
                    << ") not printed because not enclosed by a spec-tag edge");
            }

            basicTagEdges.pop();
            if (noAlts_) {
                while (!basicTagEdges.empty()) {
                    basicTagEdges.pop();
                }
            }
        }
        if (alternativeOpened) {
            outputIterator_.closeAlternative();
            alternativeOpened = false;
        }

        if (linear_) {
            try {
                vd = lattice_.getEdgeTarget(
                    lattice_.firstOutEdge(vd, lattice_.getLayerTagManager().anyTag())
                );
            } catch (NoEdgeException) {
                break;
            }
        }

    }

    typedef std::pair<std::string, Lattice::VertexDescriptor> TagTargetPair;
    BOOST_FOREACH(TagTargetPair tagTargetpair, targets) {
        outputIterator_.closeGroup(tagTargetpair.first);
    }

    outputIterator_.flush();

}

bool LatticeIterWriter::isHandledTag_(std::string tagName) {
    return std::find(handledTags_.begin(), handledTags_.end(), tagName) != handledTags_.end();
}
