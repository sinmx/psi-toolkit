#ifndef LEX_DB_LEMMATIZER_HDR
#define LEX_DB_LEMMATIZER_HDR

#include <string>
#include <list>

#include <pqxx/connection>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"

class LexDbLemmatizer {

public:
    LexDbLemmatizer(const boost::program_options::variables_map& options);

    bool lemmatize(const std::string& token,
                   AnnotationItemManager& annotationItemManager,
                   LemmatizerOutputIterator& outputIterator);

    static std::string getName();
    static boost::filesystem::path getFile();

    std::string getLanguage() const;

    std::list<std::string> getLayerTags();

    static boost::program_options::options_description optionsHandled();

private:
    pqxx::connection connection_;

    void parseSinflection_(const std::string& sinflection,
                           std::string& partOfSpeech,
                           std::string& flags);

};

#endif
