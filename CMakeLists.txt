#--------------------------------------------------------------------------------
# This is an example project that uses CMake as a build system to compile a Qt5.x
# based project. 
# To have CMake pick a Qt installation of your choice that won't be found
# automatically, set the CMAKE_PREFIX_PATH environment variable.
# For example: "export CMAKE_PREFIX_PATH=/usr/local/trolltech/qt4.3.5"
#  

PROJECT (HelloCMakeQt)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++0x")
message("CMAKE_CURRENT_BINARY_DIR " ${CMAKE_CURRENT_BINARY_DIR})
message("")

#--------------------------------------------------------------------------------
# This is the list of source files that need to be compiled
#--------------------------------------------------------------------------------
SET( HELLO_PROJ_SRCS
  main.cpp
  src/qtlogo.cpp
  src/glview.cpp
  src/mainwindow.cpp
  src/application.cpp
)

#--------------------------------------------------------------------------------
# another list, this time it includes all header files that should be treated with moc
#--------------------------------------------------------------------------------
SET( HELLO_PROJ_MOC_HDRS
  src/glview.h
  src/mainwindow.h
  src/application.h
)

#--------------------------------------------------------------------------------
# .ui files that are used in the project
#--------------------------------------------------------------------------------
SET( HELLO_PROJ_UIS
  ui/MainWindow.ui
)

#--------------------------------------------------------------------------------
# and finally any resource files. I do not have any in this example
#--------------------------------------------------------------------------------
SET( HELLO_PROJ_RCS

)

#--------------------------------------------------------------------------------
# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the 
#  IF(WIN32) conditional.
IF (WIN32)
    SET (QT_USE_QTMAIN TRUE)
ENDIF (WIN32)

#--------------------------------------------------------------------------------
# this command finds Qt5 libraries and sets all required variables
# note that it's Qt5, not QT5 or qt5
set(QT5_DIR "/Users/jennal/qt/5.3")
set(QT5_INCLUDE_DIR "${QT5_DIR}/Src/qtbase/include")
set(QT5_CMAKE_DIR "${QT5_DIR}/clang_64/lib/cmake")
set(CMAKE_PREFIX_PATH ${QT5_CMAKE_DIR})

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)

#--------------------------------------------------------------------------------
# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt5 ) that contains a path to
# a CMake script that sets up this directory for building with Qt5
set(QT_INCLUDE_DIRS 
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5OpenGL_INCLUDE_DIRS}
)

add_definitions(
  ${Qt5Core_DEFINITIONS}
  ${Qt5Widgets_DEFINITIONS}
  ${Qt5Gui_DEFINITIONS}
  ${Qt5OpenGL_DEFINITIONS}
)

set(QT_LIBRARIES
  Qt5::Core
  Qt5::Widgets
  Qt5::Gui
  Qt5::OpenGL
)

#--------------------------------------------------------------------------------
# this command will generate rules that will run rcc on all files from HELLO_PROJ_RCS
# in result HELLO_PROJ_GEN_RCS variable will contain paths to files produced by rcc
QT5_ADD_RESOURCES( HELLO_PROJ_GEN_RCS ${HELLO_PROJ_RCS} )

#--------------------------------------------------------------------------------
# this will run uic on .ui files:
QT5_WRAP_UI( HELLO_PROJ_GEN_UIS ${HELLO_PROJ_UIS} )

#--------------------------------------------------------------------------------
# and finally this will run moc on the headers
QT5_WRAP_CPP( HELLO_PROJ_GEN_MOC_HDRS ${HELLO_PROJ_MOC_HDRS} )

#--------------------------------------------------------------------------------
# we need this to be able to include headers produced by uic in our code
# CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES()
#  works just like INCLUDEPATH from qmake
INCLUDE_DIRECTORIES (
  # Look in the cmake build directory (some generated headers could be there)
  ${CMAKE_CURRENT_BINARY_DIR}
  ${QT_INCLUDE_DIRS}
  "src"
)

#--------------------------------------------------------------------------------
# For Apple set the icns file containing icons
IF(APPLE)
  # # set how it shows up in the Info.plist file
  # SET(MACOSX_BUNDLE_ICON_FILE HelloCMakeQt.icns) 
  # # set where in the bundle to put the icns file
  # SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/HelloCMakeQt.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  # # include the icns file in the target
  # SET(HELLO_PROJ_SRCS ${HELLO_PROJ_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/HelloCMakeQt.icns)
ENDIF(APPLE)

#--------------------------------------------------------------------------------
#  CMake's way of creating an executable
ADD_EXECUTABLE( HelloCMakeQt MACOSX_BUNDLE WIN32
  ${HELLO_PROJ_SRCS}
  ${HELLO_PROJ_GEN_MOC_HDRS} 
  ${HELLO_PROJ_GEN_UIS}
  ${HELLO_PROJ_GEN_RCS}
)

if(APPLE)
  find_library(LIB_COCOA Cocoa)
  find_library(LIB_FOUNDATION Foundation)
  find_library(LIB_OPENGL OpenGL)
  find_library(LIB_AGL AGL)

  set(APPLE_LIBS
    ${LIB_COCOA}
    ${LIB_FOUNDATION}
    ${LIB_OPENGL}
    ${LIB_AGL}
  )
endif(APPLE)

#--------------------------------------------------------------------------------
# Tell CMake which libraries we need to link our executable against.
TARGET_LINK_LIBRARIES ( HelloCMakeQt
  ${QT_LIBRARIES}
  ${APPLE_LIBS}
)


#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
SET(plugin_dest_dir bin)
SET(qtconf_dest_dir bin)
SET(APPS "\${CMAKE_CURRENT_BINARY_DIR}/bin/HelloCMakeQt")
IF(APPLE)
  SET(plugin_dest_dir HelloCMakeQt.app/Contents/MacOS)
  SET(qtconf_dest_dir HelloCMakeQt.app/Contents/Resources)
  SET(APPS "\${CMAKE_CURRENT_BINARY_DIR}/HelloCMakeQt.app")
ENDIF(APPLE)
IF(WIN32)
  SET(APPS "\${CMAKE_CURRENT_BINARY_DIR}/bin/HelloCMakeQt.exe")
ENDIF(WIN32)

#--------------------------------------------------------------------------------
# Install the HelloCMakeQt application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
INSTALL(TARGETS HelloCMakeQt 
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${plugin_dest_dir}/plugins COMPONENT Runtime)

#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
INSTALL(CODE "
    file(WRITE \"\${CMAKE_CURRENT_BINARY_DIR}/${qtconf_dest_dir}/qt.conf\" \"\")
    " COMPONENT Runtime)


#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# directories to look for dependencies
SET(DIRS ${QT_LIBRARY_DIRS})

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.
INSTALL(CODE "
    file(GLOB_RECURSE QTPLUGINS
      \"\${CMAKE_CURRENT_BINARY_DIR}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
    " COMPONENT Runtime)


# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well
set(CPACK_BINARY_DRAGNDROP ON)
include(CPack)

