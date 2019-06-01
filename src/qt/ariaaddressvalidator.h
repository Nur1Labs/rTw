// Copyright (c) 2010-2018 The AriA developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ARIA_QT_ADDRESSVALIDATOR_H
#define ARIA_QT_ADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class AriAAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit AriAAddressEntryValidator(QObject* parent);

    State validate(QString& input, int& pos) const;
};

/** AriA address widget validator, checks for a valid aria address.
 */
class AriAAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit AriAAddressCheckValidator(QObject* parent);

    State validate(QString& input, int& pos) const;
};

#endif // ARIA_QT_ADDRESSVALIDATOR_H
