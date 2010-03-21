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

#ifndef PLAYBOARD_H
#define PLAYBOARD_H

#include <QWidget>
#include <QVector>

class Player;

class PlayBoard : public QWidget {
    Q_OBJECT

    public:
        // vytvori hraciu plochu
        PlayBoard(QWidget*, int);

        ~PlayBoard();

        // vrati pocet stvorcekov na jednom riadku
        int squareCount() const {
            return m_size;
        };

        bool putPlayer(int, int, Player*);
        bool putPlayerOriental(int, int, Player*);

        QPoint squareCoords(int, int);

        // postara sa o priradenie hracov ploche
        void mouseReleaseEvent(QMouseEvent*);

        // nastavi premenne urcujuce velkosti atd.
        void resizeEvent(QResizeEvent*);

        // nakresli hraciu plochu
        void paintEvent(QPaintEvent*);

    private:
        // uchovava info o jednotlivich stvorcekoch
        QVector< QVector<Player*> >  m_board;

        // uchovava dlzku resp. vysku plochy v stvorcekoch
        int m_size;

        // urcuje velkost vykreslovaneho stvoreceka
        int m_sideSize;

        // urcuje kde sa (ralativne k widgetu) zacina
        // vykreslovat hracia plocha
        int m_fromRight;
        int m_fromTop;
};

#endif // PLAYBOARD_H
