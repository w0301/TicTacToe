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

#ifndef PLAYERCREATOR_H
#define PLAYERCREATOR_H

#include "main.h"

#include <QWidget>
#include <QVector>

class Player;
class PlayerSign;

class QLineEdit;
class QComboBox;
class QPushButton;
class QColorDialog;

// zakladna trieda pre triedy, ktore zobrazuju GUI vytvorenie hracov
class PlayerCreatorBase : public QWidget {
    public:
        typedef PlayerSign *(*PlayerSignConstructor)();
        typedef QVector< QPair<bool, PlayerSignConstructor> > SignsList;

        PlayerCreatorBase(QWidget* = NULL);
        virtual ~PlayerCreatorBase() { };

        // vytvori instanciu vytvaracej triedy
        static PlayerCreatorBase* createCreator() {
            return NULL;
        };

        // vytvori noveho hraca - u derivatoch zisti
        // parameter a vrati jeho ukazatel
        virtual Player* createPlayer() = 0;

        // registruje sign / vrati ich list
        static void registerSign(PlayerSignConstructor cntFunc) {
            m_signsList.push_back( QPair<bool, PlayerSignConstructor>(true, cntFunc) );
        };
        static SignsList& signsList() {
            return m_signsList;
        };
        static void enableSign(PlayerSignConstructor cnt, bool val) {
            m_signsList[m_signsList.indexOf( QPair<bool, PlayerSignConstructor>(!val, cnt) )].first = val;
        };

    private:
        static SignsList m_signsList;
};

// deklaracia vytvaracej triedy pre obycajneho hraca
class PlayerCreator : public PlayerCreatorBase {
    Q_OBJECT

    public:
        PlayerCreator(QWidget* = NULL);
        virtual ~PlayerCreator() { };

        // vrati novy objekt seba sameho
        static PlayerCreatorBase* createCreator() {
            return new PlayerCreator;
        };

        // vrati noveho hraca na zaklade dat z widgetov
        virtual Player* createPlayer() { return NULL; };

    public slots:
        // spusti dialog pre vyber farby
        void startColorDialog();

        // zmeni farbu color buttonu
        void changeColorButton(const QColor&);

        // refresne m_signType widget tak aby zobrazoval iba
        // pouzitelne podpisy
        void refreshSignTypes();

    private:
        // edit pre meno hraca
        QLineEdit *m_nameEdit;

        // widget pre vyber podpisu
        QComboBox *m_signType;

        // tlacitko, ktore zobrazuje farbu a spusta dialog pre jej vyber
        QPushButton *m_colorButton;

        // dialog vyberu farby podpisu
        QColorDialog *m_colorDialog;
};

#endif // PLAYERCREATOR_H
