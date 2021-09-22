/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "strings.h"
#include "xml.h"

namespace {

    void nop(void*, const char*, ...) {}
}

using xlsx::SharedStrings;

SharedStrings::SharedStrings(const std::string& path)
{
    xml::Doc* doc = xmlParseFile(path.c_str());
    xmlSetGenericErrorFunc(nullptr, nop);

    xml::xpath::Ctx* ctx = xmlXPathNewContext(doc);
    xmlXPathRegisterNs(ctx,  cast("ns"),
		       cast("http://schemas.openxmlformats.org/spreadsheetml/2006/main"));

    xml::xpath::Obj match {ctx, "/ns:sst/ns:si/ns:t"};
    for (auto node: match) {
	val.push_back(content(node));
    }
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
}

const std::string& SharedStrings::operator[] (unsigned n) const
{
    return val.at(n);
}
