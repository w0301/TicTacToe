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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Game;
class PlayBoard;
class QLineEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

    public slots:
        void updateTime(int);

    private:
        Game *m_game;
        PlayBoard *m_playBoard;

        QLineEdit *edit;
};

#endif // MAINWINDOW_H
