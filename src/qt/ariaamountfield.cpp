// Copyright (c) 2010-2018 The AriA developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "ariaamountfield.h"

#include "ariaunits.h"
#include "guiconstants.h"
#include "guiutil.h"
#include "qvaluecombobox.h"

#include <QAbstractSpinBox>
#include <QApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>

/** QSpinBox that uses fixed-point numbers internally and uses our own
 * formatting/parsing functions.
 */
class AmountSpinBox : public QAbstractSpinBox
{
    Q_OBJECT

public:
    explicit AmountSpinBox(QWidget* parent) : QAbstractSpinBox(parent),
                                              currentUnit(AriAUnits::ARA),
                                              singleStep(100000) // satoshis
    {
        setAlignment(Qt::AlignRight);

        connect(lineEdit(), SIGNAL(textEdited(QString)), this, SIGNAL(valueChanged()));
    }

    QValidator::State validate(QString& text, int& pos) const
    {
        if (text.isEmpty())
            return QValidator::Intermediate;
        bool valid = false;
        parse(text, &valid);
        /* Make sure we return Intermediate so that fixup() is called on defocus */
        return valid ? QValidator::Intermediate : QValidator::Invalid;
    }

    void fixup(QString& input) const
    {
        bool valid = false;
        CAmount val = parse(input, &valid);
        if (valid) {
            input = AriAUnits::format(currentUnit, val, false, AriAUnits::separatorAlways);
            lineEdit()->setText(input);
        }
    }

    CAmount value(bool* valid_out = 0) const
    {
        return parse(text(), valid_out);
    }

    void setValue(const CAmount& value)
    {
        lineEdit()->setText(AriAUnits::format(currentUnit, value, false, AriAUnits::separatorAlways));
        emit valueChanged();
    }

    void stepBy(int steps)
    {
        bool valid = false;
        CAmount val = value(&valid);
        val = val + steps * singleStep;
        val = qMin(qMax(val, CAmount(0)), AriAUnits::maxMoney());
        setValue(val);
    }

    void setDisplayUnit(int unit)
    {
        bool valid = false;
        CAmount val = value(&valid);

        currentUnit = unit;

        if (valid)
            setValue(val);
        else
            clear();
    }

    void setSingleStep(const CAmount& step)
    {
        singleStep = step;
    }

    QSize minimumSizeHint() const
    {
        if (cachedMinimumSizeHint.isEmpty()) {
            ensurePolished();

            const QFontMetrics fm(fontMetrics());
            int h = lineEdit()->minimumSizeHint().height();
            int w = fm.width(AriAUnits::format(AriAUnits::ARA, AriAUnits::maxMoney(), false, AriAUnits::separatorAlways));
            w += 2; // cursor blinking space

            QStyleOptionSpinBox opt;
            initStyleOption(&opt);
            QSize hint(w, h);
            QSize extra(35, 6);
            opt.rect.setSize(hint + extra);
            extra += hint - style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxEditField, this).size();
            // get closer to final result by repeating the calculation
            opt.rect.setSize(hint + extra);
            extra += hint - style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxEditField, this).size();
            hint += extra;
            hint.setHeight(h);

            opt.rect = rect();

            cachedMinimumSizeHint = style()->sizeFromContents(QStyle::CT_SpinBox, &opt, hint, this).expandedTo(QApplication::globalStrut());
        }
        return cachedMinimumSizeHint;
    }

private:
    int currentUnit;
    CAmount singleStep;
    mutable QSize cachedMinimumSizeHint;

    /**
     * Parse a string into a number of base monetary units and
     * return validity.
     * @note Must return 0 if !valid.
     */
    CAmount parse(const QString& text, bool* valid_out = 0) const
    {
        CAmount val = 0;
        bool valid = AriAUnits::parse(currentUnit, text, &val);
        if (valid) {
            if (val < 0 || val > AriAUnits::maxMoney())
                valid = false;
        }
        if (valid_out)
            *valid_out = valid;
        return valid ? val : 0;
    }

