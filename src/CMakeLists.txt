include(FetchContent)

add_library(HPP_ARCHIVE STATIC
	hpp-archive.h
	ArchiveWriter.cpp
	ArchiveReader.cpp
	base-archive.cpp
	iterator.cpp
)

FetchContent_Declare(
  libarchive
  GIT_REPOSITORY https://github.com/libarchive/libarchive.git
  GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(libarchive)

target_link_libraries(HPP_ARCHIVE
	PRIVATE
	archive
)
