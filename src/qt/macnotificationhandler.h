// Copyright (c) 2010-2018 The AriA developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ARIA_QT_MACNOTIFICATIONHANDLER_H
#define ARIA_QT_MACNOTIFICATIONHANDLER_H

#include <QObject>

/** Macintosh-specific notification handler (supports UserNotificationCenter).
 */
class MacNotificationHandler : public QObject
{
    Q_OBJECT

public:
    /** shows a 10.8+ UserNotification in the UserNotificationCenter
     */
    void showNotification(const QString& title, const QString& text);

    /** executes AppleScript */
    void sendAppleScript(const QString& script);

    /** check if OS can handle UserNotifications */
    bool hasUserNotificationCenterSupport(void);
    static MacNotificationHandler* instance();
};


#endif // ARIA_QT_MACNOTIFICATIONHANDLER_H
