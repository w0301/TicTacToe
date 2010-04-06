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


#include "kernel/game.h"
#include "kernel/player.h"
#include "ui/newgamedlg.h"

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>


// NewGameDialog class
NewGameDialog::NewGameDialog(QWidget *parent) :
        QWizard(parent), m_playersCount(NULL), m_signsCountToWin(NULL),
        m_boardSize(NULL), m_timeLimit(NULL) {
    connect(this, SIGNAL(accepted()), this, SLOT(createNewGame()));
    addPage( createInitialPage() );
}

NewGameDialog::~NewGameDialog() {

}

// vytvorenie novej hry
void NewGameDialog::createNewGame() {
    QVector<Player*> players;
    players.reserve(m_playersCount->text().toInt());
    players.push_back( new Player(NULL, new CrossPlayerSign(Qt::blue), "Wizard") );
    players.push_back( new Player(NULL, new CirclePlayerSign(Qt::red), "Arcan") );

    emit newGameCreated(
       new Game(players, NULL, m_boardSize->text().toInt(), m_timeLimit->text().toInt()*1000, m_signsCountToWin->text().toInt())
   );
}

// vytvaranie stran
QWizardPage *NewGameDialog::createInitialPage() {
    QWizardPage *newPage = new QWizardPage;
    newPage->setTitle( tr("Main settings") );

    // pocet hracov
    QHBoxLayout *playersCountLayout = new QHBoxLayout;
    playersCountLayout->addWidget( new QLabel( tr("Players count: ") ) );
    playersCountLayout->addWidget( (m_playersCount = new QLineEdit( QString().setNum(DEFAULT_PLAYERS_COUNT) )) );

    // pocet podpisov v rade na vytazstvo
    QHBoxLayout *signsCountToWinLayout = new QHBoxLayout;
    signsCountToWinLayout->addWidget( new QLabel( tr("Number of signs to win: ") ) );
    signsCountToWinLayout->addWidget( (m_signsCountToWin = new QLineEdit( QString().setNum(DEFAULT_WIN_STONES) )) );

    // velkost plochy
    QHBoxLayout *boardSizeLayout = new QHBoxLayout;
    boardSizeLayout->addWidget( new QLabel( tr("Number of squares in board side: ") ) );
    boardSizeLayout->addWidget( (m_boardSize = new QLineEdit( QString().setNum(DEFAULT_BOARD_SIZE) )) );

    // casovy limit tahu
    QHBoxLayout *timeLimitLayout = new QHBoxLayout;
    timeLimitLayout->addWidget( new QLabel( tr("Time limit of player's turn in seconds (0 = unlimited): ") ) );
    timeLimitLayout->addWidget( (m_timeLimit = new QLineEdit( QString().setNum(DEFAULT_TIME_LIMIT) )) );


    // pridanie sub layoutoch do hlavneho layoutu
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(playersCountLayout);
    mainLayout->addLayout(signsCountToWinLayout);
    mainLayout->addLayout(boardSizeLayout);
    mainLayout->addLayout(timeLimitLayout);

    // nastavenie layoutu a navrat funkcie
    newPage->setLayout(mainLayout);
    return newPage;
}


