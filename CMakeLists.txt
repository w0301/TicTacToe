CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(TicTacToe)

# verzia projektu
set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(MAINTENANCE_VERSION 0)

# nazov vysledneho suboru
set(FINAL_EXE "bin/TicTacToe")

# najdenie potrebnych kniznic a pod.
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

# kde sa maju hladat hlavicky
include_directories("include/")

# nastavenie suborov na spracovanie
set(SRC_FILES src/main.cpp src/kernel/game.cpp src/kernel/player.cpp src/ui/mainwindow.cpp 
			  src/ui/newgamedlg.cpp src/ui/playboard.cpp src/ui/playercreator.cpp src/ui/playersign.cpp)

set(MOC_FILES include/main.h include/kernel/game.h include/kernel/player.h include/ui/mainwindow.h 
			  include/ui/newgamedlg.h include/ui/playboard.h include/ui/playercreator.h include/ui/playersign.h)

set(TS_FILES translations/tictactoe_sk.ts)
set(QM_FILES translations/tictactoe_sk.qm)
set(ARTS_FILES arts/icon.png)

# sprovozdnenie uninstall targetu
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# pridanie targetu pre generovanie .ts suborov
add_custom_target(generatets "${QT_LUPDATE_EXECUTABLE}" ${SRC_FILES} ${MOC_FILES} -ts ${TS_FILES})

# pridanie tagetu pre vytvorenie .qm suborov z .ts suborov
add_custom_target(generateqm "${QT_LRELEASE_EXECUTABLE}" ${TS_FILES} -qm ${QM_FILES})

# vytvorenie konfiguracneho include suboru pre projekt
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in"
			   "${CMAKE_CURRENT_SOURCE_DIR}/include/config.h")



# vytvori moc subory
qt_wrap_cpp(${FINAL_EXE} MOC_SRC_FILES ${MOC_FILES})

# vytvorenie execka
add_executable(${FINAL_EXE} ${SRC_FILES} ${MOC_SRC_FILES})
target_link_libraries(${FINAL_EXE} ${QT_LIBRARIES})

# instalacne prikazy - execko a subory jazyka
install(TARGETS ${FINAL_EXE} DESTINATION bin)
install(FILES ${QM_FILES} DESTINATION share/TicTacToe/translations)
install(FILES ${ARTS_FILES} DESTINATION share/TicTacToe/arts)

