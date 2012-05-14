#include "link_parser.hpp"

#include <vector>

#include <boost/assign.hpp>
#include <boost/foreach.hpp>

#include "edge_description.hpp"
#include "lang_specific_processor_file_fetcher.hpp"
#include "plugin_manager.hpp"


Annotator* LinkParser::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options
) {
    std::string lang = options["lang"].as<std::string>();
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    if (
        !options.count("dict") &&
        !options.count("knowledge") &&
        !options.count("constituent-knowledge") &&
        !options.count("affix")
    ) {
        return new LinkParser(lang);
    }

    std::string dictPathString;
    if (options.count("dict")) {
        std::string dictFilename
            = options["dict"].as<std::string>();
        boost::filesystem::path dictPath
            = fileFetcher.getOneFile(dictFilename).string();
        dictPathString = dictPath.string();
    }

    std::string knowledgePathString;
    if (options.count("knowledge")) {
        std::string knowledgeFilename
            = options["knowledge"].as<std::string>();
        boost::filesystem::path knowledgePath
            = fileFetcher.getOneFile(knowledgeFilename).string();
        knowledgePathString = knowledgePath.string();
    }

    std::string constituentKnowledgePathString;
    if (options.count("constituent-knowledge")) {
        std::string constituentKnowledgeFilename
            = options["constituent-knowledge"].as<std::string>();
        boost::filesystem::path constituentKnowledgePath
            = fileFetcher.getOneFile(constituentKnowledgeFilename).string();
        constituentKnowledgePathString = constituentKnowledgePath.string();
    }

    std::string affixPathString;
    if (options.count("affix")) {
        std::string affixFilename
            = options["affix"].as<std::string>();
        boost::filesystem::path affixPath
            = fileFetcher.getOneFile(affixFilename).string();
        affixPathString = affixPath.string();
    }

    return new LinkParser(
        lang,
        dictPathString,
        knowledgePathString,
        constituentKnowledgePathString,
        affixPathString
    );

}

void LinkParser::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {
    optionsDescription.add_options()
        ("dict",
        boost::program_options::value<std::string>(),
        "dictionary file name")
        ("knowledge",
        boost::program_options::value<std::string>(),
        "post process file name")
        ("constituent-knowledge",
        boost::program_options::value<std::string>(),
        "constituent knowledge file name")
        ("affix",
        boost::program_options::value<std::string>(),
        "affix file name")
        ;
}

std::string LinkParser::Factory::doGetName() const {
    return "link-parser";
}

std::list<std::string> LinkParser::Factory::doGetAliases() {
    return boost::assign::list_of<std::string>
        (std::string("link-grammar"))
        (std::string("link-grammar-parse"))
        (std::string("link-grammar-parser"))
        (std::string("link-parse"))
    ;
}

boost::filesystem::path LinkParser::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > LinkParser::Factory::doRequiredLayerTags() {
    return boost::assign::list_of(
        boost::assign::list_of(std::string("segment"))
    );
}

std::list<std::list<std::string> > LinkParser::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> LinkParser::Factory::doProvidedLayerTags() {
    return boost::assign::list_of("link-grammar")("parse");
}

LatticeWorker* LinkParser::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

LinkParser::Worker::Worker(LinkParser& processor, Lattice& lattice) :
    LatticeWorker(lattice), processor_(processor)
{
    //TODO
}

void LinkParser::Worker::doRun() {
    processor_.parse(lattice_);
}

std::string LinkParser::doInfo() {
    return "link grammar parser";
}

LinkParser::LinkParser(std::string language) : langCode_(language) {
    adapter_ = dynamic_cast<LinkParserAdapterInterface*>(
        PluginManager::getInstance().createPluginAdapter("link-parser")
    );
    adapter_->setDictionary(language);
}

LinkParser::LinkParser(
    std::string language,
    std::string dictionaryName,
    std::string postProcessFileName,
    std::string constituentKnowledgeName,
    std::string affixName
) : langCode_(language) {
    adapter_ = dynamic_cast<LinkParserAdapterInterface*>(
        PluginManager::getInstance().createPluginAdapter("link-parser")
    );
    adapter_->setDictionary(
        dictionaryName,
        postProcessFileName,
        constituentKnowledgeName,
        affixName
    );
}

LinkParser::~LinkParser() {
    if (adapter_) {
        PluginManager::getInstance().destroyPluginAdapter("link-parser", adapter_);
    }
}

LinkParserAdapterInterface * LinkParser::getAdapter() {
    return adapter_;
}

bool LinkParser::isActive() {
    if (adapter_) {
        return true;
    } else {
        return false;
    }
}


AnnotatorFactory::LanguagesHandling LinkParser::languagesHandling(
    const boost::program_options::variables_map& /*options*/) {
    return AnnotatorFactory::LANGUAGE_DEPENDENT;
}

std::list<std::string> LinkParser::languagesHandled(
    const boost::program_options::variables_map& /*options*/) {
    return boost::assign::list_of(std::string("en"))(std::string("lt"));
}


void LinkParser::parse(Lattice &lattice) {
    LayerTagMask maskSegment = lattice.getLayerTagManager().getMaskWithLangCode(
        "segment",
        langCode_
    );
    Lattice::EdgesSortedBySourceIterator ei(lattice, maskSegment);
    while (ei.hasNext()) {
        Lattice::EdgeDescriptor edge = ei.next();
        std::map<int, EdgeDescription> parsingResult
            = adapter_->parseSentence(lattice.getEdgeText(edge));
        std::map<int, Lattice::EdgeDescriptor> addedEdges;
        typedef std::pair<int, EdgeDescription> ParsingResultElement;
        BOOST_FOREACH(ParsingResultElement parsingResultElement, parsingResult) {
            EdgeDescription edgeDescription = parsingResultElement.second;
            AnnotationItem aiLink(
                edgeDescription.label,
                StringFrag(
                    lattice.getAllText(),
                    edgeDescription.start,
                    edgeDescription.end-edgeDescription.start
                )
            );
            LayerTagCollection tagParse
                = lattice.getLayerTagManager().createTagCollectionFromListWithLangCode(
                    boost::assign::list_of("link-grammar")("parse"),
                    langCode_
                );
            Lattice::EdgeSequence::Builder builder(lattice);
            BOOST_FOREACH(int childNo, edgeDescription.children) {
                builder.addEdge(addedEdges[childNo]);
            }
            if (edgeDescription.start == edgeDescription.end) {
                edgeDescription.start = lattice.addLooseVertex();
                edgeDescription.end = lattice.addLooseVertex();
            }
            addedEdges[parsingResultElement.first] = lattice.addEdge(
                edgeDescription.start,
                edgeDescription.end,
                aiLink,
                tagParse,
                builder.build()
            );
        }
    }
}