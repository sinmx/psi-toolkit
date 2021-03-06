#ifndef LAYER_TAG_MANAGER_HDR
#define LAYER_TAG_MANAGER_HDR

#include <list>
#include <string>

#include <boost/bimap.hpp>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>

#include "layer_tag_collection.hpp"
#include "layer_tag_mask.hpp"

/*!
  LayerTagManager is used to
  * create layer tags
  * keep mapping to string so that LayerTagCollection is just a bit vector
  */
class LayerTagManager {

public:

    LayerTagManager();

    LayerTagCollection createEmptyTagCollection() {
        return LayerTagCollection();
    }

    LayerTagCollection createSingletonTagCollection(std::string tagName);

    LayerTagCollection createTagCollection(std::list<std::string> tagNames);

    LayerTagCollection createTagCollection(std::vector<std::string> tagNames);

    // alternative non-ambiguous form to make it simple to use Boost.Assign
    LayerTagCollection createTagCollectionFromList(std::list<std::string> tagNames) {
        return createTagCollection(tagNames);
    }

    // alternative non-ambiguous form to make it simple to use Boost.Assign
    LayerTagCollection createTagCollectionFromVector(std::vector<std::string> tagNames) {
        return createTagCollection(tagNames);
    }

    static std::string getLanguageTag(std::string langCode) {
        return std::string("!") + langCode;
    }

    LayerTagCollection createLanguageTag(std::string langCode) {
        return createSingletonTagCollection(getLanguageTag(langCode));
    }

    LayerTagCollection createSingletonTagCollectionWithLangCode(
        std::string tagName,
        std::string langCode
    ) {
        return createUnion(
            createSingletonTagCollection(tagName),
            createLanguageTag(langCode)
        );
    }

    LayerTagCollection createTagCollectionFromListWithLangCode(
        std::list<std::string> tagNames,
        const std::string& langCode
    ) {
        return createUnion(
            createTagCollection(tagNames),
            createLanguageTag(langCode)
        );
    }


    LayerTagCollection createTagCollection(LayerTagMask mask);

    /**
     * returns tags sorted alphabetically
     */
    std::list<std::string> getTagNames(const LayerTagCollection& tagCollection);

    /**
     * returns a layer tag mask that matches any tag
     */
    LayerTagMask anyTag() const {
        return LayerTagMask(true);
    }

    LayerTagMask getMask(LayerTagCollection tagCollection) {
        return LayerTagMask(tagCollection);
    }

    LayerTagMask getMask(std::string tagName) {
        return getMask(createSingletonTagCollection(tagName));
    }

    LayerTagMask getMask(std::list<std::string> tagNames) {
        return getMask(createTagCollection(tagNames));
    }

    LayerTagMask getMask(std::vector<std::string> tagNames) {
        return getMask(createTagCollection(tagNames));
    }

    // alternative non-ambiguous form to make it simple to use Boost.Assign
    LayerTagMask getMaskFromList(std::list<std::string> tagNames) {
        return getMask(tagNames);
    }

    // alternative non-ambiguous form to make it simple to use Boost.Assign
    LayerTagMask getMaskFromVector(std::vector<std::string> tagNames) {
        return getMask(tagNames);
    }

    LayerTagMask getMaskWithLangCode(
        const std::string& tagName, const std::string& langCode) {

        return getMask(
            createUnion(
                createSingletonTagCollection(tagName),
                createLanguageTag(langCode)));
    }

    LayerTagMask getMaskWithLangCode(
        std::list<std::string> tagNames,
        const std::string& langCode) {

        return getMask(
            createUnion(createTagCollection(tagNames),
                        createLanguageTag(langCode)));
    }

    LayerTagMask getAlternativeMask(
        LayerTagCollection tagCollection1,
        LayerTagCollection tagCollection2) {

        std::vector<LayerTagCollection> alts =
            boost::assign::list_of
            (tagCollection1)
            (tagCollection2);

        return LayerTagMask(alts);
    }

    LayerTagMask getAlternativeMask(
        LayerTagCollection tagCollection1,
        LayerTagCollection tagCollection2,
        LayerTagCollection tagCollection3) {

        std::vector<LayerTagCollection> alts =
            boost::assign::list_of
            (tagCollection1)
            (tagCollection2)
            (tagCollection3);

        return LayerTagMask(alts);
    }

    LayerTagMask getAlternativeMask(
        std::vector<LayerTagCollection> tagCollections) {

        return LayerTagMask(tagCollections);
    }


    LayerTagCollection planeTags();
    LayerTagCollection onlyPlaneTags(LayerTagCollection tags);

    bool areInTheSamePlane(LayerTagCollection tags1, LayerTagCollection tags2);

    bool isThere(std::string tagName, LayerTagCollection tags);

    bool canBeAppliedToImplicitSymbol(const LayerTagMask& tags);

private:

    typedef boost::bimap<std::string, size_t> StringBimap;
    typedef StringBimap::value_type StringBimapItem;
    StringBimap m_;
    LayerTagCollection symbolTag_;

};

#endif
