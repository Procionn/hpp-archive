#include "hpp-archive.h"

#include <archive.h>
#include <archive_entry.h>

#include <iostream>


bool BaseArchive::error_handler(const int &error) {
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

ErrorClass::ErrorClass(const char *code): message(code){}

const char *ErrorClass::what() {
    return message;
}
