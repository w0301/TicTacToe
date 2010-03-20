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

class PlayBoard : public QWidget {
    Q_OBJECT

    public:
        // vytvori hraciu plochu
        PlayBoard(QWidget*, int);

        // nakresli hraciu plochu
        void paintEvent(QPaintEvent*);

        // vrati pocet stvorcekov na jednom riadku
        int squareCount() const {
            return m_size;
        };

    private:
        // uchovava info o jednotlivich stvorcekoch
        int **m_board;

        // uchovava dlzku resp. vysku plochy v stvorcekoch
        int m_size;
};

#endif // PLAYBOARD_H
