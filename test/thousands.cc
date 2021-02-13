#include <thousands.h>

#include <split.h>

#include <orchis.h>

#include <vector>
#include <string>

namespace thousands {

    using orchis::TC;

    void assert_splits(const std::string& s, const char* const ref)
    {
	const Thousands t {s, "."};
	std::ostringstream oss;
	oss << t;
	orchis::assert_eq(oss.str(), ref);
    }

    void simple(TC)
    {
	assert_splits("1000", "1.000");
    }

    void small(TC)
    {
	assert_splits(  "1",   "1");
	assert_splits( "12",  "12");
	assert_splits("123", "123");
    }

    void large(TC)
    {
	assert_splits("1234",
		      "1.234");
	assert_splits("12345",
		      "12.345");
	assert_splits("123456",
		      "123.456");
	assert_splits("1234567",
		      "1.234.567");
	assert_splits("dabcabcabcabcabcabc",
		      "d.abc.abc.abc.abc.abc.abc");
    }

    void empty(TC)
    {
	assert_splits("", "");
    }
}