protected:
    bool event(QEvent* event)
    {
        if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Comma) {
                // Translate a comma into a period
                QKeyEvent periodKeyEvent(event->type(), Qt::Key_Period, keyEvent->modifiers(), ".", keyEvent->isAutoRepeat(), keyEvent->count());
                return QAbstractSpinBox::event(&periodKeyEvent);
            }
        }
        return QAbstractSpinBox::event(event);
    }

    StepEnabled stepEnabled() const
    {
        StepEnabled rv = 0;
        if (isReadOnly()) // Disable steps when AmountSpinBox is read-only
            return StepNone;
        if (text().isEmpty()) // Allow step-up with empty field
            return StepUpEnabled;
        bool valid = false;
        CAmount val = value(&valid);
        if (valid) {
            if (val > 0)
                rv |= StepDownEnabled;
            if (val < AriAUnits::maxMoney())
                rv |= StepUpEnabled;
        }
        return rv;
    }

signals:
    void valueChanged();
};

#include "ariaamountfield.moc"

AriAAmountField::AriAAmountField(QWidget* parent) : QWidget(parent),
                                                          amount(0)
{
    this->setObjectName("AriAAmountField"); // ID as CSS-reference
    // For whatever reasons the Gods of Qt-CSS-manipulation won't let us change this class' stylesheet in the CSS file.
    // Workaround for the people after me:
    // - name all UI objects, preferably with a unique name
    // - address those names globally in the CSS file

    amount = new AmountSpinBox(this);
    // According to the Qt-CSS specs this should work, but doesn't
    amount->setStyleSheet("QSpinBox::up-button:hover { background-color: #f2f2f2; }"
                          "QSpinBox::down-button:hover { background-color: #f2f2f2; }");
    amount->setLocale(QLocale::c());
    amount->installEventFilter(this);
    amount->setMaximumWidth(170);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(amount);
    unit = new QValueComboBox(this);
    unit->setModel(new AriAUnits(this));
    layout->addWidget(unit);
    layout->addStretch(1);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);

    setFocusPolicy(Qt::TabFocus);
    setFocusProxy(amount);

    // If one if the widgets changes, the combined content changes as well
    connect(amount, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
    connect(unit, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged(int)));

    // Set default based on configuration
    unitChanged(unit->currentIndex());
}

void AriAAmountField::clear()
{
    amount->clear();
    unit->setCurrentIndex(0);
}

void AriAAmountField::setEnabled(bool fEnabled)
{
    amount->setEnabled(fEnabled);
    unit->setEnabled(fEnabled);
}

bool AriAAmountField::validate()
{
    bool valid = false;
    value(&valid);
    setValid(valid);
    return valid;
}

void AriAAmountField::setValid(bool valid)
{
    if (valid)
        // According to the Qt-CSS specs this should work, but doesn't
        amount->setStyleSheet("QSpinBox::up-button:hover { background-color: #f2f2f2 }"
                              "QSpinBox::down-button:hover { background-color: #f2f2f2 }");
    else
        amount->setStyleSheet(STYLE_INVALID);
}

bool AriAAmountField::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::FocusIn) {
        // Clear invalid flag on focus
        setValid(true);
    }
    return QWidget::eventFilter(object, event);
}

QWidget* AriAAmountField::setupTabChain(QWidget* prev)
{
    QWidget::setTabOrder(prev, amount);
    QWidget::setTabOrder(amount, unit);
    return unit;
}

CAmount AriAAmountField::value(bool* valid_out) const
{
    return amount->value(valid_out);
}

void AriAAmountField::setValue(const CAmount& value)
{
    amount->setValue(value);
}

void AriAAmountField::setReadOnly(bool fReadOnly)
{
    amount->setReadOnly(fReadOnly);
    unit->setEnabled(!fReadOnly);
}

void AriAAmountField::unitChanged(int idx)
{
    // Use description tooltip for current unit for the combobox
    unit->setToolTip(unit->itemData(idx, Qt::ToolTipRole).toString());

    // Determine new unit ID
    int newUnit = unit->itemData(idx, AriAUnits::UnitRole).toInt();

    amount->setDisplayUnit(newUnit);
}

void AriAAmountField::setDisplayUnit(int newUnit)
{
    unit->setValue(newUnit);
}

void AriAAmountField::setSingleStep(const CAmount& step)
{
    amount->setSingleStep(step);
}
