/*
    SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "sttmanager.h"

#include "logging.h"
#include "plasmakeyboardsettings.h"

#include <QProcess>
#include <QStandardPaths>

namespace
{
//! Handy transcribes and pastes after the recording stops; until then the
//! voice mode shows its busy state. It does not report when it is done, so the
//! state falls back on its own after this long.
constexpr int settleDelayMs = 4000;

const QString handyBinary()
{
    return QStandardPaths::findExecutable(QStringLiteral("handy"));
}
}

namespace PlasmaKeyboardStt
{

SttManager::SttManager(QObject *parent)
    : QObject(parent)
{
    m_settleTimer.setSingleShot(true);
    connect(&m_settleTimer, &QTimer::timeout, this, &SttManager::settle);
}

bool SttManager::isEnabled() const
{
    return PlasmaKeyboardSettings::self()->sttEnabled();
}

void SttManager::setEnabled(bool enabled)
{
    if (enabled == isEnabled()) {
        return;
    }
    PlasmaKeyboardSettings::self()->setSttEnabled(enabled);
    PlasmaKeyboardSettings::self()->save();
    Q_EMIT enabledChanged();
    Q_EMIT stateChanged();
}

bool SttManager::isAvailable() const
{
    return !handyBinary().isEmpty();
}

bool SttManager::isRecording() const
{
    return m_recording;
}

bool SttManager::isBusy() const
{
    return m_busy;
}

qreal SttManager::level() const
{
    return 0;
}

QString SttManager::lastError() const
{
    return m_lastError;
}

void SttManager::startRecording(const QString &keyboardLanguage)
{
    Q_UNUSED(keyboardLanguage)

    if (m_recording) {
        return;
    }
    if (!isAvailable()) {
        setLastError(tr("Voice input needs the Handy dictation service, which is not installed."));
        return;
    }
    if (isBusy()) {
        // Still transcribing the previous phrase.
        return;
    }
    if (!runHandy({QStringLiteral("--toggle-transcription")})) {
        return;
    }

    setLastError(QString());
    m_recording = true;
    Q_EMIT recordingChanged();
    Q_EMIT stateChanged();
}

void SttManager::stopRecording()
{
    if (!m_recording) {
        return;
    }
    runHandy({QStringLiteral("--toggle-transcription")});

    m_recording = false;
    m_busy = true;
    Q_EMIT recordingChanged();
    Q_EMIT busyChanged();
    Q_EMIT stateChanged();
    m_settleTimer.start(settleDelayMs);
}

void SttManager::cancel()
{
    m_settleTimer.stop();
    if (m_recording) {
        runHandy({QStringLiteral("--cancel")});
    }
    m_recording = false;
    m_busy = false;
    Q_EMIT recordingChanged();
    Q_EMIT busyChanged();
    Q_EMIT stateChanged();
}

void SttManager::settle()
{
    if (!m_busy) {
        return;
    }
    m_busy = false;
    Q_EMIT busyChanged();
    Q_EMIT stateChanged();
}

bool SttManager::runHandy(const QStringList &arguments)
{
    const QString binary = handyBinary();
    if (binary.isEmpty()) {
        setLastError(tr("Voice input needs the Handy dictation service, which is not installed."));
        return false;
    }

    if (!QProcess::startDetached(binary, arguments)) {
        setLastError(tr("Could not start Handy."));
        return false;
    }
    qCDebug(PlasmaKeyboard) << "voice input: handy" << arguments;
    return true;
}

void SttManager::setLastError(const QString &message)
{
    if (message == m_lastError) {
        return;
    }
    m_lastError = message;
    Q_EMIT lastErrorChanged();
    Q_EMIT stateChanged();
}

} // namespace PlasmaKeyboardStt
