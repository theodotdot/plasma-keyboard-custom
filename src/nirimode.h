/*
    SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once

#include <QByteArray>
#include <QtGlobal>

/**
 * Whether the generic Wayland backend is active (niri and other compositors
 * that do not implement input-method-unstable-v1).
 *
 * Enabled with PLASMA_KEYBOARD_NIRI=1. The Plasma path stays the default so
 * the fork keeps behaving as before on Plasma.
 */
inline bool plasmaKeyboardNiriMode()
{
    static const bool enabled = [] {
        const QByteArray value = qgetenv("PLASMA_KEYBOARD_NIRI");
        return !value.isEmpty() && value != "0" && value != "false";
    }();
    return enabled;
}
