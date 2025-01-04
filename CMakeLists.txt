cmake_minimum_required(VERSION 3.16)

project(QR-Scanner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(CPM)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.6 QUIET REQUIRED
	COMPONENTS
		Core
		Qml
		Quick
		Multimedia
)

CPMAddPackage(
	NAME             ZXing
	GIT_REPOSITORY   git@github.com:zxing-cpp/zxing-cpp.git
	GIT_TAG          v2.2.1
)

qt_add_executable(QR-Scanner)

qt_add_qml_module(QR-Scanner
	URI QRScanner
	VERSION 1.0
	RESOURCE_PREFIX /
	QML_FILES
		main.qml
	SOURCES
		include/QrScanner.h
		include/FrameProcessor.h
		include/QrCodeWorker.h
		src/QrScanner.cpp
		src/FrameProcessor.cpp
		src/QrCodeWorker.cpp
		src/main.cpp
)

target_include_directories(QR-Scanner
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)


set(QT_LIBS
	Qt6::Quick
	Qt6::Multimedia
)

target_link_libraries(QR-Scanner
	PRIVATE
		${QT_LIBS}
		ZXing::ZXing
)
