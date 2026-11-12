/*
    SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once

#include <QObject>
#include <QString>

struct wl_display;
struct wl_registry;
struct wl_seat;
struct zwp_input_method_manager_v2;
struct zwp_input_method_v2;

/**
 * Text commits through zwp_input_method_v2.
 *
 * The uinput path can only press keys that exist on the compositor's keymap,
 * so characters such as "ê" cannot be typed by pressing keys at all. An input
 * method speaks the text protocol instead and hands the compositor a string,
 * which is what this class exposes.
 */
class ImV2Client : public QObject
{
    Q_OBJECT

public:
    static ImV2Client *instance();

    //! Whether a text input currently has this input method active.
    bool isActive() const;

    //! Commits @p text to the focused text input. Returns false when there is
    //! no binding (the compositor lacks the protocol) or no active text input.
    bool commitText(const QString &text);

    //! Binds the protocol on @p display. Safe to call more than once.
    void initialize(wl_display *display);

    static void handleGlobal(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t version);
    static void handleGlobalRemove(void *data, wl_registry *registry, uint32_t name);

    static void handleActivate(void *data, zwp_input_method_v2 *im);
    static void handleDeactivate(void *data, zwp_input_method_v2 *im);
    static void handleDone(void *data, zwp_input_method_v2 *im);
    static void handleUnavailable(void *data, zwp_input_method_v2 *im);

private:
    ImV2Client();

    wl_display *m_display = nullptr;
    wl_registry *m_registry = nullptr;
    wl_seat *m_seat = nullptr;
    zwp_input_method_manager_v2 *m_manager = nullptr;
    zwp_input_method_v2 *m_inputMethod = nullptr;
    bool m_active = false;
    uint32_t m_doneCount = 0;
    bool m_warnedInactive = false;
};
