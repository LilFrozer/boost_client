cmake_minimum_required(VERSION 3.19)
project(boost_client LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(boost_client
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainclient.cpp
    mainclient.h
    mainclient.ui
    connection.h connection.cpp
    GroupData.h
)

target_link_libraries(boost_client
    PRIVATE
        Qt::Core
        Qt::Widgets
)

target_include_directories(boost_client
    PRIVATE
        ${BOOST_INCLUDE_DIR}
)
target_compile_definitions(boost_client
    PRIVATE
    BOOST_BEAST_USE_STD_ERIC=1
    BOOST_ASIO_USE_STD_ERIC=1
)

include(GNUInstallDirs)

install(TARGETS boost_client
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET boost_client
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
