#ifndef ANNOTATION_ITEM_MANAGER_HDR
#define ANNOTATION_ITEM_MANAGER_HDR

#include <list>
#include <map>
#include <string>

#include <boost/bimap.hpp>

#include "annotation_item.hpp"
#include "zvalue.hpp"


class AnnotationItemManager {

public:

    AnnotationItemManager();

    ~AnnotationItemManager();

    /**
     * Sets value to be stored as a zvalue's string representation.
     */
    void setStringValue(AnnotationItem & annotationItem, std::string attribute, std::string value);

    /**
     * Sets value and stores it as zvalue-string if it's not a number.
     * Numbers are stored as zvalue-integers.
     */
    void setValue(AnnotationItem & annotationItem, std::string attribute, std::string value);

    void setValue(AnnotationItem & annotationItem, std::string attribute, int value);
    void setValue(AnnotationItem & annotationItem, std::string attribute, zvalue value);

    zvalue getValue(AnnotationItem & annotationItem, std::string attribute);
    std::string getValueAsString(AnnotationItem & annotationItem, std::string attribute);

    std::string getCategory(AnnotationItem & annotationItem) const {
        return annotationItem.category_;
    }

    std::list< std::pair<std::string, zvalue> > getValuesAsZvalues(
        const AnnotationItem & annotationItem
    );

    std::list< std::pair<std::string, std::string> > getValues(
        const AnnotationItem & annotationItem
    );

    std::map<std::string, std::string> getAVMap(
        const AnnotationItem & annotationItem
    );

    //Converter & master stuff:

    zvalue stringToZvalue(std::string s) const;
    std::string zvalueToString(zvalue z) const;

    bool is_int(zvalue value) const;
    bool is_string(zvalue value) const;

    int to_int(zvalue value) const;
    std::string to_string(zvalue value) const;

    std::string string_representation(zvalue value) const;

    zvalue from_int(int i);
    zvalue from_string(const std::string& s);
    zvalue from_bool(bool b);

    zvalue false_value() const;
    zvalue any_value() const;

    bool is_false(zvalue value) const;
    bool is_true(zvalue value) const;
    bool is_any(zvalue value) const;

private:

    typedef boost::bimap<std::string, int> StringBimap;
    typedef StringBimap::value_type StringBimapItem;
    StringBimap m_;
    zobjects_holder * zObjectsHolder_;
    zsymboltable * zSymbolTable_;
    zsymbolfactory * zSymbolFactory_;

    std::string fail_string_;
    std::string false_string_;
    std::string empty_string_;
    std::string any_string_;
    std::string nil_string_;

};


#endif
