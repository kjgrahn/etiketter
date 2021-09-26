/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "strings.h"
#include "xml.h"
#include "utf8.h"

#include <cstring>
#include <vector>
#include <iterator>

namespace {

    void nop(void*, const char*, ...) {}

    /**
     * Decoding UTF-8 to Latin-1 in XML content. Characters which do
     * not fit (emojis, fancy quotes, foreign alphabets, ...) are
     * replaced with a '?'.  Undefined result on broken UTF-8.
     */
    class Utf8 {
    public:
	std::string decode(xml::Node* node);

    private:
	std::vector<unsigned> v;
    };
}

std::string Utf8::decode(xml::Node* node)
{
    const auto p = xmlNodeGetContent(node);
    const auto q = p + std::strlen(cast(p));
    v.clear();
    auto it = std::back_inserter(v);
    utf8::decode(p, q, it);
    xmlFree(p);
    for (auto& ch : v) {
	if (ch>0xff) ch = '?';
    }
    return {v.begin(), v.end()};
}

using xlsx::SharedStrings;

SharedStrings::SharedStrings(const std::string& data)
{
    xml::Doc* const doc = xml::parse_string(data);
    xmlSetGenericErrorFunc(nullptr, nop);

    xml::xpath::Ctx* const ctx = xmlXPathNewContext(doc);
    xmlXPathRegisterNs(ctx,  cast("ns"),
		       cast("http://schemas.openxmlformats.org/spreadsheetml/2006/main"));

    Utf8 utf8;
    for (auto node: xml::xpath::Obj{ctx, "/ns:sst/ns:si/ns:t"}) {
	val.push_back(utf8.decode(node));
    }
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
}

const std::string& SharedStrings::operator[] (unsigned n) const
{
    return val.at(n);
}
