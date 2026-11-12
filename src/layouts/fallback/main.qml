// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.VirtualKeyboard.Components
import QtQuick.Layouts

import org.kde.plasma.keyboard.custom.lib as PlasmaKeyboard

KeyboardLayout {
    id: pcLayout
    // Caps Lock affects letters, not the punctuation level of PC keys.
    readonly property bool symbolsShifted: InputContext.shiftActive && !InputContext.capsLockActive
    inputMode: InputEngine.InputMode.Latin
    keyWeight: 160
    readonly property real normalKeyWidth: normalKey.width
    readonly property real functionKeyWidth: mapFromItem(normalKey, normalKey.width / 2, 0).x
    KeyboardRow {
        Key {
            text: pcLayout.symbolsShifted ? "~" : "`"
            alternativeKeys: "`~"
            smallText: pcLayout.symbolsShifted ? "`" : "~"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "!" : "1"
            alternativeKeys: "1!"
            smallText: pcLayout.symbolsShifted ? "1" : "!"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "@" : "2"
            alternativeKeys: "2@"
            smallText: pcLayout.symbolsShifted ? "2" : "@"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "#" : "3"
            alternativeKeys: "3#"
            smallText: pcLayout.symbolsShifted ? "3" : "#"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "$" : "4"
            alternativeKeys: "4$"
            smallText: pcLayout.symbolsShifted ? "4" : "$"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "%" : "5"
            alternativeKeys: "5%"
            smallText: pcLayout.symbolsShifted ? "5" : "%"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "^" : "6"
            alternativeKeys: "6^"
            smallText: pcLayout.symbolsShifted ? "6" : "^"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "&" : "7"
            alternativeKeys: "7&"
            smallText: pcLayout.symbolsShifted ? "7" : "&"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "*" : "8"
            alternativeKeys: "8*"
            smallText: pcLayout.symbolsShifted ? "8" : "*"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "(" : "9"
            alternativeKeys: "9("
            smallText: pcLayout.symbolsShifted ? "9" : "("
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? ")" : "0"
            alternativeKeys: "0)"
            smallText: pcLayout.symbolsShifted ? "0" : ")"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "_" : "-"
            alternativeKeys: "-_"
            smallText: pcLayout.symbolsShifted ? "-" : "_"
            smallTextVisible: true
        }
        Key {
            text: pcLayout.symbolsShifted ? "+" : "="
            alternativeKeys: "=+"
            smallText: pcLayout.symbolsShifted ? "=" : "+"
            smallTextVisible: true
        }
    }
    KeyboardRow {
        Key {
            key: Qt.Key_Q
            text: "q"
        }
        Key {
            id: normalKey
            key: Qt.Key_W
            text: "w"
        }
        Key {
            key: Qt.Key_E
            text: "e"
            alternativeKeys: "êeëèé"
        }
        Key {
            key: Qt.Key_R
            text: "r"
            alternativeKeys: "ŕrř"
        }
        Key {
            key: Qt.Key_T
            text: "t"
            alternativeKeys: "ţtŧť"
        }
        Key {
            key: Qt.Key_Y
            text: "y"
            alternativeKeys: "ÿyýŷ"
        }
        Key {
            key: Qt.Key_U
            text: "u"
            alternativeKeys: "űūũûüuùú"
        }
        Key {
            key: Qt.Key_I
            text: "i"
            alternativeKeys: "îïīĩiìí"
        }
        Key {
            key: Qt.Key_O
            text: "o"
            alternativeKeys: "œøõôöòóo"
        }
        Key {
            key: Qt.Key_P
            text: "p"
        }
    }
    KeyboardRow {
        KeyboardRow {
            Layout.preferredWidth: functionKeyWidth
            Layout.fillWidth: false
            FillerKey {
            }
            Key {
                key: Qt.Key_A
                text: "a"
                alternativeKeys: "aäåãâàá"
                weight: normalKeyWidth
                Layout.fillWidth: false
            }
        }
        Key {
            key: Qt.Key_S
            text: "s"
            alternativeKeys: "šsşś"
        }
        Key {
            key: Qt.Key_D
            text: "d"
            alternativeKeys: "dđď"
        }
        Key {
            key: Qt.Key_F
            text: "f"
        }
        Key {
            key: Qt.Key_G
            text: "g"
            alternativeKeys: "ġgģĝğ"
        }
        Key {
            key: Qt.Key_H
            text: "h"
        }
        Key {
            key: Qt.Key_J
            text: "j"
        }
        Key {
            key: Qt.Key_K
            text: "k"
        }
        KeyboardRow {
            Layout.preferredWidth: functionKeyWidth
            Layout.fillWidth: false
            Key {
                key: Qt.Key_L
                text: "l"
                alternativeKeys: "ĺŀłļľl"
                weight: normalKeyWidth
                Layout.fillWidth: false
            }
            FillerKey {
            }
        }
    }
    KeyboardRow {
        ShiftKey {
            weight: functionKeyWidth
            Layout.fillWidth: false
        }
        Key {
            key: Qt.Key_Z
            text: "z"
            alternativeKeys: "zžż"
        }
        Key {
            key: Qt.Key_X
            text: "x"
        }
        Key {
            key: Qt.Key_C
            text: "c"
            alternativeKeys: "çcċčć"
        }
        Key {
            key: Qt.Key_V
            text: "v"
        }
        Key {
            key: Qt.Key_B
            text: "b"
        }
        Key {
            key: Qt.Key_N
            text: "n"
            alternativeKeys: "ņńnň"
        }
        Key {
            key: Qt.Key_M
            text: "m"
        }
        BackspaceKey {
            weight: functionKeyWidth
            Layout.fillWidth: false
        }
    }
    KeyboardRow {
        SymbolModeKey {
            weight: functionKeyWidth
            Layout.fillWidth: false
        }
        PlasmaKeyboard.FloatingKey {
            weight: normalKeyWidth
            Layout.fillWidth: false
        }
        Key {
            key: Qt.Key_Comma
            weight: normalKeyWidth
            Layout.fillWidth: false
            text: ","
            highlighted: true
        }
        SpaceKey {
        }
        Key {
            key: Qt.Key_Period
            weight: normalKeyWidth
            Layout.fillWidth: false
            text: "."
            alternativeKeys: "!.?"
            smallText: "!?"
            smallTextVisible: true
            highlighted: true
        }
        HideKeyboardKey {
            weight: normalKeyWidth
            Layout.fillWidth: false
        }
        EnterKey {
            weight: functionKeyWidth
            Layout.fillWidth: false
        }
    }
}
