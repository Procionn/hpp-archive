#include "hpp-archive.h"

#include <archive.h>
#include <archive_entry.h>
#include <cstring>

ArchiveWriter::ArchiveWriter(const std::filesystem::path &archivePath)
{
    main = archive_write_new();
    archive_write_set_format_pax_restricted(main);
    archive_write_add_filter_zstd(main);

    archive_write_open_filename(main,
                                std::strcat((char *) (archivePath.c_str()),
                                            (const char *) ".tar.zst"));
}
