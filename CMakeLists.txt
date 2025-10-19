cmake_minimum_required(VERSION 3.19)
project(proyectoUno-EstruII LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(proyectoUno-EstruII
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    escenario.h escenario.cpp
    jugador.h jugador.cpp
    nivel2ruleta.h nivel2ruleta.cpp nivel2ruleta.ui
    mapa.h mapa.cpp mapa.ui
)

target_link_libraries(proyectoUno-EstruII
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS proyectoUno-EstruII
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET proyectoUno-EstruII
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
