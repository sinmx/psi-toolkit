#include "lattice_writer_factory.hpp"

LatticeWriter* LatticeWriterFactory::createLatticeWriter(const boost::program_options::variables_map& options) {
    return dynamic_cast<LatticeWriter*>(createProcessor(options));
}


LatticeWriter* LatticeWriterFactory::doCreateProcessor(const boost::program_options::variables_map& options) {
    return doCreateLatticeWriter(options);
}

LatticeWriterFactory::~LatticeWriterFactory() {
}

std::string LatticeWriterFactory::doGetContinuation(
    const boost::program_options::variables_map& /*options*/) const {
    return std::string();
}


