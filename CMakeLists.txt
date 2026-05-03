cmake_minimum_required(VERSION 3.19)
project(milandr_client LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(milandr_client
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainclient.cpp
    mainclient.h
    mainclient.ui
    connection.h connection.cpp
    GroupData.h
)

target_link_libraries(milandr_client
    PRIVATE
        Qt::Core
        Qt::Widgets
)

target_include_directories(milandr_client
    PRIVATE
        ${BOOST_INCLUDE_DIR}
)
target_compile_definitions(milandr_client
    PRIVATE
    BOOST_BEAST_USE_STD_ERIC=1
    BOOST_ASIO_USE_STD_ERIC=1
)

include(GNUInstallDirs)

install(TARGETS milandr_client
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET milandr_client
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
