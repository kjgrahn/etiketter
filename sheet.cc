/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "sheet.h"
#include "xml.h"

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
}

Sheet::Sheet(const std::string& path,
	     const SharedStrings& strings)
{
    xml::Doc* doc = xmlParseFile(path.c_str());

    xml::xpath::Ctx* ctx = xmlXPathNewContext(doc);
    xmlXPathRegisterNs(ctx,  cast("ns"),
		       cast("http://schemas.openxmlformats.org/spreadsheetml/2006/main"));

    for (xml::Node* row : xml::xpath::Obj{ctx, "/ns:worksheet/ns:sheetData/ns:row"}) {

	val.emplace_back();
	auto& valr = val.back();

	for (xml::Node* cell : xml::xpath::Obj{ctx, row, "ns:c"}) {
	    valr.push_back(celltext(cell, strings));
	}
    }
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
}
