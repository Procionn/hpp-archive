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

add_executable(MAIN
	main.cpp
)
target_link_libraries(MAIN
	HPP_ARCHIVE
)
