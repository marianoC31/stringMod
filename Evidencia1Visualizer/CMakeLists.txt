cmake_minimum_required(VERSION 3.19)
project(Evidencia1Visualizer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Evidencia1Visualizer
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    mcode1.txt mcode2.txt  transmission1.txt
    transmission2.txt
    mcode3.txt
    style.qss
)
configure_file(transmission1.txt transmission1.txt COPYONLY)
configure_file(transmission2.txt transmission2.txt COPYONLY)
configure_file(mcode1.txt mcode1.txt COPYONLY)
configure_file(mcode2.txt mcode2.txt COPYONLY)
configure_file(mcode3.txt mcode3.txt COPYONLY)
configure_file(style.qss style.qss COPYONLY)

add_custom_command(TARGET Evidencia1Visualizer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/style.qss
            $<TARGET_FILE_DIR:Evidencia1Visualizer>/style.qss
)
target_link_libraries(Evidencia1Visualizer
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS Evidencia1Visualizer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Evidencia1Visualizer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
