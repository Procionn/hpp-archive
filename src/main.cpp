#include "hpp-archive.h"
#include <archive_entry.h>
#include <archive.h>
#include <iostream>

int main() {
    setlocale(LC_ALL, "");
    ArchiveReader archive("tst.tar.gz");
    archive.set_directory("testing/tmp");

    for (auto* entry : archive) {
        archive.write_on_disk();
    }




    return 0;
}
