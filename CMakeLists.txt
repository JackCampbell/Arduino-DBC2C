cmake_minimum_required(VERSION 3.2)
project(MarineFeather)

find_package(wxWidgets REQUIRED COMPONENTS core base adv stc)

add_executable(${CMAKE_PROJECT_NAME}
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
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE DEBUG ${wxWidgets_DEFINITIONS})
if(UNIX)
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wno-deprecated -Wno-nonportable-include-path)
endif()
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARY_DIRS})
if(MSVC)
	target_link_options(${CMAKE_PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS)
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} ${wxWidgets_LIBRARIES})

