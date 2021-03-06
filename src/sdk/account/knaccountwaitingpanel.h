/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNACCOUNTWAITINGPANEL_H
#define KNACCOUNTWAITINGPANEL_H

#include <QWidget>

class QLabel;
class KNClockWheel;
/*!
 * \brief The KNAccountWaitingPanel class provides a wheel widget for waiting
 * widget.
 */
class KNAccountWaitingPanel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountWaitingPanel widget.
     * \param parent The parent widget.
     */
    explicit KNAccountWaitingPanel(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Let the ticking ring start ticking.
     */
    void startTicking();

    /*!
     * \brief Stop the ticking ring.
     */
    void stopTicking();

private slots:
    void retranslate();

private:
    KNClockWheel *m_waitingWheel;
    QLabel *m_waitingHint;
};

#endif // KNACCOUNTWAITINGPANEL_H
