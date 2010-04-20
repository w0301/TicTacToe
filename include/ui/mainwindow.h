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

#include <QFrame>
#include <QLCDNumber>
#include <QMainWindow>

class Game;
class Player;
class PlayBoard;
class NewGameDialog;

class QLabel;
class QCheckBox;
class QListWidget;
class QDockWidget;

// TimeLimitFrame class
class TimeLimitFrame : public QLCDNumber {
    Q_OBJECT

    public:
        TimeLimitFrame(QWidget*, int = 0);

    public slots:
        // videli 1000 a zobrazi ako des. cislo
        void showTimeLimit(int);

        // nastavi cas na 0
        void resetTimeLimit();
};

// PlayerSignFrame class
class PlayerSignFrame : public QFrame {
    Q_OBJECT

    public:
        PlayerSignFrame(QWidget*);

        // reimplementacia paint eventu
        void paintEvent(QPaintEvent*);

        // vrati hraca
        Player* player() {
            return m_actuPlayer;
        };
        const Player* player() const {
            return m_actuPlayer;
        };

    public slots:
        void setPlayer(Player*);

    private:
        Player *m_actuPlayer;
};

// PlayerListFrame class
class PlayerListFrame : public QFrame {
    Q_OBJECT

    public:
        PlayerListFrame(QWidget*);

    public slots:
        void setGame(Game*);
        void unsetGame();

        void fillList();
        void setActualPlayer(Player*);

    private:
        // hra z ktorej sa zobrazuju hraci
        Game *m_game;

        // zobrazuje meno aktualneho hraca
        QLabel *m_actuPlayerName;

        // zobrazi podpis hraca
        PlayerSignFrame *m_actuPlayerSign;

        // zobrazuje zoznam vsetkych hracov
        QListWidget *m_playerList;
};

// MainWindow class
class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

    public slots:
        void setGame(Game*);
        void unsetGame();
        void startNewGameDialog();
        void startWonWindow(Player*);
        void startAboutWindow();

    private:
        // hra pre okno
        Game *m_game;

        /// widgety
        // hracia plocha pre hru
        PlayBoard *m_playBoard;

        // zobrazovac casoveho limitu
        TimeLimitFrame *m_timeLimitFrame;

        // zobrazovac hracov
        PlayerListFrame *m_playerListFrame;

        // checkbox na pauzu
        QCheckBox *m_pauseCheckBox;

        // docky okna - zatial jeden mozno bude aj druhy
        QDockWidget *m_leftDock;
};

#endif // MAINWINDOW_H
