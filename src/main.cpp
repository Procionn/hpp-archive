#include "hpp-archive.h"
#include <archive_entry.h>
#include <archive.h>
#include <iostream>

int main() {
    setlocale(LC_ALL, "");
#if 0
    ArchiveReader archive("tst.tar.gz");
    // archive.set_directory("testing/tmp");

    for (auto* entry : archive) {
        archive.write_on_disk();
    }
#endif
    ArchiveWriter archive("testing_archive");

    archive.write_in_archive("first.gif");
    archive.write_in_archive("CMakeFiles", "CMake");



    return 0;
}
