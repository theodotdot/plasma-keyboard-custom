/*
    SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "imv2client.h"

#include "logging.h"

#include <QDebug>
#include <QGuiApplication>
#include <QLoggingCategory>
#include <QtGui/qpa/qplatformnativeinterface.h>

#include <wayland-client.h>

#include "wayland-input-method-unstable-v2-client-protocol.h"

namespace
{
const zwp_input_method_v2_listener inputMethodListener = {
    .activate = ImV2Client::handleActivate,
    .deactivate = ImV2Client::handleDeactivate,
    .surrounding_text = [](void *, zwp_input_method_v2 *, const char *, uint32_t, uint32_t) {},
    .text_change_cause = [](void *, zwp_input_method_v2 *, uint32_t) {},
    .content_type = [](void *, zwp_input_method_v2 *, uint32_t, uint32_t) {},
    .done = ImV2Client::handleDone,
    .unavailable = ImV2Client::handleUnavailable,
};

const wl_registry_listener registryListener = {
    .global = ImV2Client::handleGlobal,
    .global_remove = ImV2Client::handleGlobalRemove,
};
}

ImV2Client::ImV2Client()
    : QObject(nullptr)
{
}

ImV2Client *ImV2Client::instance()
{
    static ImV2Client client;
    return &client;
}

bool ImV2Client::isActive() const
{
    return m_active;
}

void ImV2Client::initialize(wl_display *display)
{
    if (m_display || !display) {
        return;
    }
    m_display = display;

    m_registry = wl_display_get_registry(m_display);
    wl_registry_add_listener(m_registry, &registryListener, this);
    // Collect the globals, then the input method's initial events.
    wl_display_roundtrip(m_display);
    wl_display_roundtrip(m_display);

    if (!m_manager) {
        qCWarning(PlasmaKeyboard) << "input method v2: compositor does not offer zwp_input_method_v2";
        return;
    }
    if (!m_seat) {
        qCWarning(PlasmaKeyboard) << "input method v2: no seat to attach the input method to";
        return;
    }

    m_inputMethod = zwp_input_method_manager_v2_get_input_method(m_manager, m_seat);
    zwp_input_method_v2_add_listener(m_inputMethod, &inputMethodListener, this);
    wl_display_flush(m_display);
    qCDebug(PlasmaKeyboard) << "input method v2: bound";
}

bool ImV2Client::commitText(const QString &text)
{
    if (text.isEmpty()) {
        return false;
    }
    if (!m_inputMethod) {
        return false;
    }
    if (!m_active && !m_warnedInactive) {
        qCWarning(PlasmaKeyboard) << "input method v2: no active text input, cannot commit" << text;
        m_warnedInactive = true;
        return false;
    }
    if (!m_active) {
        return false;
    }

    zwp_input_method_v2_commit_string(m_inputMethod, text.toUtf8().constData());
    // The serial is the number of done events received so far; see the
    // protocol's commit description.
    zwp_input_method_v2_commit(m_inputMethod, m_doneCount);
    wl_display_flush(m_display);
    qCDebug(PlasmaKeyboard) << "input method v2: committed" << text;
    return true;
}

void ImV2Client::handleGlobal(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t version)
{
    auto *self = static_cast<ImV2Client *>(data);
    if (qstrcmp(interface, zwp_input_method_manager_v2_interface.name) == 0 && !self->m_manager) {
        self->m_manager = static_cast<zwp_input_method_manager_v2 *>(
            wl_registry_bind(registry, name, &zwp_input_method_manager_v2_interface, qMin(version, 1u)));
    } else if (qstrcmp(interface, wl_seat_interface.name) == 0 && !self->m_seat) {
        self->m_seat = static_cast<wl_seat *>(wl_registry_bind(registry, name, &wl_seat_interface, qMin(version, 4u)));
    }
}

void ImV2Client::handleGlobalRemove(void *, wl_registry *, uint32_t)
{
}

void ImV2Client::handleActivate(void *data, zwp_input_method_v2 *)
{
    auto *self = static_cast<ImV2Client *>(data);
    self->m_active = true;
    self->m_warnedInactive = false;
    qCDebug(PlasmaKeyboard) << "input method v2: active";
}

void ImV2Client::handleDeactivate(void *data, zwp_input_method_v2 *)
{
    auto *self = static_cast<ImV2Client *>(data);
    self->m_active = false;
    qCDebug(PlasmaKeyboard) << "input method v2: inactive";
}

void ImV2Client::handleDone(void *data, zwp_input_method_v2 *)
{
    auto *self = static_cast<ImV2Client *>(data);
    ++self->m_doneCount;
}

void ImV2Client::handleUnavailable(void *data, zwp_input_method_v2 *)
{
    auto *self = static_cast<ImV2Client *>(data);
    qCWarning(PlasmaKeyboard) << "input method v2: another input method took the seat";
    self->m_active = false;
}

