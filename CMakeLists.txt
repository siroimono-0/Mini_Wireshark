cmake_minimum_required(VERSION 3.16)

project(Mini_Wireshark VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core Sql)

if(WIN32)
    set(NPCAP_ROOT "" CACHE PATH "Npcap SDK root directory")
    set(_NPCAP_SEARCH_ROOTS
        ${NPCAP_ROOT}
        $ENV{NPCAP_ROOT}
        $ENV{NPCAP_SDK}
        $ENV{NPCAP_SDK_ROOT}
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/npcap-sdk-1.13"
        "C:/NpcapSDK"
        "C:/Npcap SDK"
        "C:/Program Files/Npcap SDK"
    )

    find_path(PCAP_INCLUDE_DIR
        NAMES pcap.h pcap/pcap.h
        HINTS ${_NPCAP_SEARCH_ROOTS}
        PATH_SUFFIXES Include include
    )

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_NPCAP_LIB_SUFFIXES Lib/x64 lib/x64 Lib lib)
    else()
        set(_NPCAP_LIB_SUFFIXES Lib lib Lib/x86 lib/x86)
    endif()

    find_library(PCAP_WPCAP_LIBRARY
        NAMES wpcap
        HINTS ${_NPCAP_SEARCH_ROOTS}
        PATH_SUFFIXES ${_NPCAP_LIB_SUFFIXES}
    )
    find_library(PCAP_PACKET_LIBRARY
        NAMES Packet packet
        HINTS ${_NPCAP_SEARCH_ROOTS}
        PATH_SUFFIXES ${_NPCAP_LIB_SUFFIXES}
    )

    if(NOT PCAP_INCLUDE_DIR OR NOT PCAP_WPCAP_LIBRARY OR NOT PCAP_PACKET_LIBRARY)
        message(FATAL_ERROR
            "Npcap SDK was not found. Install the Npcap SDK and configure "
            "-DNPCAP_ROOT=C:/path/to/npcap-sdk, or set NPCAP_ROOT/NPCAP_SDK_ROOT."
        )
    endif()

    set(PCAP_INCLUDE_DIRS ${PCAP_INCLUDE_DIR})
    set(PCAP_LIBRARIES ${PCAP_WPCAP_LIBRARY} ${PCAP_PACKET_LIBRARY} ws2_32)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PCAP REQUIRED libpcap)
endif()

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appMini_Wireshark
    main.cpp
)

qt_add_qml_module(appMini_Wireshark
    # Qt 6.9 qmlimportscanner mangles non-ASCII source paths on Windows.
    # This application uses a dynamic Qt build, so static plugin scanning is unnecessary.
    NO_IMPORT_SCAN
    URI Mini_Wireshark
    VERSION 1.0
    QML_FILES
        Main.qml
        NicListView.qml
        CaptureView.qml
        PktView.qml
        TEST.qml
        Qml_Module/Rec_icon.qml

    SOURCES
        NicList.h
        NicList.cpp
        Support.h
        Capture.h
        Capture.cpp
        Pcap.h
        Pcap.cpp
        PlatformCompat.h
        Struct_in.h
        TreeItem.h
        TreeItem.cpp
        TreeModel.h
        TreeModel.cpp
        Open_List.h Open_List.cpp

    RESOURCES
        img/mini.png
        img/mini_gray.png
        img/back.png
        img/stop.png
        img/stop_gray.png
        img/reset.png
        img/save.png
        img/save_gray.png
        img/pkt_back.png
        img/wireshark_it_security_neon_white.svg
        img/start_.svg
        img/start_gray.svg
        img/stop_new.svg
        img/stop_new_gray.svg
        img/back_new.svg
        img/reset_new.svg
        img/save_new.svg
        img/save_new_gray.svg

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
target_link_libraries(appMini_Wireshark
    PRIVATE
        Qt6::Core
        Qt6::Sql
        ${PCAP_LIBRARIES}
)
target_include_directories(appMini_Wireshark PRIVATE ${PCAP_INCLUDE_DIRS})

if(MSVC)
    target_compile_definitions(appMini_Wireshark PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

include(GNUInstallDirs)
install(TARGETS appMini_Wireshark
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
