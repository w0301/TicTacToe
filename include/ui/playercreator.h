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

#include <QList>
#include <QWidget>

class Player;
class PlayerSign;
class PlayerCreatorBase;

class QLineEdit;
class QComboBox;
class QPushButton;
class QColorDialog;


// PlayerCreatorRegistrator class - registruje vytvarac widgetu pre vytvorenie noveho hraca
class PlayerCreatorRegistrator {
    public:
        typedef PlayerCreatorBase *(*PlayerCreatorConstructor)();
        typedef QList< QPair<QString, PlayerCreatorConstructor> > PlayerCreatorsList;

        PlayerCreatorRegistrator(const QString&, PlayerCreatorConstructor);

        static PlayerCreatorsList& list() {
            return sm_allCreators;
        };

    private:
        static PlayerCreatorsList sm_allCreators;
};

// PlayerCreatorBase class - zakladna trieda pre triedy, ktore zobrazuju GUI vytvorenie hracov
class PlayerCreatorBase : public QWidget {
    Q_OBJECT

    public:
        typedef PlayerSign *(*PlayerSignConstructor)();
        typedef QList< QPair<bool, PlayerSignConstructor> > SignsList;

        PlayerCreatorBase(QWidget* = NULL);
        virtual ~PlayerCreatorBase() { };

        // vytvori instanciu vytvaracej triedy
        static PlayerCreatorBase* createCreator() {
            return NULL;
        };

        virtual Player* createPlayer() = 0;

        // registruje sign / vrati ich list
        static void registerSign(PlayerSignConstructor cntFunc) {
            m_signsList.append( qMakePair(true, cntFunc) );
        };
        static void enableSign(PlayerSignConstructor cnt, bool val) {
            m_signsList[m_signsList.indexOf(qMakePair(!val, cnt))].first = val;
        };
        static SignsList& signsList() {
            return m_signsList;
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

        // zaregistruje triedu
        static PlayerCreatorRegistrator sm_register;
};

#endif // PLAYERCREATOR_H
