/*
    SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "niriinputmethod.h"
#include "logging.h"

#include <QCoreApplication>
#include <QGuiApplication>
#include <QKeyEvent>

NiriInputMethod::NiriInputMethod(QObject *parent)
    : QVirtualKeyboardAbstractInputMethod(parent)
{
}

QList<QVirtualKeyboardInputEngine::InputMode> NiriInputMethod::inputModes(const QString &locale)
{
    Q_UNUSED(locale)
    return {QVirtualKeyboardInputEngine::InputMode::Latin};
}

bool NiriInputMethod::setInputMode(const QString &locale, QVirtualKeyboardInputEngine::InputMode inputMode)
{
    Q_UNUSED(locale)
    Q_UNUSED(inputMode)
    return true;
}

bool NiriInputMethod::setTextCase(QVirtualKeyboardInputEngine::TextCase textCase)
{
    m_textCase = textCase;
    return true;
}

bool NiriInputMethod::keyEvent(Qt::Key key, const QString &text, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(m_textCase)
    QObject *item = QGuiApplication::focusObject();
    if (!item) {
        qCWarning(PlasmaKeyboard) << "niri input method: no focus object for key" << key;
        return false;
    }

    // InputListenerItem expects a press/release pair; its handlers then send
    // the key to the compositor through the uinput device.
    QKeyEvent press(QEvent::KeyPress, key, modifiers, text);
    QCoreApplication::sendEvent(item, &press);
    QKeyEvent release(QEvent::KeyRelease, key, modifiers, text);
    QCoreApplication::sendEvent(item, &release);
    return true;
}
