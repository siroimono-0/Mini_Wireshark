cmake_minimum_required(VERSION 3.16)

project(Mini_Wireshark VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appMini_Wireshark
    main.cpp
)

qt_add_qml_module(appMini_Wireshark
    URI Mini_Wireshark
    VERSION 1.0
    QML_FILES
        Main.qml
        NicListView.qml
        CaptureView.qml

    SOURCES
        NicList.h
        NicList.cpp
        Support.h
        Capture.h
        Capture.cpp
        Pcap.h
        Pcap.cpp
        Struct_in.h

    RESOURCES
        img/mini.png
        img/back.png
        img/stop.png
        img/reset.png

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMini_Wireshark PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMini_Wireshark
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMini_Wireshark
    PRIVATE Qt6::Quick
)
target_link_libraries(appMini_Wireshark PRIVATE Qt6::Core)
target_link_libraries(appMini_Wireshark
    PRIVATE
        ${PCAP_LIBRARIES}
)
target_link_libraries(appMini_Wireshark PRIVATE Qt6::Core)
target_link_libraries(appMini_Wireshark PRIVATE Qt6::Core)
include_directories(${PCAP_INCLUDE_DIRS})

include(GNUInstallDirs)
install(TARGETS appMini_Wireshark
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
