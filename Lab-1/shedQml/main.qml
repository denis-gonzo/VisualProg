import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: appWindow

    visible: true
    title: "Расписание"
    property int margin: 11
    width: mainLayout.implicitWidth + 2 * margin
    height: mainLayout.implicitHeight + 2 * margin
    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: appWindow.margin
        RowLayout{
            id: topRow
            Layout.fillWidth: true

            ColumnLayout {
                id: leftColumn
                Layout.fillWidth: true
                Layout.maximumHeight: rightColumn.height
                RowLayout{
                    id: actionRow
                    Layout.fillWidth: true
                    Label { text: "Действие" }
                    ComboBox { Layout.fillWidth: true; model: ["Праздник","Выходной","Встреча"] }
                }
                Item { Layout.fillHeight: true } //spacer
                Button { Layout.fillWidth: true; text: "Информация" }
            }
            Rectangle { Layout.margins: appWindow.margin; border.color: "darkgray"; width: 2; height: rightColumn.height }
            ColumnLayout {
                id: rightColumn
                Layout.fillWidth: true
                GridLayout {
                    id: animalLayout
                    rows: 2
                    flow: GridLayout.TopToBottom
                    Label { text: "Животное" }
                    Label { text: "Вид" }

                    ComboBox { id: animal; Layout.fillWidth: true; model: [ "ДжоДжо","Ниф Ниф","Наф Наф", "Нуф Нуф" ] }
                    Label { Layout.fillWidth: true; text: animal.currentIndex===0 ? "Капибара":"Поросёнок" }
                }
                Button { Layout.fillWidth: true; text: "Информация" }
                Button { Layout.fillWidth: true; text: "Поиск" }
            }
        }
        GridLayout{
            id: datetimeGrid
            rows: 2
            flow: GridLayout.TopToBottom
            Label { id: timelabel; text: "Время" }
            Label { text: "Дата" }
            Rectangle{
                Layout.fillWidth: true
                height: timeInput.implicitHeight + 4
                color:"white"
                border.color: "darkgray"
                border.width: 1
                TextInput {
                    id: timeInput
                    width: parent.width-4
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 2

                    text: "00:00"
                    inputMask: "99:99"
                    validator: RegExpValidator { regExp: /^([0-1\s]?[0-9\s]|2[0-3\s]):([0-5\s][0-9\s])$ / }
                }
            }
            Calendar { id: calendar; Layout.fillWidth: true;Layout.fillHeight: true}
        }
        RowLayout {
            id: bottomButtons
            Layout.fillWidth: true
            Button { Layout.fillWidth: true; text: "Назначить" }
            Button { Layout.fillWidth: true; text: "Выход"; onClicked: Qt.quit() }
        }
    }
}
