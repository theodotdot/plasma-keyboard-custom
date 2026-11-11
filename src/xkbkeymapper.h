/*
    SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once

#include <QString>
#include <QtGlobal>

#include <xkbcommon/xkbcommon.h>

/**
 * Resolves a keysym to an evdev keycode on a keymap built from the
 * compositor's configured layout, plus the level that keycode has to be
 * pressed at.
 *
 * Needed by the uinput transport: the input-method-v1 keymap that
 * Keyboard::evdevKeycodeForKeysym() scans is only available while a Plasma
 * text-input context exists. This builds its own keymap from the XKB rules,
 * so keys can be mapped with no input-method context at all (niri, or any
 * compositor without input-method-unstable-v1).
 */
class XkbKeyMapper
{
public:
    struct Match {
        //! evdev scancode (xkb keycode - 8); 0 when the keysym is unmapped.
        quint32 keycode = 0;
        //! XKB level of the match: 0 none, 1 shift, 2 altgr, 3 shift+altgr.
        int level = 0;

        bool isValid() const
        {
            return keycode != 0;
        }
    };

    explicit XkbKeyMapper(const QString &layout, const QString &variant = QString());
    ~XkbKeyMapper();

    XkbKeyMapper(const XkbKeyMapper &) = delete;
    XkbKeyMapper &operator=(const XkbKeyMapper &) = delete;

    bool isValid() const
    {
        return m_keymap != nullptr;
    }

    /**
     * Finds @p keysym on the keymap. Case is folded through the keysym's UTF-32
     * value, the same way Keyboard::evdevKeycodeForKeysym() does it, so a
     * shifted letter is found on its unshifted keycode. Levels are scanned in
     * order, so a symbol that needs Shift or AltGr is reported with that level.
     */
    Match find(xkb_keysym_t keysym) const;

private:
    xkb_context *m_context = nullptr;
    xkb_keymap *m_keymap = nullptr;
};
