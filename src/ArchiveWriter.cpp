#include "hpp-archive.h"

#include <archive.h>
#include <archive_entry.h>
#include <fstream>
#include <cassert>

ArchiveWriter::ArchiveWriter(const std::filesystem::path& archivePath, const std::filesystem::path& exp)
    : path(archivePath), expansion(exp) {
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

#ifdef __linux__
            archive_entry_set_pathname(entry, (pathInArchive / relative_path).u8string().c_str());
#elif WIN32
            archive_entry_copy_pathname_w(entry, (pathInArchive / relative_path).wstring().c_str());
#endif

            if (target.is_directory()) {
                archive_entry_set_filetype(entry, AE_IFDIR);
                archive_entry_set_perm(entry, 0755);
                archive_write_header(main, entry);
            }
            else if (target.is_regular_file())
                write_file(target.path());
            else
                std::runtime_error("HPP-ARCHIVE ERROR: not supported file type");

            archive_entry_clear(entry);
        }
    }
    else if (is_regular_file(path)) {
#ifdef __linux__
        archive_entry_set_pathname(entry, (pathInArchive / path.filename()).u8string().c_str());
#elif WIN32
        archive_entry_copy_pathname_w(entry, (pathInArchive /  path.filename()).wstring().c_str());
#endif
        write_file(path);
    }

    archive_entry_free(entry);
}


void ArchiveWriter::open () {
    main = archive_write_new();
    archive_write_set_format_pax_restricted(main);
    archive_write_add_filter_zstd(main);

#ifdef __linux__
    archive_write_open_filename(main, (path += expansion).u8string().c_str());
#elif WIN32
    archive_write_open_filename_w(main, (path += expansion).wstring().c_str());
#endif
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



void ArchiveWriter::clone (ArchiveReader* cloned) {
    assert(cloned);

    char buf[divisor];
    size_t size = divisor;
    la_ssize_t gcount;

    for (auto start = cloned->begin(), end = cloned->end(); start != end; ++start) {
        entry = *start;
        error_handler(archive_write_header(main, entry));
        while((gcount = archive_read_data(cloned->main, &buf, size)) == size)
            archive_write_data(main, &buf, gcount);
        archive_write_data(main, &buf, gcount);
    }
}
