#pragma once

#include <filesystem>

class BaseArchive
{
protected:
    struct archive* main;
    struct archive_entry* entry = nullptr;

    BaseArchive() = default;
    ~BaseArchive() = default;

    bool error_handler(const int&);
};





class ArchiveReader : public BaseArchive
{
friend class Iterator;
friend class ArchiveWriter;
class Iterator
    {
        signed char pos = 0;
        ArchiveReader* parent = nullptr;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = archive_entry*;
        using difference_type   = archive_entry*;
        using pointer           = archive_entry*;
        using reference         = archive_entry*&;


        explicit Iterator(ArchiveReader* parent_);
        explicit Iterator();
        void operator++();
        reference operator*();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };
protected:
    std::filesystem::path directory = "";

    void writing_file(archive* disk, archive_entry* entry);

public:
    ArchiveReader(const std::filesystem::path& archivePath);

    ~ArchiveReader();

#ifdef __linux__
    const char* get_target_filename();
#elif WIN64
    const wchar_t* get_target_filename();
#endif

    void write_on_disk(const std::filesystem::path& filename, archive_entry* = nullptr);

    void write_on_disk(archive* disk = nullptr, archive_entry* = nullptr);

    void set_export_directory(const std::filesystem::path& directory);

    Iterator begin();
    Iterator end();
};





class ArchiveWriter : public BaseArchive
{
protected:
    std::filesystem::path path;
    static constexpr unsigned divisor = 8192;
    std::filesystem::path expansion;

public:
    ArchiveWriter(const std::filesystem::path& archivePath,
                  const std::filesystem::path& expansion = ".tar.zst");

    virtual ~ArchiveWriter();

    void write_in_archive(const std::filesystem::path& path,
                          const std::filesystem::path& pathInArchive = "");

    void clone(ArchiveReader*);

protected:
    virtual void open();

    void write_file(const std::filesystem::path& target);

};
