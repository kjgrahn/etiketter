/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "xlsx.h"

#include "sheet.h"
#include "join.h"

#include <archive.h>
#include <archive_entry.h>
#include <iostream>

namespace {

    /**
     * For use with archive_read_open() and a std::istream.
     */
    struct IStreamSource {
	explicit IStreamSource(std::istream& is) : is{is} {}
	int open(struct archive*) { return 0; }
	la_ssize_t read(struct archive* a, const void **buffer);
	int close(struct archive*) { return 0; }
    private:
	std::istream& is;
	char buf[8192];
    };

    la_ssize_t IStreamSource::read(struct archive*, const void **buffer)
    {
	is.read(buf, sizeof buf);
	*buffer = buf;
	return is.gcount();
    }

    IStreamSource& iss(void *client_data)
    {
	auto p = static_cast<IStreamSource*>(client_data);
	return *p;
    }

    int open_callback(struct archive* a, void *client_data)
    {
	return iss(client_data).open(a);
    }

    la_ssize_t read_callback(struct archive* a, void *client_data, const void **buffer)
    {
	return iss(client_data).read(a, buffer);
    }

    int close_callback(struct archive* a, void *client_data)
    {
	return iss(client_data).close(a);
    }

    /**
     * The current file, read into a string.
     */
    std::string read_file(struct archive* a)
    {
	std::string s;

	const void* buf;
	size_t len;
	off_t offset;
	while (archive_read_data_block(a, &buf, &len, &offset)==0) {
	    s.resize(offset);
	    s.append(static_cast<const char*>(buf), len);
	}
	return s;
    }
}

/**
 * Extract CSV data from the kind of simple Microsoft Excel 2007
 * documents which are the export format of Artportalen.
 *
 * The CSV format is "foo","bar","baz",... there's no attempt to quote
 * the cell contents.
 */
void xlsx::csv(std::ostream& os, std::istream& is)
{
    archive* a = archive_read_new();
    archive_read_support_format_zip(a);

    IStreamSource iss {is};
    archive_read_open(a, &iss,
		      open_callback,
		      read_callback,
		      close_callback);

    /* Accumulate the two interesting XML files into two strings. It
     * would be nice to instead stream them all the way through, but
     * that would imply learning how to feed libxml2 its input piece
     * by piece, and libxml2 sucks. Also, there's no guaranteed order
     * between the worksheet and sharedStrings.xml, which is needed to
     * use the worksheet.
     */
    struct {
	std::string sheet;
	std::string ss;
    } buf;

    archive_entry* entry;
    while (archive_read_next_header(a, &entry)==ARCHIVE_OK) {
	const std::string path = archive_entry_pathname(entry);
	if (path == "xl/worksheets/sheet1.xml") {
	    buf.sheet = read_file(a);
	}
	else if (path == "xl/sharedStrings.xml") {
	    buf.ss = read_file(a);
	}
	if (buf.sheet.size() && buf.ss.size()) break;
    }

    archive_read_free(a);

    if (buf.sheet.empty() || buf.ss.empty()) {
	/* Should analyze the cause for this more, and report it
	 * rather than just emit no CSV data. The trigger would be an
	 * archive_foo() error return, or failure to find one of the
	 * files we're looking for.
	 */
	return;
    }

    const SharedStrings ss {buf.ss};
    const Sheet sheet {buf.sheet, ss};
    buf.ss.clear();
    buf.sheet.clear();

    for (const auto& row : sheet) {
	util::join_to(os << '"', "\",\"",
		      row.begin(), row.end()) << "\"\n";
    }
}
