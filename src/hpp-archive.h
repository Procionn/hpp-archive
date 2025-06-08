#pragma once

#include <filesystem>
#include <exception>
#include <iterator>

class ErrorClass : public std::exception
{
public:
    ErrorClass(const char*);
    const char* what();
private:
    const char* message;
};





class BaseArchive
{
protected:
    struct archive* main;
    struct archive_entry* entry;

    BaseArchive() = default;
    ~BaseArchive() = default;

    bool error_handler(const int&);
};




class ArchiveReader : BaseArchive
{
public:
    ArchiveReader(const std::filesystem::path& archivePath);

    ~ArchiveReader() = default;

private:
friend class Iterator;
class Iterator : public std::iterator<std::input_iterator_tag,
    archive_entry*,
    archive_entry*,
    const archive_entry**,
    archive_entry*
>{
    long pos = 0;
    ArchiveReader* parent;
public:
    explicit inline Iterator(ArchiveReader* parent_) : parent(parent_) {}
    void operator++ ();
    archive_entry* operator*();
    bool inline operator==(const Iterator& other) const {return (pos >= 0 ? true : false);}
    bool inline operator!=(const Iterator& other) const {return (pos >= 0 ? false : true);}
};
};





class ArchiveWriter : BaseArchive
{
public:
    ArchiveWriter(const std::filesystem::path& archivePath);

    ~ArchiveWriter() = default;
};
