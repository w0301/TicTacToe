HEADERS += ../../include/main.h \
    ../../include/ui/mainwindow.h \
    ../../include/ui/playboard.h \
    ../../include/kernel/player.h \
    ../../include/kernel/game.h

SOURCES += ../../src/main.cpp \
    ../../src/ui/mainwindow.cpp \
    ../../src/ui/playboard.cpp \
    ../../src/kernel/player.cpp \
    ../../src/kernel/game.cpp

DESTDIR = ../../bin
OBJECTS_DIR = ../../obj
INCLUDEPATH += ../../include
CODECFORTR = UTF-8
TRANSLATIONS = ../../translations/tictactoe_sk.ts
