add_custom_command(
    OUTPUT run-e2e-http.sh
    COMMAND perl ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../patch-script.perl 
    	    ${CMAKE_CURRENT_SOURCE_DIR}/run-e2e-http.sh.in 
	    ${CMAKE_CURRENT_BINARY_DIR}/run-e2e-http.sh 
	    ASEBAHTTP=$<TARGET_FILE:asebahttp> 
	    ASEBADUMMYNODE=$<TARGET_FILE:asebadummynode> 
	    ASEBASWITCH=$<TARGET_FILE:asebaswitch> 
	    ASEBAMASSLOADER=$<TARGET_FILE:asebamassloader>
	    DASHELPORTLIST=${dashel_DIR}/portlist
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/run-e2e-http.sh.in
)
add_custom_target(
    create-run-e2e-http ALL
    DEPENDS run-e2e-http.sh
)

add_custom_command(
    OUTPUT run-e2e-switch.sh
    COMMAND perl ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../patch-script.perl 
    	    ${CMAKE_CURRENT_SOURCE_DIR}/run-e2e-switch.sh.in 
	    ${CMAKE_CURRENT_BINARY_DIR}/run-e2e-switch.sh 
	    ASEBAHTTP=$<TARGET_FILE:asebahttp> 
	    ASEBADUMMYNODE=$<TARGET_FILE:asebadummynode> 
	    ASEBASWITCH=$<TARGET_FILE:asebaswitch> 
	    ASEBAMASSLOADER=$<TARGET_FILE:asebamassloader>
	    DASHELPORTLIST=${dashel_DIR}/portlist
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/run-e2e-switch.sh.in
)
add_custom_target(
    create-run-e2e-switch ALL
    DEPENDS run-e2e-switch.sh
)

find_program( HAVE_NPM npm )

if (HAVE_NPM)
    if (WIN32)
        set(NPM_INSTALL cmd //c npm install)
    else (WIN32)
        set(NPM_INSTALL npm install)
    endif (WIN32)
    add_custom_target(
	install_jasmine ALL
	COMMAND ${NPM_INSTALL}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Installing jasmine"
    )
endif (HAVE_NPM)

configure_file(3000-1-http_spec.js ${CMAKE_CURRENT_BINARY_DIR}/3000-1-http_spec.js COPYONLY)
configure_file(3000-2-interaction_spec.js ${CMAKE_CURRENT_BINARY_DIR}/3000-2-interaction_spec.js COPYONLY)
configure_file(3001-1-http_spec.js ${CMAKE_CURRENT_BINARY_DIR}/3001-1-http_spec.js COPYONLY)
configure_file(3001-2-remap_spec.js ${CMAKE_CURRENT_BINARY_DIR}/3001-2-remap_spec.js COPYONLY)
configure_file(3001-3-load-aesl_spec.js ${CMAKE_CURRENT_BINARY_DIR}/3001-3-load-aesl_spec.js COPYONLY)
configure_file(3002-1-thymio-II_spec.js ${CMAKE_CURRENT_BINARY_DIR}/3002-1-thymio-II_spec.js COPYONLY)
configure_file(3003-1-dashel-target_spec.js ${CMAKE_CURRENT_BINARY_DIR}/3003-1-dashel-target_spec.js COPYONLY)
configure_file(3004-1-switch_spec.js ${CMAKE_CURRENT_BINARY_DIR}/3004-1-switch_spec.js COPYONLY)
configure_file(3005-1-switch_spec.js ${CMAKE_CURRENT_BINARY_DIR}/3005-1-switch_spec.js COPYONLY)
configure_file(package.json ${CMAKE_CURRENT_BINARY_DIR}/package.json COPYONLY)
configure_file(ping0123.aesl ${CMAKE_CURRENT_BINARY_DIR}/ping0123.aesl COPYONLY)
configure_file(dice.aesl ${CMAKE_CURRENT_BINARY_DIR}/dice.aesl COPYONLY)
configure_file(thymio.aesl ${CMAKE_CURRENT_BINARY_DIR}/thymio.aesl COPYONLY)
configure_file(README.md ${CMAKE_CURRENT_BINARY_DIR}/README.md COPYONLY)

add_test(NAME e2e-http COMMAND bash run-e2e-http.sh)