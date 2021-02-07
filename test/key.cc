#include <key.h>

#include <split.h>

#include <orchis.h>

#include <vector>
#include <string>

namespace key {

    using orchis::TC;
    using orchis::assert_false;
    using orchis::assert_true;
    using orchis::assert_eq;

    namespace {

	std::string join(const std::vector<std::string>& ss)
	{
	    return ::join("\",\"", ss);
	}

	void assert_invalid(Key& key, const std::vector<std::string>& ss)
	{
	    assert_false(key.feed(join(ss)));
	    assert_false(key.valid());
	}

	void assert_valid(Key& key, const std::vector<std::string>& ss)
	{
	    assert_true(key.feed(join(ss)));
	    assert_true(key.valid());
	}
    }

    void creation(TC)
    {
	Key key;
	assert_false(key.valid());
	assert_invalid(key, {});
	assert_invalid(key, {"foo", "bar"});
	assert_invalid(key, {"Starttid", "Provins"});

	assert_valid(key, {"DNA sekvens",
			   "Vetenskapligt namn", "Startdatum",
			   "Bestämningsår"});
    }

    namespace lookup {

	Key minkey()
	{
	    Key key;
	    assert_valid(key,
			 {"Vetenskapligt namn", "Auktor",
			  "Ostkoordinat", "Nordkoordinat", "Provins", "Startdatum"});
	    return key;
	}

	void simple(TC)
	{
	    const Key key = minkey();
	    const Record e {key, join(
		    {"Salix", "L.",
		     "4711", "6869", "Vg", "2021-02-10"})};

	    assert_eq(e.taxon(),  "Salix");
	    assert_eq(e.auctor(), "L.");
	    assert_eq(e.coordinate()[0], "6869");
	    assert_eq(e.coordinate()[1], "4711");
	    assert_eq(e.province(), "Vg");
	    assert_eq(e.date(),     "2021-02-10");
	}

	void failure(TC)
	{
	    const Key key = minkey();
	    const Record e {key, join(
		    {"Salix", "L.",
		     "4711", "6869", "Vg"})};

	    assert_eq(e.taxon(),  "Salix");

	    assert_eq(e.date(),  "");
	    assert_eq(e.radius(),  "");
	}
    }
}
