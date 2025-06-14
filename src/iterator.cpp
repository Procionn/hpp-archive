#include "hpp-archive.h"

#include <archive.h>
#include <archive_entry.h>

#include <iostream>

bool error_handl(const int &error, archive* main) {
    if (error == ARCHIVE_OK)
        return true;
    if (error == ARCHIVE_EOF)
        return false;
    if (error < ARCHIVE_OK)
        std::cerr << archive_error_string(main) << std::endl;
    if (error == ARCHIVE_FATAL)
        throw ErrorClass(archive_error_string(main));

    std::cerr << "error functuion is end!" << std::endl;
    return false;
}


ArchiveReader::Iterator::Iterator (ArchiveReader* parent_) : pos(0), parent(parent_) {
    if (!error_handl(archive_read_next_header(parent->main, &parent->entry), parent->main)) {
        pos = -1;
    }
}
ArchiveReader::Iterator::Iterator () : pos(-1) {}


void ArchiveReader::Iterator::operator++() {
    if (!error_handl(archive_read_next_header(parent->main, &parent->entry), parent->main)) {
        pos = -1;
    }
    else {
        pos = 1;
    }
}


ArchiveReader::Iterator::reference  ArchiveReader::Iterator::operator*() {
    return parent->entry;
}


bool ArchiveReader::Iterator::operator==(const Iterator& other) const {
    return (pos < 0 ? true : false);
}


bool ArchiveReader::Iterator::operator!=(const Iterator& other) const {
    return (pos >= 0 ? true : false);
}

