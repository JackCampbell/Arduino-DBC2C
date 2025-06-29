cmake_minimum_required(VERSION 3.2)
project(Arduino-DBC2C
	VERSION 1.0
	DESCRIPTION "This application reads DBC files and generates code for CAN communication on the Arduino Feather M4 board.")

find_package(wxWidgets REQUIRED COMPONENTS core base adv stc)

set(SOURCE_FILES
	MarineApp.cpp
	Lexer.cpp
	DBCFile.cpp
	DBCObject.cpp
	AttributePanel.cpp
	DefinePanel.cpp
	ValTablePanel.cpp
	NodePanel.cpp
	DBCBitMap.cpp
	MessagePanel.cpp
	DBCFileDialog.cpp
	GenSignal.cpp
	Utils.cpp
	SignalSelector.cpp
	MarineAppDev.cpp
	FileSelectDialog.cpp
	stubs/Resource.cpp)

## Target
add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE WIN32 ${SOURCE_FILES})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF)
# Platform support
if(APPLE)
	set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/app_icon.icns")
	set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/dist/MacOSXBundleInfo.plist.in"
		MACOSX_BUNDLE_GUI_IDENTIFIER "jack.campbell.Arduino-DBC2C"
		MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
		MACOSX_BUNDLE_DISPLAY_NAME "${PROJECT_NAME}"
		MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} - ${APP_COPYRIGHT}"
		MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} ${PROJECT_VERSION}"
		MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
		MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
		MACOSX_BUNDLE_ICON_FILE "app_icon.icns"
		MACOSX_BUNDLE_COPYRIGHT "${APP_DESCRIPTION}")
elseif(WIN32)
	set(APP_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/dist/resource.rc")
elseif(UNIX)
	set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/app_icon.svg")
	set(APP_INFO "${CMAKE_CURRENT_SOURCE_DIR}/dist/Arduino-DBC2C.desktop")
endif()
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${APP_ICON} ${APP_INFO} ${APP_RESOURCE})

# Build
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE ${wxWidgets_DEFINITIONS})
if(UNIX)
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wno-deprecated -Wno-nonportable-include-path -Wno-delete-incomplete)
elseif(MSVC)
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /D_CRT_SECURE_NO_WARNINGS /wd4996)
endif()
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARY_DIRS})
if(MSVC)
	target_link_options(${CMAKE_PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS)
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} ${wxWidgets_LIBRARIES})
source_group("source" FILES ${SOURCE_FILES})

## install
if(APPLE)
	install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION "/Applications")
elseif(UNIX)
	install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION "/usr/local/bin")
	install(FILES ${APP_ICON} DESTINATION "/usr/local/share/Arduino-DBC2C")
	install(FILES ${APP_INFO} DESTINATION "/usr/share/applications")
endif()

