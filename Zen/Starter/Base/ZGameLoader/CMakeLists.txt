cmake_minimum_required (VERSION 2.6) 

project (ZGameLoader)

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED system filesystem)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZGameLoader source file


	
set ( ZGameLoader_SRC
    main.cpp
    ZGameLoader.cpp

    )

add_definitions( ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_executable( ZGameLoader ${ZGameLoader_SRC} )

add_dependencies( ZGameLoader ZenCore ZenEngine BaseCommon BaseClient )

target_link_libraries( ZGameLoader ZenCore ZenEngine BaseCommon BaseClient ${Boost_LIBRARIES} )

set_target_properties ( ZGameLoader 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZGameLoader PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZGameLoader PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)
