/* -*- c++ -*-
 *
 * Copyright (c) 2021 J�rgen Grahn
 * All rights reserved.
 */
#ifndef ETIKETT_KEY_H
#define ETIKETT_KEY_H

#include <string>
#include <unordered_map>
#include <vector>
#include <array>

/**
 * What turns a spreadsheet row into a Sighting.  You need
 * to know which column has which field, and they may appear
 * in different orders (or be absent I guess).
 *
 * These are the ones I got in a recent export:
 *
 *   Skyddat fynd             L�n                      Djup min
 *   Id                       Kommun                   Djup max
 *   Taxonsorteringsordning   Provins                  H�jd min
 *   Valideringsstatus        F�rsamling               H�jd max
 *   R�dlistade               Externid                 Publik samling
 *   Artnamn                  Startdatum               Privat samling
 *   Vetenskapligt namn       Starttid                 Samlingsnummer
 *   Auktor                   Slutdatum                Samlingsbeskrivning
 *   Antal                    Sluttid                  Artbest�md av
 *   Enhet                    Kommentar                Best�mnings�r
 *   �lder/stadium            Privat kommentar         Bekr�ftad av
 *   K�n                      Ej �terfunnen            Bekr�ftelse�r
 *   Aktivitet                Os�ker artbest�mning     Redigeringsansvarig
 *   L�ngd                    Best�mningsmetod         Rapport�r
 *   Vikt                     Ospontan                 Observat�rer
 *   Metod                    Biotop                   DNA sekvens
 *   Huvudlokal               Biotopbeskrivning        Projektnamn
 *   Lokalnamn                Substrat
 *   Ostkoordinat             Vetenskapligt Substratnamn
 *   Nordkoordinat            Beskrivning
 *   Noggrannhet              Substrat
 *   Diffusion                Antal substrat
 *                            Substratbeskrivning
 *
 */
class Key {
public:
    bool feed(const std::string& s);
    bool valid() const;

    using spair = std::array<std::string, 2>;
    using V = std::vector<std::string>;

    const std::string& id(const V& v) const;

    const std::string& name(const V& v) const;
    const std::string& taxon(const V& v) const;
    const std::string& auctor(const V& v) const;

    const std::string& place(const V& v) const;
    spair              coordinate(const V& v) const;
    const std::string& radius(const V& v) const;

    const std::string& province(const V& v) const;
    const std::string& kommun(const V& v) const;
    const std::string& parish(const V& v) const;

    const std::string& date(const V& v) const;
    const std::string& comment(const V& v) const;
    const std::string& biotope(const V& v) const;

    const std::string& leg(const V& v) const;

private:
    std::unordered_map<std::string, unsigned> map;

    const std::string& get(const V& v, const char* name) const;
};

/**
 * A Key applied to a row, making it possible to interpret as a
 * record.
 */
class Record {
public:
    Record(const Key& key, const std::string& s);

    using spair = Key::spair;

    std::string id()       const { return key.id(v); }

    std::string name()     const { return key.name(v); }
    std::string taxon()    const { return key.taxon(v); }
    std::string auctor()   const { return key.auctor(v); }

    std::string place()    const { return key.place(v); }
    spair  coordinate()    const { return key.coordinate(v); }
    std::string radius()   const { return key.radius(v); }

    std::string province() const { return key.province(v); }
    std::string kommun()   const { return key.kommun(v); }
    std::string parish()   const { return key.parish(v); }

    std::string date()     const { return key.date(v); }
    std::string comment()  const { return key.comment(v); }
    std::string biotope()  const { return key.biotope(v); }

    std::string leg()      const { return key.leg(v); }

private:
    const Key& key;
    const std::vector<std::string> v;
};

#endif
