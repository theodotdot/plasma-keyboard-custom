// SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

pragma Singleton

import QtQuick

/**
 * Bridge between the microphone key of a layout and the keyboard window.
 *
 * Layouts are loaded as separate QML components, so their keys cannot reach
 * the keyboard window's own properties or functions (Window.window is null
 * there). The key asks for the voice mode through this singleton instead, and
 * the window opens it.
 */
QtObject {
    //! Emitted when a microphone key is pressed.
    signal requested()
}
