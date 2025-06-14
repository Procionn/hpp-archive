#include "hpp-archive.h"

#include <archive.h>
#include <archive_entry.h>
#include <fstream>
#include <iostream>

ArchiveWriter::ArchiveWriter(const std::filesystem::path& archivePath) : path(archivePath) {
    open();
}


ArchiveWriter::~ArchiveWriter () {
    error_handler(archive_write_close(main));
    error_handler(archive_write_free(main));
}


void ArchiveWriter::write_in_archive(const std::filesystem::path& path,
                                     const std::filesystem::path& pathInArchive) {
    entry = archive_entry_new();
    if (is_directory(path)) {
        for (const auto& target : std::filesystem::recursive_directory_iterator(path)) {
            std::filesystem::path relative_path = std::filesystem::relative(target.path(), path);
            archive_entry_set_pathname(entry, (pathInArchive / relative_path).u8string().c_str());

            if (target.is_directory()) {
                archive_entry_set_filetype(entry, AE_IFDIR);
                archive_entry_set_perm(entry, 0755);
                archive_write_header(main, entry);
            }
            else if (target.is_regular_file())
                write_file(target.path());
            else
                std::cerr << "HPP-ARCHIVE ERROR: not supported file type" << std::endl;

            archive_entry_clear(entry);
        }
    }
    else if (is_regular_file(path)) {
        archive_entry_set_pathname(entry, (pathInArchive / path.filename()).u8string().c_str());
        write_file(path);
    }

    archive_entry_free(entry);
}


void ArchiveWriter::open () {
    main = archive_write_new();
    archive_write_set_format_pax_restricted(main);
    archive_write_add_filter_zstd(main);

    archive_write_open_filename(main, (path += ".tar.zst").u8string().c_str());
}


void ArchiveWriter::write_file (const std::filesystem::path& target) {
    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_size(entry, file_size(target));
    archive_entry_set_perm(entry, 0644);
    archive_write_header(main, entry);

    std::ifstream file(target, std::ios::binary);
    char buffer[divisor];

    while (file.read(buffer, divisor))
        archive_write_data(main, buffer, file.gcount());
    archive_write_data(main, buffer, file.gcount());
}
