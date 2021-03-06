# asebahttp - a switch to bridge HTTP to Aseba
# 2014-12-01 David James Sherman <david dot sherman at inria dot fr>

if (CMAKE_VERSION VERSION_LESS "3.1")
	if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
	endif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
else (CMAKE_VERSION VERSION_LESS "3.1")
	set (CMAKE_CXX_STANDARD 11)
endif (CMAKE_VERSION VERSION_LESS "3.1")

# need libxml2 to read aesl files
find_package(LibXml2)
if (LIBXML2_FOUND)
	
        include_directories(${LIBXML2_INCLUDE_DIR})

	set(http_SRCS
		http.cpp
		main.cpp
	)
	set(http_MOCS
		http.h
	)
	
	add_executable(asebahttp ${http_SRCS} ${http_MOCS})
	
	target_link_libraries(asebahttp asebacompiler asebacommon ${LIBXML2_LIBRARIES} ${ASEBA_CORE_LIBRARIES})

	install(TARGETS asebahttp RUNTIME
		DESTINATION bin
	)

	add_library(asebahttphub ${http_SRCS})
	set_target_properties(asebahttphub PROPERTIES VERSION ${LIB_VERSION_STRING} 
						    SOVERSION ${LIB_VERSION_MAJOR})

	install(TARGETS asebahttphub
			LIBRARY DESTINATION ${LIB_INSTALL_DIR} 
			ARCHIVE DESTINATION ${LIB_INSTALL_DIR} 
	)

	set (ASEBACORE_HDR_HTTP
		http.h
	)
	install(FILES ${ASEBACORE_HDR_HTTP}
		DESTINATION include/aseba/switches/http
	)

endif (LIBXML2_FOUND)
