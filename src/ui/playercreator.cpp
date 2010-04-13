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

#include "ui/newgamedlg.h"
#include "ui/playersign.h"
#include "ui/playercreator.h"

#include <QtGui>

// PlayerCreatorRegistrator
PlayerCreatorRegistrator::PlayerCreatorsList PlayerCreatorRegistrator::sm_allCreators;

PlayerCreatorRegistrator::PlayerCreatorRegistrator(const QString& name, PlayerCreatorConstructor cnt) {
    sm_allCreators.append( qMakePair(name, cnt) );
}

// PlayerCreatorBase
PlayerCreatorBase::SignsList PlayerCreatorBase::m_signsList;

PlayerCreatorBase::PlayerCreatorBase(QWidget *parent) :
        QWidget(parent) {

}

// PlayerCreator
PlayerCreatorRegistrator PlayerCreator::sm_register(QObject::tr("Local player"), &PlayerCreator::createCreator);

PlayerCreator::PlayerCreator(QWidget *parent) :
        PlayerCreatorBase(parent), m_nameEdit(NULL), m_signType(NULL), m_colorButton(NULL), m_colorDialog(NULL) {
    // layout pre widget
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    /// pridanie widgetov do layoutu
    // edit pre meno
    mainLayout->addWidget( ( m_nameEdit = new QLineEdit( tr("Name of player") ) ) );
    mainLayout->addSpacing(1);

    // box pre vyber podpisu
    mainLayout->addWidget( (m_signType = new QComboBox) );
    refreshSignTypes();
}

// sloty
void PlayerCreator::refreshSignTypes() {
    PlayerCreatorBase::SignsList& list = PlayerCreatorBase::signsList();
    typedef const QPair<bool, PlayerCreatorBase::PlayerSignConstructor>& ref;
    foreach(ref i, list) {
        if(i.first == true) {
            m_signType->addItem( QIcon(i.second()->signPixmap(NULL, QPoint(0, 0), 16)), "" );
        }
    }
}

void PlayerCreator::startColorDialog() {

}

void PlayerCreator::changeColorButton(const QColor&) {

}
