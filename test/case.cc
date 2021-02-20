#include <case.h>

#include <orchis.h>

#include <vector>
#include <string>

namespace casefix {

    using orchis::TC;

    void assert_fixes(const char* s, const char* ref)
    {
	orchis::assert_eq(fix_case(s), ref);
    }

    void assert_leaves(const char* s)
    {
	orchis::assert_eq(fix_case(s), s);
    }

    void empty(TC)
    {
	assert_leaves("");
    }

    void fixes(TC)
    {
	assert_fixes("Ag",
		     "ag");

	assert_fixes("En",
		     "en");

	assert_fixes("Blodn�va",
		     "blodn�va");

	assert_fixes("Kambr�ken",
		     "kambr�ken");

	assert_fixes("Stenros � hartsros",
		     "stenros � hartsros");

	assert_fixes("Stor kardborre",
		     "stor kardborre");

	assert_fixes("�lggr�s",
		     "�lggr�s");

	assert_fixes("�kerb�r",
		     "�kerb�r");

	assert_fixes("�landskungsljus",
		     "�landskungsljus");

	assert_fixes("�landskungsljus � grenigt kungsljus",
		     "�landskungsljus � grenigt kungsljus");
    }

    void pedicularis(TC)
    {
	assert_fixes("Kung Karls spira", "kung Karls spira");
    }

    void adam(TC)
    {
	assert_leaves("Adam och Eva");
	assert_leaves("Jungfru Marie nycklar");
	assert_leaves("Sankt Pers nycklar");
    }

    void rubus(TC)
    {
	assert_leaves("Allanders bj�rnb�r");
	assert_leaves("Gelerts bj�rnb�r");
	assert_leaves("K�hlers bj�rnb�r");
	assert_leaves("Martensens bj�rnb�r");
	assert_leaves("Pedersens bj�rnb�r");
	assert_leaves("Raunkiaers bj�rnb�r");
	assert_leaves("Webers bj�rnb�r");
    }

    void hieracium(TC)
    {
	assert_leaves("Lars-Levis fibbla");
	assert_leaves("Wolfs fibbla");
    }

    void taraxacum(TC)
    {
	assert_leaves("Birgittamaskros");
	assert_leaves("Blomgrens maskros");
	assert_leaves("Borgvalls maskros");
	assert_leaves("Broddesons maskros");
	assert_leaves("B�kmans maskros");
	assert_leaves("Christiansens maskros");
	assert_leaves("Dahls maskros");
	assert_leaves("Dahlstedts maskros");
    }

    void ostenfeld(TC)
    {
	assert_leaves("Ostenfelds �gontr�st");
	assert_leaves("Ostenfelds maskros");
    }
}
