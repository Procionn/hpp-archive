add_library(HPP_ARCHIVE STATIC
	hpp-archive.h
	ArchiveWriter.cpp
	ArchiveReader.cpp
	base-archive.cpp
	iterator.cpp
)

target_link_libraries(HPP_ARCHIVE
	PRIVATE
	archive
)

target_include_directories(HPP_ARCHIVE PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
