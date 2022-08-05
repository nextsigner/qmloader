import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Window 2.12

Window {
    id: app
    visible: true
    visibility: "Maximized"
    width: app.fs*20
    height: app.fs*10
    title: qsTr("Qmloader: Información")
    property int fs: Screen.width*0.02
    Column{
        spacing: app.fs
        anchors.centerIn: parent
        Text{
            width: app.width-app.fs*2
            text: '<b>Qmloader:</b><br/>No se ha especicado correctamente qué aplicación QML debe cargar Qmloader.<br/>Si está viendo esta ventana es poque no está conectado a internet o no se ha podido descargar correctamente la aplicación de ayuda o documentación.<br /><br />Para más información dirijasé a https://github.com/nextsigner/qmloaderdoc'
            font.pixelSize: app.fs*0.5
            wrapMode: Text.WordWrap
        }
        Button{
            text: 'Ir al sitio de Ayuda'
            onClicked: Qt.openUrlExternally('https://github.com/nextsigner/qmloaderdoc')
        }
        Text{
            width: app.width-app.fs*2
            text: '<b>Contacto:</b><br/><b>E-Mail:</b>nextsigner@gmail.com<br/><b>Whatsapp:</b>+54 11 3802 4370'
            font.pixelSize: app.fs*0.5
            wrapMode: Text.WordWrap
        }
    }
    Shortcut{
        sequence: 'Esc'
        onActivated: Qt.quit()
    }
}
