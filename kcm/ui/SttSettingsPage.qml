/*
    SPDX-FileCopyrightText: 2026 Plasma Keyboard contributors

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

/**
 * Voice input settings.
 *
 * The dictation itself is Handy's: it owns the microphone, the models, the
 * voice activity detection and the insertion of the recognised text. The
 * keyboard only shows the microphone key and toggles Handy's recording, so
 * this page holds the switch and points at Handy for everything else.
 */
SettingsFormPage {
    id: page

    SettingsRow {
        label: i18n("Voice input:")
        description: i18n("Dictate into the focused field. A microphone key appears next to the space bar while this is on.")

        QQC2.Switch {
            checked: kcm.sttEnabled
            onCheckedChanged: {
                kcm.sttEnabled = checked;
                checked = Qt.binding(() => kcm.sttEnabled);
            }
        }
    }

    SettingsRow {
        label: i18n("Dictation service:")
        description: i18n("Recording, speech recognition and text insertion are handled by Handy. Choose its models, language and microphone in Handy's own settings; the keyboard follows them.")
    }

    Kirigami.InlineMessage {
        Layout.fillWidth: true
        type: Kirigami.MessageType.Information
        visible: true
        text: i18n("Handy must be running for the microphone key to work. Start it with your session, and make sure “wtype” is installed so it can type into Wayland applications.")
    }
}
