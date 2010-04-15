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
#include "kernel/game.h"
#include "kernel/player.h"

#include "ui/playersign.h"
#include "ui/newgamedlg.h"
#include "ui/mainwindow.h"
#include "ui/playboard.h"

#include <QtGui>

// TimeLimitFrame class
TimeLimitFrame::TimeLimitFrame(QWidget *parent, int num) : QLCDNumber(parent) {
    display(num);
    setToolTip(tr("Time limit of move"));
}

void TimeLimitFrame::showTimeLimit(int num) {
    display( double(num) / 1000.0 );
}

void TimeLimitFrame::resetTimeLimit() {
    display(0);
}

// PlayerSignFrame class
PlayerSignFrame::PlayerSignFrame(QWidget *parent) :
        QFrame(parent), m_actuPlayer(NULL) {
    setMinimumSize(16, 16);
    setMaximumSize(16, 16);
    setToolTip(tr("Sign of player"));
}

// nastavi hraca, ktoreho podpis sa bude vykreslovat
void PlayerSignFrame::setPlayer(Player *pl) {
    m_actuPlayer = pl;
    repaint();
}

// vykresli podpis hraca
void PlayerSignFrame::paintEvent(QPaintEvent*) {
    if(player() == NULL) {
        return;
    }
    QPainter painter(this);
    painter.drawPixmap(0, 0, player()->sign()->signPixmap(this, QPoint(0, 0), 16));
}

// PlayerListFrame class
PlayerListFrame::PlayerListFrame(QWidget *parent) :
        QFrame(parent), m_game(NULL), m_actuPlayerName(NULL), m_playerList(NULL) {
    // pridanie widgetu s menom
    m_actuPlayerName = new QLabel(this);

    // pridanie widgetu s podpisom
    m_actuPlayerSign = new PlayerSignFrame(this);

    // pridanie widgetu listu hracov
    m_playerList = new QListWidget(this);
    m_playerList->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

    // prve dve do horizontalneho layoutu
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_actuPlayerName);
    hLayout->addWidget(m_actuPlayerSign);

    // a potom do vertikalneho layoutu
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(m_playerList);
}

// naplni list menami hracov
void PlayerListFrame::fillList() {
    if(m_game == NULL) {
        return;
    }
    m_playerList->clear();
    foreach(Player *i, m_game->players()) {
        m_playerList->addItem(i->name());
    }
}

// nastavi label aktualneho hraca
void PlayerListFrame::setActualPlayer(Player *pl) {
    if(m_actuPlayerName != NULL) {
        m_actuPlayerName->setText(pl->name());
        m_actuPlayerName->setToolTip(tr("Actually moving"));
    }
    if(m_actuPlayerSign != NULL) {
        m_actuPlayerSign->setPlayer(pl);
    }
}

// nastavi hru s ktorov bude widget pracovat
void PlayerListFrame::setGame(Game *game) {
    unsetGame();
    m_game = game;
    if(m_game != NULL) {
        // pri zacati hry sa naplni list
        connect(m_game, SIGNAL(gameStarted(Player*)), this, SLOT(fillList()));

        // ked sa zmeni hrac nastavime label
        connect(m_game, SIGNAL(playerChanged(Player*)), this, SLOT(setActualPlayer(Player*)));

        // ked sa hra skonci musime vicistit list a label
        connect(m_game, SIGNAL(gameStopped()), m_playerList, SLOT(clear()));
        connect(m_game, SIGNAL(gameStopped()), m_actuPlayerName, SLOT(clear()));
    }
}

void PlayerListFrame::unsetGame() {
    if(m_game != NULL) {
        disconnect(m_game, SIGNAL(gameStarted(Player*)), this, SLOT(fillList()));
        disconnect(m_game, SIGNAL(playerChanged(Player*)), this, SLOT(setActualPlayer(Player*)));
        disconnect(m_game, SIGNAL(gameStopped()), m_playerList, SLOT(clear()));
        disconnect(m_game, SIGNAL(gameStopped()), m_actuPlayerName, SLOT(clear()));

        m_game = NULL;
        m_playerList->clear();
    }
}

// MainWindow class
MainWindow::MainWindow() :
        QMainWindow(NULL), m_game(NULL), m_playBoard(NULL), m_timeLimitFrame(NULL),
        m_leftDock(NULL) {
    /// nastavenie defaultnych hodnot
    setWindowTitle( tr("TicTacToe") );
    setMinimumSize(505, 405);



    // vytvorenie menu
    QMenuBar *mainMenuBar = new QMenuBar(this);
    setMenuBar(mainMenuBar);

    // menu s polozkami pre ovladanie hry
    QMenu *gameMenu = new QMenu(tr("&Game"), mainMenuBar);
    QAction *newGameAction = gameMenu->addAction( tr("&New game...") );
    mainMenuBar->addMenu(gameMenu);
    connect(newGameAction, SIGNAL(triggered()), this, SLOT(startNewGameDialog()));
}

