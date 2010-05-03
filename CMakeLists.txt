CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(TicTacToe)

# verzia projektu
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)
set(MAINTENANCE_VERSION 0)

# cesta k vyslednemu suboru
set(FINAL_PATH "./bin")

# nazov vysledneho suboru
set(FINAL_EXE "${FINAL_PATH}/TicTacToe")

# najdenie potrebnych kniznic a pod.
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

# sprava build modiv - defaultny je release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# kde sa maju hladat hlavicky
include_directories("include/" "${CMAKE_CURRENT_BINARY_DIR}/include/")

# nastavenie suborov na spracovanie
set(SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/player.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/mainwindow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/newgamedlg.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/playboard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/playercreator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/playersign.cpp
)

set(MOC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/include/main.h ${CMAKE_CURRENT_SOURCE_DIR}/include/kernel/game.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kernel/player.h ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/mainwindow.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/newgamedlg.h ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/playboard.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/playercreator.h ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/playersign.h
)

set(TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/tictactoe_sk.ts)
# QM_FILES - automaticky generovane z TS_FILES
set(ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/arts/TicTacToe.png)
set(ARTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/arts/icon.png)
set(DESKTOP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/desktop/TicTacToe.desktop)
set(RC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/desktop/win32_res.rc)

# prilinkovanie .rc suboru - kvoli ikone
if(WIN32)
	set(SRC_FILES ${SRC_FILES} ${RC_FILE})
endif(WIN32)

# sprovozdnenie uninstall targetu
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# pridanie targetu pre generovanie .ts suborov
add_custom_target(generatets "${QT_LUPDATE_EXECUTABLE}" ${SRC_FILES} ${MOC_FILES} -ts ${TS_FILES})

# pridanie tagetu pre vytvorenie .qm suborov z .ts suborov
add_custom_target(generateqm)
foreach(ts ${TS_FILES})
	string(REPLACE ".ts" ".qm" qm ${ts})
	string(REPLACE "/" "_" qmTarget ${qm})
	set(qmTarget generate_${qmTarget})
	add_custom_target(${qmTarget} "${QT_LRELEASE_EXECUTABLE}" ${ts} -qm ${qm})
	add_dependencies(generateqm ${qmTarget})
endforeach(ts)

# target pre vytvorenie priecunku v ktorom sa vytvori execko
add_custom_target(make_target_dir "${CMAKE_COMMAND}" -E make_directory "${FINAL_PATH}")

# vytvorenie konfiguracneho include suboru pre projekt
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in"
			   "${CMAKE_CURRENT_BINARY_DIR}/include/config.h")

# vytvori moc subory
qt_wrap_cpp(${FINAL_EXE} MOC_SRC_FILES ${MOC_FILES})

# vytvorenie execka
add_executable(${FINAL_EXE} WIN32 ${SRC_FILES} ${MOC_SRC_FILES})
target_link_libraries(${FINAL_EXE} ${QT_LIBRARIES})
add_dependencies(${FINAL_EXE} make_target_dir)

# instalacne prikazy - rozne na unixe a windowse!
if(UNIX)
	install(TARGETS ${FINAL_EXE} DESTINATION bin)
	install(DIRECTORY ./translations DESTINATION share/TicTacToe FILES_MATCHING PATTERN "*.qm")
	install(FILES ${ARTS_FILES} DESTINATION share/TicTacToe/arts)
	install(FILES ${ICON_FILE} DESTINATION share/pixmaps)
	install(FILES ${DESKTOP_FILE} DESTINATION share/applications)
else(NOT UNIX)
	install(TARGETS ${FINAL_EXE} DESTINATION ./)
	install(DIRECTORY ./translations/ DESTINATION ./translations FILES_MATCHING PATTERN "*.qm")
	install(FILES ${ARTS_FILES} DESTINATION ./arts)
endif(UNIX)

