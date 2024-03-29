/* Copyright (c) 2021 J�rgen Grahn
 * All rights reserved.
 */
#include "key.h"

bool Key::feed(const Key::V& v)
{
    map.clear();
    unsigned n = 0;
    for (const auto& name : v) {
	map[name] = n++;
    }
    return valid();
}

bool Key::valid() const
{
    auto has = [&] (const char* s) { return map.count(s); };

    return has("Startdatum") && has("Vetenskapligt namn");
}

const std::string& Key::id(const V& v) const       { return get(v, "Id"); }

const std::string& Key::name(const V& v) const     { return get(v, "Artnamn"); }
const std::string& Key::taxon(const V& v) const    { return get(v, "Vetenskapligt namn"); }
const std::string& Key::auctor(const V& v) const   { return get(v, "Auktor"); }

const std::string& Key::place(const V& v) const    { return get(v, "Lokalnamn"); }
const std::string& Key::radius(const V& v) const   { return get(v, "Noggrannhet"); }

const std::string& Key::province(const V& v) const { return get(v, "Provins"); }
const std::string& Key::kommun(const V& v) const   { return get(v, "Kommun"); }
const std::string& Key::parish(const V& v) const   { return get(v, "F�rsamling"); }

const std::string& Key::date(const V& v) const     { return get(v, "Startdatum"); }
const std::string& Key::comment(const V& v) const  { return get(v, "Kommentar"); }
const std::string& Key::biotope(const V& v) const  { return get(v, "Biotopbeskrivning"); }

const std::string& Key::leg(const V& v) const      { return get(v, "Observat�rer"); }

Key::spair Key::coordinate(const V& v) const
{
    return { get(v, "Nordkoordinat"),
	     get(v, "Ostkoordinat") };
}

/**
 * Get the column matching key 'name' in 'v'. Returns a reference to
 * an empty string if there is no such column name, or no such column,
 * or of course if it's empty.
 */
const std::string& Key::get(const V& v, const char* name) const
{
    static const std::string nil;

    auto it = map.find(name);
    if (it == end(map)) return nil;

    const unsigned n = it->second;
    if (n < v.size()) return v[n];

    return nil;
}

Record::Record(const Key& key, const Key::V& v)
    : key {key},
      v {v}
{}
