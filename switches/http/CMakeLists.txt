# libjson
find_package(LibJson)
# libxml2
find_package(LibXml2)
if (LIBXML2_FOUND AND LIBJSON_FOUND)
	
        include_directories(${LIBXML2_INCLUDE_DIR} ${LIBJSON_INCLUDE_DIR})

	set(http_SRCS
		http.cpp
		mongoose.c
	)
	set(http_MOCS
		http.h
		mongoose.h
	)
	
	add_executable(asebahttp ${http_SRCS} ${http_MOCS})
	
	target_link_libraries(asebahttp asebacompiler ${LIBXML2_LIBRARIES} ${LIBJSON_LIBRARY} ${ASEBA_CORE_LIBRARIES})

	install(TARGETS asebahttp RUNTIME
		DESTINATION bin
	)
endif (LIBXML2_FOUND AND LIBJSON_FOUND)