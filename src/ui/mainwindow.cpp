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
#include "ui/playboard.h"

#include <QtGui>

MainWindow::MainWindow() : QMainWindow(NULL) {
    // nastavenie defaultnych hodnot
    setWindowTitle(tr("TicTacToe"));
    setMinimumSize(405, 405);

    // vytvorenie hracej plochy
    m_board = new PlayBoard(this, DEFAULT_BOARD_SIZE);
    setCentralWidget(m_board);

}

