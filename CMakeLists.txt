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
    video.h video.cpp video.ui
    nivel1video.h nivel1video.cpp nivel1video.ui
    nivel3batalla.h nivel3batalla.cpp nivel3batalla.ui
    nivel4casa.h nivel4casa.cpp nivel4casa.ui
    cuarto.h cuarto.cpp cuarto.ui
    laboratorio.h laboratorio.cpp laboratorio.ui
    juego1.h juego1.cpp juego1.ui
    juego2.h juego2.cpp juego2.ui
    juego3.h juego3.cpp juego3.ui
    juego4.h juego4.cpp juego4.ui
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