MainWindow::~MainWindow() {

}

// nastavi hru okna a pripoji k oknu potrebne signaly a zobrazi
// potrebne okna
void MainWindow::setGame(Game *game) {
    unsetGame();
    m_game = game;
    if(m_game != NULL) {
        m_game->setParent(this);

        /// vytvorenie hracej plochy
        m_playBoard = new PlayBoard(m_game, this);
        setCentralWidget(m_playBoard);

        /// vytvorenie laveho docku
        m_leftDock = new QDockWidget(this);
        m_leftDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        m_leftDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

        // pomocny widget a layout
        QWidget *leftDockWidgets = new QWidget(m_leftDock);
        QVBoxLayout *leftDockLayout = new QVBoxLayout(leftDockWidgets);

        /// vytvorenie zobrazovaca casu
        m_timeLimitFrame = new TimeLimitFrame(leftDockWidgets, 0);

        // signal pre update casoveho limitu
        connect(m_game, SIGNAL(gameStarted(Player*)), m_timeLimitFrame, SLOT(resetTimeLimit()));
        connect(m_game, SIGNAL(gameStopped()), m_timeLimitFrame, SLOT(resetTimeLimit()));
        connect(m_game, SIGNAL(timerUpdated(int)), m_timeLimitFrame, SLOT(showTimeLimit(int)));

        // vytvorenie listu hracov
        m_playerListFrame = new PlayerListFrame(leftDockWidgets);
        m_playerListFrame->setGame(m_game);

        /// vytvorenie check boxu na pauzu
        m_pauseCheckBox = new QCheckBox(tr("&Pause"), leftDockWidgets);

        // signali pre check box pauzi na docku
        connect(m_pauseCheckBox, SIGNAL(clicked(bool)), m_game, SLOT(pauseGame(bool)));
        connect(m_game, SIGNAL(gamePaused(bool)), m_pauseCheckBox, SLOT(setChecked(bool)));

        // pridanie do layoutu
        leftDockLayout->addWidget(m_timeLimitFrame);
        leftDockLayout->addWidget(m_playerListFrame);
        leftDockLayout->addWidget(m_pauseCheckBox);

        // nastavenie widgetu dockovacieho widgetu
        m_leftDock->setWidget(leftDockWidgets);

        // priradenie docku oknu
        addDockWidget(Qt::LeftDockWidgetArea, m_leftDock);

        // signali pre zistenie vyhercu
        connect(m_game, SIGNAL(playerWon(Player*)), this, SLOT(startWonWindow(Player*)));

        // signali pre ukoncenie
        connect(m_game, SIGNAL(gameStopped()), this, SLOT(unsetGame()));

        // spustime hru
        m_game->startGame();
    }
}

// zrusi aktulnu hru v okne
void MainWindow::unsetGame() {
    if(m_game != NULL) {
        delete m_playBoard;
        m_playBoard = NULL;

        delete m_pauseCheckBox;
        m_pauseCheckBox = NULL;

        delete m_playerListFrame;
        m_playerListFrame = NULL;

        delete m_timeLimitFrame;
        m_timeLimitFrame = NULL;

        delete m_leftDock;
        m_leftDock = NULL;

        // a nakoniec zmazanie samotnej hry
        delete m_game;
        m_game = NULL;
    }
}

// vytvori a spusti dialog novej hry
void MainWindow::startNewGameDialog() {
    // vytvorenie a spustenie dialogu
    NewGameDialog newGameDlg(this);
    connect(&newGameDlg, SIGNAL(newGameCreated(Game*)), this, SLOT(setGame(Game*)));
    newGameDlg.exec();
}

// otvory mess. box ktory informuje o tom, ze vyhral hrac pl
void MainWindow::startWonWindow(Player *pl) {
    QMessageBox winBox(tr("Player won."), tr("Player '") + pl->name() + tr("' won the game."),
                       QMessageBox::Information, QMessageBox::Ok, QMessageBox::Retry, QMessageBox::NoButton, this);
    connect(winBox.button(QMessageBox::Retry), SIGNAL(clicked()), m_game, SLOT(resetGame()));
    //connect(winBox.button(QMessageBox::Ok), SIGNAL(clicked()), this, SLOT(unsetGame()));
    winBox.exec();
}
