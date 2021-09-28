/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "sheet.h"
#include "xml.h"
#include "utf8.h"

#include <string>
#include <stdexcept>

using xlsx::SharedStrings;
using xlsx::Sheet;

namespace {

    /**
     * The text contents of a <c>. Such a cell is, in the Excel export
     * from Artportalen I'm looking at, on either of these forms:
     *
     * <c t="s">             string content; the string is
     *   <v>38</v>           in SharedStrings
     * </c>
     * <c>                   numerical content in decimal
     *   <v>91418341</v>     notation
     * </c>
     *
     */
    std::string celltext(xml::Node* const cell,
			 const SharedStrings& strings)
    {
	xml::Node* val = xmlFirstElementChild(cell);
	if (!val) return "-";
	if (attribute(cell, "t") == "s") {
	    try {
		unsigned n = std::stoul(content(val));
		return strings[n];
	    }
	    catch (const std::logic_error&) {
		return "-";
	    }
	}
	return content(val);
    }

    /**
     * Decoding UTF-8 to Latin-1 in XML content. Characters which do
     * not fit (emojis, fancy quotes, foreign alphabets, ...) are
     * replaced with a '?'.  Undefined result on broken UTF-8.
     */
    class Utf8 {
    public:
	std::string decode(const std::string& s);

    private:
	std::vector<unsigned> v;
    };
}

std::string Utf8::decode(const std::string& s)
{
    const unsigned char* p = reinterpret_cast<const unsigned char*>(s.data());
    const auto q = p + s.size();
    v.clear();
    auto it = std::back_inserter(v);
    utf8::decode(p, q, it);
    for (auto& ch : v) {
	if (ch>0xff) ch = '?';
    }
    return {v.begin(), v.end()};
}

Sheet::Sheet(const std::string& data,
	     const SharedStrings& strings)
{
    Utf8 utf8;
    xml::Doc* const doc = xml::parse_string(data);

    xml::xpath::Ctx* const ctx = xmlXPathNewContext(doc);
    xmlXPathRegisterNs(ctx,  cast("ns"),
		       cast("http://schemas.openxmlformats.org/spreadsheetml/2006/main"));

    for (xml::Node* row : xml::xpath::Obj{ctx, "/ns:worksheet/ns:sheetData/ns:row"}) {

	val.emplace_back();
	auto& valr = val.back();

	for (xml::Node* cell : xml::xpath::Obj{ctx, row, "ns:c"}) {
	    valr.push_back(utf8.decode(celltext(cell, strings)));
	}
    }
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
}
