/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef XLSX_XML
#define XLSX_XML

#include <vector>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

inline
const char* cast(const xmlChar* s)
{
    return reinterpret_cast<const char*>(s);
}

inline
const xmlChar* cast(const char* s)
{
    return reinterpret_cast<const xmlChar*>(s);
}

namespace xml {
    using Doc = xmlDoc;
    using Node = xmlNode;

    namespace xpath {
	using Ctx = xmlXPathContext;

	// wrapper for xmlXPathObject
	class Obj {
	public:
	    Obj(Ctx* ctx, const char* expr)
		: val(xmlXPathEvalExpression(cast(expr), ctx))
	    {}
	    Obj(Ctx* ctx, Node* root, const char* expr)
		: val(xmlXPathNodeEval(root, cast(expr), ctx))
	    {}
	    ~Obj()
	    {
		xmlXPathFreeObject(val);
	    }

	    Node** begin() const { return val->nodesetval->nodeTab; }
	    Node** end() const { return begin() + val->nodesetval->nodeNr; }

	private:
	    xmlXPathObject* val;
	};
    }

    inline
    xml::Doc* parse_string(const std::string& s)
    {
	return xmlParseMemory(s.data(), s.size());
    }
}

inline
std::vector<xml::Node*> nodes(xml::xpath::Obj& obj)
{
    return {obj.begin(), obj.end()};
}

inline
std::string content(xml::Node* node)
{
    auto p = xmlNodeGetContent(node);
    std::string s = cast(p);
    xmlFree(p);
    return s;
}

inline
std::string attribute(xml::Node* node, const char* name)
{
    auto p = xmlGetProp(node, cast(name));
    if (!p) return "";
    std::string s = cast(p);
    xmlFree(p);
    return s;
}

#endif
