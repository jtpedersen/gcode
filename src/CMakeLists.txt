#resources
# qt5_add_resources(
#   resources_added 
#   "../res/icons.qrc")

# add_library(
# )

add_executable(
  main
  main.cpp

  GCodeParser.h
  GCodeParser.cpp

  PathWindow.h
  PathWindow.cpp

  MainWindow.h
  MainWindow.cpp

  Sketcher.h
  Sketcher.cpp
  )

target_link_libraries(
  main
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::3DCore
  Qt5::3DExtras
  Qt5::3DRender
  Qt5::3DInput
  )


add_executable(
  tester
  test.cpp

  GCodeParser.h
  GCodeParser.cpp

  )

target_link_libraries(
  tester
  Qt5::Core
  Qt5::Gui
)
