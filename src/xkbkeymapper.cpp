/*
    SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "xkbkeymapper.h"

#include <QByteArray>
#include <QDebug>

namespace
{
uint32_t normalizeKeysymUtf32(uint32_t keysym)
{
    uint32_t utf32 = xkb_keysym_to_utf32(keysym);
    if (utf32 >= 'A' && utf32 <= 'Z') {
        utf32 += 'a' - 'A';
    }
    return utf32;
}
}

XkbKeyMapper::XkbKeyMapper(const QString &layout, const QString &variant)
{
    m_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!m_context) {
        return;
    }

    // The strings only have to outlive the call; xkb_keymap_new_from_names()
    // copies them into the keymap.
    const QByteArray layoutBytes = (layout.isEmpty() ? QStringLiteral("us") : layout).toUtf8();
    const QByteArray variantBytes = variant.toUtf8();
    struct xkb_rule_names names = {
        .rules = "evdev",
        .model = "pc105",
        .layout = layoutBytes.constData(),
        .variant = variantBytes.isEmpty() ? nullptr : variantBytes.constData(),
        .options = nullptr,
    };

    m_keymap = xkb_keymap_new_from_names(m_context, &names, XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (!m_keymap) {
        qWarning() << "XkbKeyMapper: could not build a keymap for layout" << layoutBytes;
        xkb_context_unref(m_context);
        m_context = nullptr;
    }
}

XkbKeyMapper::~XkbKeyMapper()
{
    if (m_keymap) {
        xkb_keymap_unref(m_keymap);
    }
    if (m_context) {
        xkb_context_unref(m_context);
    }
}

XkbKeyMapper::Match XkbKeyMapper::find(xkb_keysym_t keysym) const
{
    Match match;
    if (!m_keymap || keysym == XKB_KEY_NoSymbol) {
        return match;
    }

    const uint32_t targetUtf32 = normalizeKeysymUtf32(keysym);
    const xkb_keycode_t minKeycode = xkb_keymap_min_keycode(m_keymap);
    const xkb_keycode_t maxKeycode = xkb_keymap_max_keycode(m_keymap);

    // Exact keysym first, so a layout that has both an ASCII and a non-ASCII
    // keysym with the same UTF-32 value still resolves deterministically.
    for (xkb_keycode_t code = minKeycode; code <= maxKeycode; ++code) {
        const int levels = xkb_keymap_num_levels_for_key(m_keymap, code, 0);
        for (int level = 0; level < levels; ++level) {
            const xkb_keysym_t *syms = nullptr;
            const int count = xkb_keymap_key_get_syms_by_level(m_keymap, code, 0, level, &syms);
            for (int i = 0; i < count; ++i) {
                if (syms[i] == keysym) {
                    return {code - 8, level};
                }
            }
        }
    }

    if (targetUtf32 != 0) {
        for (xkb_keycode_t code = minKeycode; code <= maxKeycode; ++code) {
            const int levels = xkb_keymap_num_levels_for_key(m_keymap, code, 0);
            for (int level = 0; level < levels; ++level) {
                const xkb_keysym_t *syms = nullptr;
                const int count = xkb_keymap_key_get_syms_by_level(m_keymap, code, 0, level, &syms);
                for (int i = 0; i < count; ++i) {
                    if (normalizeKeysymUtf32(syms[i]) == targetUtf32) {
                        return {code - 8, level};
                    }
                }
            }
        }
    }

    return match;
}
