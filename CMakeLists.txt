#TODO: Remove these 5 lines later when integrated into playground or kdeartwork
project(kwaterscreensaver)
find_package(KDE4 REQUIRED)
find_package(Strigi REQUIRED)
include_directories(${KDE4_INCLUDES})
include(KDE4Defaults)
#####################

set(kwatergl.kss_SRCS kwatergl.cpp)

kde4_add_executable(kwatergl.kss ${kwatergl.kss_SRCS})

target_link_libraries(kwatergl.kss ${KDE4_KDEUI_LIBS} ${STRIGI_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${OPENGL_GLU_LIBRARY} kscreensaver)

install(TARGETS kwatergl.kss ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kwatergl.desktop DESTINATION ${SERVICES_INSTALL_DIR}/ScreenSavers)
