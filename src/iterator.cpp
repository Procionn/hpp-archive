#include "hpp-archive.h"

#include <archive.h>
#include <archive_entry.h>



void ArchiveReader::Iterator::operator++ () {
    if (archive_read_next_header(parent->main, &parent->entry) != ARCHIVE_OK) {
	pos = -1;
    }
    else {
	
    }
}


archive_entry* ArchiveReader::Iterator::operator* () {
    return parent->entry;
}
