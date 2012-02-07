#include "tests.hpp"

#include "config.hpp"
#include "bilexicon.hpp"

#include <list>

#include <boost/program_options/parsers.hpp>
#include <boost/assign.hpp>

#include <stdio.h>

void testOnLattice(
    BiLexicon& biLexicon,
    const std::string& word,
    const std::string& lexeme,
    const std::string& cat,
    const std::list<std::string>& expectedEntries);

BOOST_AUTO_TEST_SUITE( bilexicon )

BOOST_AUTO_TEST_CASE( bilexicon_simple ) {
    const char* argv[4] = {
        "fakename",
        "--plain-text-lexicon",
        ROOT_DIR "tools/lexica/bilexicon/t/plen.txt",
        0};

    const int argc = 3;

    boost::program_options::variables_map options;
    boost::program_options::store(
        boost::program_options::parse_command_line(
            argc, argv, BiLexicon::optionsHandled()), options);
    boost::program_options::notify(options);

    BiLexicon biLexicon(options);
    testOnLattice(biLexicon, "kanału", "kanał_subst", "subst",
                  boost::assign::list_of("channel_subst")("canal_subst"));

    testOnLattice(biLexicon, "garnki", "garnek_subst", "subst",
                  boost::assign::list_of("pot_subst"));

    testOnLattice(biLexicon, "nauskopii", "nauskopia_subst", "subst",
                  std::list<std::string>());

}

BOOST_AUTO_TEST_CASE( bilexicon_save_and_load ) {
    char* tmpFile = tempnam(0, "bilexicon_save_and_load_bin");

    {
        const char* argv[6] = {
            "fakename",
            "--plain-text-lexicon",
            ROOT_DIR "tools/lexica/bilexicon/t/plen.txt",
            "--save-binary-lexicon",
            tmpFile,
            0};

        const int argc = 5;

        boost::program_options::variables_map options;
        boost::program_options::store(
            boost::program_options::parse_command_line(
                argc, argv, BiLexicon::optionsHandled()), options);
        boost::program_options::notify(options);

        BiLexicon biLexicon(options);
        testOnLattice(biLexicon, "kanału", "kanał_subst", "subst",
                      boost::assign::list_of("channel_subst")("canal_subst"));

    }

    {
        const char* argv[4] = {
            "fakename",
            "--binary-lexicon",
            tmpFile,
            0};

        const int argc = 3;

        boost::program_options::variables_map options;
        boost::program_options::store(
            boost::program_options::parse_command_line(
                argc, argv, BiLexicon::optionsHandled()), options);

        boost::program_options::notify(options);

        BiLexicon biLexicon(options);

        testOnLattice(biLexicon, "kanału", "kanał_subst", "subst",
                      boost::assign::list_of("channel_subst")("canal_subst"));

        testOnLattice(biLexicon, "garnkami", "garnek_subst", "subst",
                      boost::assign::list_of("pot_subst"));

        testOnLattice(biLexicon, "nauskopii", "nauskopia_subst", "subst",
                      std::list<std::string>());
    }

    free(tmpFile);
}

BOOST_AUTO_TEST_SUITE_END()

void testOnLattice(
    BiLexicon& biLexicon,
    const std::string& word,
    const std::string& lexeme,
    const std::string& cat,
    const std::list<std::string>& expectedEntries) {

    Lattice lattice(word);
    AnnotationItem item(cat, StringFrag(lexeme));

    LayerTagCollection tags = lattice.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("lexeme")("fake"));

    Lattice::EdgeDescriptor edge = lattice.addEdge(
        lattice.getFirstVertex(),
        lattice.getLastVertex(),
        item,
        tags);

    biLexicon.processEdge(lattice, edge);

    std::list<std::string> tagList = boost::assign::list_of("bilexicon");
    Lattice::EdgesSortedBySourceIterator it(lattice, lattice.getLayerTagManager().getMask(tagList));

    BOOST_FOREACH(const std::string& expectedEntry, expectedEntries) {
        BOOST_REQUIRE(it.hasNext());
        Lattice::EdgeDescriptor equivEdge = it.next();

        BOOST_CHECK_EQUAL(lattice.getAnnotationText(equivEdge), expectedEntry);
    }

    BOOST_CHECK(!it.hasNext());
}