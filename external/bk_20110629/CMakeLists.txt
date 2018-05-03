set(FB_SRCS
    hanna/Bitmap.cpp
    hanna/BitmapArray.cpp
    hanna/BitmapFont.cpp
    hanna/Debug.cpp
    hanna/App.cpp
    hanna/Stage.cpp
    hanna/Game.cpp
    hanna/Prop.cpp
    hanna/BitmapN.cpp
    hanna/PropN.cpp
    hanna/PropRotation.cpp	
    hanna/Sample.cpp	
    hanna/PropBase.cpp	
    hanna/PropFont.cpp	
    hanna/Widget.cpp	
    hanna/PropWidget.cpp	
    hanna/Theme.cpp
    FBApp.cpp
    FBStage.cpp
    FBResource.cpp
    main.cpp
    FBBubble.cpp
    FBLevels.cpp
    Compressor.cpp
    EntryStage.cpp
    )

set_source_files_properties(${FB_SRCS} PROPERTIES LANGUAGE "CXX")

file(GLOB_RECURSE FB_RES data/res/*.*)

include_directories(
    include
    ${CMAKE_SOURCE_DIR}/addons/main
    ${CMAKE_SOURCE_DIR}/addons/image
    ${CMAKE_SOURCE_DIR}/addons/font
    ${CMAKE_SOURCE_DIR}/addons/audio
    ${CMAKE_SOURCE_DIR}/addons/acodec
    ${CMAKE_SOURCE_DIR}/addons/color
    ${CMAKE_SOURCE_DIR}/addons/memfile
    ${CMAKE_SOURCE_DIR}/addons/native_dialog
    ${CMAKE_SOURCE_DIR}/addons/physfs
    ${CMAKE_SOURCE_DIR}/addons/primitives
    ${CMAKE_SOURCE_DIR}/addons/ttf
    )

if(APPLE)
    set(DEMO_EXECUTABLE_TYPE MACOSX_BUNDLE)
else(APPLE)
    set(DEMO_EXECUTABLE_TYPE "${EXECUTABLE_TYPE}")
endif(APPLE)

if(SUPPORT_FONT AND SUPPORT_AUDIO AND SUPPORT_ACODEC)
    add_executable(frozenbubble
        ${DEMO_EXECUTABLE_TYPE}
        ${FB_SRCS}
		${FB_RES}
        )
    
    fix_executable(frozenbubble)

    target_link_libraries(frozenbubble
        ${ALLEGRO_MAIN_LINK_WITH}
        ${FONT_LINK_WITH}
        ${IMAGE_LINK_WITH}
        ${AUDIO_LINK_WITH}
        ${ACODEC_LINK_WITH}
		${COLOR_LINK_WITH}
		${MEMFILE_LINK_WITH}
		${PRIMITIVES_LINK_WITH}
		${TTF_LINK_WITH}
        )

    if(NOT BUILD_SHARED_LIBS) 
        set_target_properties(frozenbubble PROPERTIES COMPILE_FLAGS "-DALLEGRO_STATICLINK")
    endif(NOT BUILD_SHARED_LIBS)

    # Mac OS X bundle support.
    set_target_properties(frozenbubble PROPERTIES
	MACOSX_BUNDLE_COPYRIGHT "Copyright 2011 Jhy Developers"
	MACOSX_BUNDLE_ICON_FILE "Icon.icns"
	MACOSX_BUNDLE_INFO_STRING "0.1, Copyright 2011 Jhy Developers"
	MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
	MACOSX_BUNDLE_LONG_VERSION_STRING "FrozenBubble v0.1"
	MACOSX_BUNDLE_GUI_IDENTIFIER "jhy.frozenbubble"
	)
    set_source_files_properties(${FB_RES} PROPERTIES
	MACOSX_PACKAGE_LOCATION "Resources/data/res"
	)

    set_source_files_properties(${DEMO_ICON} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
	)

else()
    message(STATUS "Not building frozenbubble")
endif()

copy_data_dir_to_build(copy_fb_data data)

#-----------------------------------------------------------------------------#
# vi: set ts=8 sts=4 sw=4 et:
