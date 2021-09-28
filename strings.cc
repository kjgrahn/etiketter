/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "strings.h"
#include "xml.h"

#include <cstring>
#include <vector>
#include <iterator>

namespace {

    void nop(void*, const char*, ...) {}
}

using xlsx::SharedStrings;

SharedStrings::SharedStrings(const std::string& data)
{
    xml::Doc* const doc = xml::parse_string(data);
    xmlSetGenericErrorFunc(nullptr, nop);

    xml::xpath::Ctx* const ctx = xmlXPathNewContext(doc);
    xmlXPathRegisterNs(ctx,  cast("ns"),
		       cast("http://schemas.openxmlformats.org/spreadsheetml/2006/main"));

    for (auto node: xml::xpath::Obj{ctx, "/ns:sst/ns:si/ns:t"}) {
	val.push_back(content(node));
    }
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
}

const std::string& SharedStrings::operator[] (unsigned n) const
{
    return val.at(n);
}
