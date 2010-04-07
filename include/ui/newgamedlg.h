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

#ifndef NEWGAMEDLG_H
#define NEWGAMEDLG_H

#include "main.h"

#include <QWizard>


class Game;

class QLabel;
class QLineEdit;
class QSpinBox;
class QWizardPage;

// NewGameDialog class
class NewGameDialog : public QWizard {
    Q_OBJECT

    public:
        // vytvori dialog
        NewGameDialog(QWidget* = NULL);

        // znici dialog
        ~NewGameDialog();

        // vytvori jednotlive strany
        QWizardPage *createInitialPage();
        QWizardPage *createPlayersPage();

    public slots:
        // vytvori hru a posle ju signalom newGameCreated
        void createNewGame();

    signals:
        void newGameCreated(Game*);

    private:
        // widgety 1. strany
        QSpinBox *m_playersCount;
        QSpinBox *m_signsCountToWin;
        QSpinBox *m_boardSize;
        QLineEdit *m_timeLimit;

};

#endif // NEWGAMEDLG_H
