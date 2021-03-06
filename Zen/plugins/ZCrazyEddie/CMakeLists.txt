cmake_minimum_required (VERSION 2.6) 

project (ZCrazyEddie)
set ( PROJECT_PREFIX "plugins/ZCrazyEddie" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )
Find_Package(SDL)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZCrazyEddie source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZCrazyEddie_SRC
    Configuration.hpp
    I_ImageSet.hpp
    I_ImageSetService.hpp
    I_SchemeService.hpp
    I_WidgetService.hpp
    src/FontService.cpp
    src/FontService.hpp
    src/ImageSet.cpp
    src/ImageSet.hpp
    src/ImageSetService.cpp
    src/ImageSetService.hpp
    src/I_ImageSet.cpp
    src/I_ImageSetService.cpp
    src/I_SchemeService.cpp
    src/I_WidgetService.cpp
    src/SchemeService.cpp
    src/SchemeService.hpp
    src/Widget.cpp
    src/Widget.hpp
    src/WidgetEvent.cpp
    src/WidgetEvent.hpp
    src/WidgetProperty.cpp
    src/WidgetProperty.hpp
    src/WidgetRenderer.cpp
    src/WidgetRenderer.hpp
    src/WidgetService.cpp
    src/WidgetService.hpp
    src/WidgetServiceFactory.cpp
    src/WidgetServiceFactory.hpp
    src/ZCrazyEddiePlugin.cpp

    )

add_definitions( -DZCRAZYEDDIE_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${OGRE_INCLUDE_DIR} ${CEGUI_INCLUDEDIR} ${CEGUI_INCLUDEDIR}/CEGUI ${CEGUI_OGRE_INCLUDEDIR} ${LUA_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZCrazyEddie ${LIB_TYPE} ${ZCrazyEddie_SRC} )

add_dependencies( ZCrazyEddie ZenCore ZenEngine ZOgre ZLua )

if (NOT MSVC_IDE)
set(CEGUI_LIBRARIES ${CEGUI_LIBRARIES} CEGUILuaScriptModule)
endif (NOT MSVC_IDE)

target_link_libraries( ZCrazyEddie ZOgre ZLua  ${OGRE_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_OGRE_LIBRARIES} ${LUA_LIBRARY} ${SDL_LIBRARY} )

set_target_properties ( ZCrazyEddie 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZCrazyEddie PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZCrazyEddie PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_ImageSet.hpp
    I_ImageSetService.hpp
    I_SchemeService.hpp
    I_WidgetService.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZCrazyEddie
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
