#include "hpp-archive.h"

#include <archive.h>
#include <archive_entry.h>
// #include <stdexcept>

#define e(code) error_handler(code)


void ArchiveReader::writing_file (archive* disk, archive_entry* entry) {
    const void* buf;
    size_t size;
    la_int64_t offset;

    e(archive_write_header(disk, entry));

    while (e(archive_read_data_block(main, &buf, &size, &offset)))
        archive_write_data_block(disk, buf, size, offset);

    e(archive_write_finish_entry(disk));
}


ArchiveReader::ArchiveReader (const std::filesystem::path& archivePath) {
    main = archive_read_new();
    if (!main)
        throw std::runtime_error("error when opening archive");
    e(archive_read_support_format_all(main));
    e(archive_read_support_filter_all(main));
    e(archive_read_open_filename(main, archivePath.u8string().c_str(), 10240));
}


ArchiveReader::~ArchiveReader () {
    e(archive_read_close(main));
    e(archive_read_free(main));
}


const char* ArchiveReader::get_target_filename () {
    return archive_entry_pathname_utf8(entry);
}


void ArchiveReader::write_on_disk (const std::filesystem::path& filename, archive_entry* entry) {
    auto* disk = archive_write_disk_new();
    if (!disk)
        throw std::runtime_error("error when creating a file on disk");
    if (!entry)
        entry = this->entry;
    archive_entry_set_pathname_utf8(entry, filename.u8string().c_str());
    writing_file(disk, entry);
}


void ArchiveReader::write_on_disk (archive* disk, archive_entry* entry) {
    bool diskLocal = false;
    if (!disk) {
        disk = archive_write_disk_new();
        diskLocal = true;
    }
    if (!entry)
        entry = this->entry;
    if (!directory.empty())
        archive_entry_set_pathname_utf8(entry, (directory / archive_entry_pathname_utf8(entry)).u8string().c_str());
    writing_file(disk, entry);
    if (diskLocal)
        archive_write_free(disk);
}


void ArchiveReader::set_export_directory (const std::filesystem::path& dir) {
    directory = dir;
}


ArchiveReader::Iterator ArchiveReader::begin() {
    return Iterator(this);
}


ArchiveReader::Iterator ArchiveReader::end() {
    return Iterator();
}
