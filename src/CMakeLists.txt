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
