#ifndef SFST_HDR
#define SFST_HDR

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>


#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>

#include "fst.h"
#include "sfst_tags_parser.hpp"
#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"
#include "annotator_factory.hpp"

#include <map>
#include <list>
#include <set>

class SfstLemmatizer {
	public:

    SfstLemmatizer(const boost::program_options::variables_map& options);


    static boost::program_options::options_description optionsHandled();
    
    static std::string getName();
    static boost::filesystem::path getFile();
    
    std::list<std::string> getLayerTags();
    
    std::string getLanguage() const;

	static AnnotatorFactory::LanguagesHandling languagesHandling(
        const boost::program_options::variables_map& options);

    static std::list<std::string> languagesHandled(
        const boost::program_options::variables_map& options);
    
	void lemmatize(
        const std::string & word,
        AnnotationItemManager & manager,
        LemmatizerOutputIterator & iterator
    );
    
    /**
     * Sets the word processing level, default 0.
     * @param level
     *  Word processing level:
     *  0 - normal mode...
     */
    void setLevel(int);

    /**
     * Sets language and reloads dict
     * @param lang
     * lang name
     */    
    void setLanguage(std::string);
    void setAutomaton(std::string);
    
    
    /**
     * Stems word using sfstLemmatizer tool storing lexical information.
     * @param word
     *  A string containing word to stem.
     * @return
     *  A multimap object with stem-tags pairs. Each stem may have more than
     *  one tag (in Morfologik tags are separated by plus sign).
     * @see simpleStem()
     */
    std::multimap<std::string, std::vector<std::string> > stem(
        const std::string & word);
    
    /**
     * Stems word using sfstLemmatizer tool.
     * @param word
     *  A string containing word to stem.
     * @return
     *  A vector of found stems with repetitions.
     * @see stem()
     */
    std::vector<std::string> simpleStem(const std::string & word);
    
    private:
		AnnotationItemManager * annotationManager;
		int level;
		std::string language;
		std::string automaton;
		
		void stemsOnLemmaLevel(const std::string &, LemmatizerOutputIterator &);
		void stemsOnLexemeLevel(const std::string &, LemmatizerOutputIterator &);
		void stemsOnFormLevel(const std::string &, LemmatizerOutputIterator &);

		//Ugly taken from morfo...
		AnnotationItem createLexemeAnnotation(
			const std::string & stem, std::string & tag
		);
		
		AnnotationItem createFormAnnotation(
			AnnotationItem & lexemeItem,
			const std::string & word,
			std::string &
		);

		
		std::set<std::string> getLemmasFromStems(
			std::multimap<std::string, std::vector<std::string> > stems
		);
		std::vector<std::string> getLexemeTagsFromStems(
			std::multimap<std::string, std::vector<std::string> > & stems,
			const std::string & lemma
		);
		std::vector<std::string> getFormTagsFromLexem(
			std::multimap<std::string, std::vector<std::string> > & stems,
			const std::string & lemma,
			const std::string & lexem
		);
		static std::string tagSeparator;
		//End of takens..
		SFST::Transducer * transducer;
		//scoped ptr... (Inteligenty wskaźnik..)
		
		/***
		 * Outpusts raw word analysis from sfst transducer.
		 * @param word
		 * A word that will be analysed.
		 * @return
		 * String vector containing output of transducer.
		 */ 
		std::vector<std::string> wordToRaw(std::string);
		
		void cookRaw(std::string &);
		
		std::string getCookedStem(std::string);
		std::vector<std::string> getCookedTags(std::string);
		
		void initializeTransducer();
		
};

#endif
