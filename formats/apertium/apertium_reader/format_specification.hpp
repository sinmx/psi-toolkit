#ifndef APERTIUM_FORMAT_SPECIFICATION_HDR
#define APERTIUM_FORMAT_SPECIFICATION_HDR

#include <string>

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

#include "format_rules.hpp"
#include "xml_property_tree.hpp"


class FormatSpecification {
public:

    FormatSpecification(FormatOptions options,
        std::vector<FormatRule> formatRules, std::vector<ReplacementRule> replacementRules);

    FormatOptions getOptions();
    std::vector<FormatRule> getFormatRules();
    std::vector<ReplacementRule> getReplacementRules();

private:

    FormatOptions formatOptions_;
    std::vector<FormatRule> formatRules_;
    std::vector<ReplacementRule> replacementRules_;

};


class FormatSpecificationReader {
public:

    class Exception : public std::exception {
    public:
        Exception(const std::string& msg) : msg_(msg) { }
        virtual ~Exception() throw() { }

        virtual const char* what() const throw() {
            return msg_.c_str();
        }
    private:
        std::string msg_;
    };

    class UnexpectedElementException : public Exception {
    public:
        UnexpectedElementException(const std::string& elementName)
            : Exception(std::string("unexpected element `") + elementName + "'") { }
    };


    FormatSpecificationReader(const boost::filesystem::path& filePath);

    FormatSpecification readFormatSpecification();

private:

    boost::shared_ptr<XmlPropertyTree> xmlParsed_;

    FormatOptions parseOptions_();
    std::vector<FormatRule> parseFormatRules_();
    std::vector<ReplacementRule> parseReplacementRules_();

};

#endif
