#include <coordinate.h>

#include <orchis.h>

#include <vector>
#include <string>
#include <algorithm>

namespace {

    bool contains(const std::string& haystack,
		  const std::string& needle)
    {
	auto it = std::search(begin(haystack), end(haystack),
			      begin(needle), end(needle));
	return it != end(haystack);
    }
}

namespace coordinate {

    using orchis::TC;

    void assert_formats(const Coordinate& c,
			const std::vector<std::string>& ref)
    {
	std::ostringstream oss;
	oss << c;
	const auto s = oss.str();

	for (const auto& rs: ref) {

	    if (!contains(s, rs)) {
		throw orchis::Failure("'" + s + "' contains '" + rs + "'");
	    }
	}
    }

    void empty(TC)
    {
	assert_formats({{"", ""}, ""}, {});
    }

    void generic(TC)
    {
	assert_formats({{"foo", "bar"}, "baz"},
		       {"foo", "bar", "baz"});
    }

    namespace rt90 {

	void simple(TC)
	{
	    assert_formats({{"6399400", "1270800"}, "10"},
			   {"63", "99400", "12", "70800",
			    "RT90", "10"});
	}

	void no_radius(TC)
	{
	    assert_formats({{"6399400", "1270800"}, ""},
			   {"63", "99400", "12", "70800",
			    "RT90"});
	}
    }

    namespace sweref99 {

	void simple(TC)
	{
	    assert_formats({{"6391427", "322318"}, "19"},
			   {"6391427", "322318",
			    "SWEREF99", "19"});
	}
    }

    namespace wgs84 {

	void simple(TC)
	{
	    assert_formats({{"57.63009", "12.02405"}, "50"},
			   {"57.63009", "12.02405",
			    "WGS", "84", "50"});
	}
    }
}
