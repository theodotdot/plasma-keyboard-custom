/*
    SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once

#include <QObject>
#include <QString>
#include <QTimer>

namespace PlasmaKeyboardStt
{

/**
 * The voice input of the keyboard, delegated to the Handy dictation service.
 *
 * Handy owns the microphone, the models, the voice activity detection and the
 * insertion of the recognised text; it is driven through its command line
 * (`handy --toggle-transcription`), which talks to the running instance.
 *
 * The keyboard therefore keeps no speech recognition of its own: it only
 * tracks the state the microphone key and the voice mode draw, and forwards
 * the errors Handy cannot report back (a missing binary, a failed launch).
 */
class SttManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool available READ isAvailable NOTIFY stateChanged)
    Q_PROPERTY(bool recording READ isRecording NOTIFY recordingChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(qreal level READ level NOTIFY levelChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)

public:
    explicit SttManager(QObject *parent = nullptr);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    /*! Whether the Handy command line is installed. */
    bool isAvailable() const;

    bool isRecording() const;
    bool isBusy() const;

    /*! Always 0: Handy does not report its microphone level. */
    qreal level() const;

    QString lastError() const;

    /*!
     * Toggles Handy's recording on. \a keyboardLanguage is accepted for API
     * compatibility and ignored: the recognition language is Handy's setting.
     */
    Q_INVOKABLE void startRecording(const QString &keyboardLanguage = QString());

    /*! Toggles Handy's recording off; Handy transcribes and inserts the text. */
    Q_INVOKABLE void stopRecording();

    /*! Cancels a recording or a transcription in progress. */
    Q_INVOKABLE void cancel();

Q_SIGNALS:
    void enabledChanged();
    void stateChanged();
    void recordingChanged();
    void busyChanged();
    void levelChanged();
    void lastErrorChanged();

private:
    bool runHandy(const QStringList &arguments);
    void setLastError(const QString &message);
    void settle();

    QTimer m_settleTimer;
    bool m_recording = false;
    bool m_busy = false;
    QString m_lastError;
};

} // namespace PlasmaKeyboardStt

//! Convenience name for the one place that creates the manager (main.cpp).
using SttManager = PlasmaKeyboardStt::SttManager;
