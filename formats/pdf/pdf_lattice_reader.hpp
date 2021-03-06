#ifndef PDF_LATTICE_READER_HDR
#define PDF_LATTICE_READER_HDR


#include "stream_lattice_reader.hpp"
#include "lattice_reader_factory.hpp"
#include "plugin/poppler_adapter_interface.hpp"

class PDFLatticeReader : public StreamLatticeReader {

public:
    PDFLatticeReader();
    ~PDFLatticeReader();

    virtual std::string getFormatName();

    PopplerAdapterInterface * getAdapter();
    bool isActive();

    class Factory : public LatticeReaderFactory<std::istream> {
    public:
        virtual ~Factory();

    private:
        virtual LatticeReader<std::istream>* doCreateLatticeReader(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;
    };

private:
    virtual std::string doInfo();

    class Worker : public ReaderWorker<std::istream> {
    public:
        Worker(PDFLatticeReader& processor,
               std::istream& inputStream,
               Lattice& lattice);

        virtual void doRun();

    private:
        PDFLatticeReader& processor_;
        LayerTagCollection textTags_;
    };

    virtual ReaderWorker<std::istream>* doCreateReaderWorker(
        std::istream& inputStream, Lattice& lattice) {

        return new Worker(*this, inputStream, lattice);
    }

    PopplerAdapterInterface * adapter_;
};

#endif
