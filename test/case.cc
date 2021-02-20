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

	assert_fixes("Blodnäva",
		     "blodnäva");

	assert_fixes("Kambräken",
		     "kambräken");

	assert_fixes("Stenros × hartsros",
		     "stenros × hartsros");

	assert_fixes("Stor kardborre",
		     "stor kardborre");

	assert_fixes("Älggräs",
		     "älggräs");

	assert_fixes("Åkerbär",
		     "åkerbär");

	assert_fixes("Ölandskungsljus",
		     "ölandskungsljus");

	assert_fixes("Ölandskungsljus × grenigt kungsljus",
		     "ölandskungsljus × grenigt kungsljus");
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
	assert_leaves("Allanders björnbär");
	assert_leaves("Gelerts björnbär");
	assert_leaves("Köhlers björnbär");
	assert_leaves("Martensens björnbär");
	assert_leaves("Pedersens björnbär");
	assert_leaves("Raunkiaers björnbär");
	assert_leaves("Webers björnbär");
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
	assert_leaves("Bökmans maskros");
	assert_leaves("Christiansens maskros");
	assert_leaves("Dahls maskros");
	assert_leaves("Dahlstedts maskros");
    }

    void ostenfeld(TC)
    {
	assert_leaves("Ostenfelds ögontröst");
	assert_leaves("Ostenfelds maskros");
    }
}
