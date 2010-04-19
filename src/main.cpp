/*!
*   This program is free software; you can redistribute it and/or
*   modify it under the terms of the GNU General Public License as
*   published by the Free Software Foundation; either version 3 of the
*   License, or (at your option) any later version.

*   This program is distributed in the hope that it will be useful, but
*   WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*   General Public License for more details:
*   http://www.gnu.org/licenses/gpl.txt
*/

#include "main.h"
#include "ui/mainwindow.h"

#include <QLocale>
#include <QTranslator>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // nastavenie translatoru pre program - TODO: opravit tak aby to fungovala aj po instalacii na linux
    QTranslator translator;
    QString trFileName = QString("tictactoe_") + QLocale::system().name();

    // pokusime sa loadnut subor s prekladom
    if( !translator.load(trFileName, "./translations" ) ) {
        if( !translator.load(trFileName, "/usr/share/TicTacToe/translations" ) ) {
            if( !translator.load(trFileName, "/usr/local/share/TicTacToe/translations" ) ) {
                translator.load(trFileName, "../translations" );
            }
        }
    }
    app.installTranslator(&translator);

    MainWindow window;
    window.show();

    return app.exec();
}
