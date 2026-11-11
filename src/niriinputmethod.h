/*
    SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once

#include <QVirtualKeyboardAbstractInputMethod>

/**
 * Input method that delivers Qt Virtual Keyboard's keys to the application's
 * own input item.
 *
 * Qt Virtual Keyboard normally gets an input method from its platform input
 * context plugin. The Wayland Qt platform plugin refuses to load that plugin
 * ("qtvirtualkeyboard currently is not supported at client-side"), so under
 * niri the input engine would have no input method and every tap would be
 * dropped with "input method is not set".
 *
 * This implementation forwards each virtual key to the focused object as a
 * normal QKeyEvent pair, which is exactly what InputListenerItem expects: its
 * keyPressEvent()/keyReleaseEvent() then send the key out through uinput.
 */
class NiriInputMethod : public QVirtualKeyboardAbstractInputMethod
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided as a singleton by the application")

public:
    explicit NiriInputMethod(QObject *parent = nullptr);

    QList<QVirtualKeyboardInputEngine::InputMode> inputModes(const QString &locale) override;
    bool setInputMode(const QString &locale, QVirtualKeyboardInputEngine::InputMode inputMode) override;
    bool setTextCase(QVirtualKeyboardInputEngine::TextCase textCase) override;
    bool keyEvent(Qt::Key key, const QString &text, Qt::KeyboardModifiers modifiers) override;

private:
    QVirtualKeyboardInputEngine::TextCase m_textCase = QVirtualKeyboardInputEngine::TextCase::Lower;
};
