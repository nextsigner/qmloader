#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QDir>

#include "unik.h"
#include "unikqprocess.h"
#include "qmlclipboardadapter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setApplicationDisplayName("Zool");
    app.setOrganizationName("Zool.ar");
    app.setOrganizationDomain("zool.ar");

    Unik u;
    bool isDev=false;
    QQmlApplicationEngine engine;
    //engine.rootContext()->setContextObject("unik", &u);
    qmlRegisterType<Unik>("unik.Unik", 1, 0, "Unik");
    qmlRegisterType<UnikQProcess>("unik.UnikQProcess", 1, 0, "UnikQProcess");
    QByteArray argtitle="";
    QByteArray mainLocation="main.qml";
    QByteArray modulesPath="";
    QByteArray urlGitArg="";
    QByteArray folderArg="";
    bool loadConf=true;
    for (int i=0; i<argc; i++) {
        qDebug()<<"Arg: "<<argv[i];
        if(QString(argv[i]).contains("-git=")){
            QStringList nvi=QString(argv[i]).split("-git=");
            urlGitArg.append(nvi.at(1));
            loadConf=false;
        }
        if(QString(argv[i]).contains("-folder=")){
            QStringList nvi=QString(argv[i]).split("-folder=");
            folderArg.append(nvi.at(1));
            loadConf=false;
        }
    }

    QByteArray confPath="";
    confPath.append(QDir::currentPath());
    confPath.append("/qmloader.git");
    qDebug()<<"Buscando archivo qmloader.git en: "<<confPath;

    if(loadConf && u.fileExist(confPath)){
        qDebug()<<"Leyendo archivo qmloader.git...";
        QString gitUrl=u.getFile(confPath);
        gitUrl=gitUrl.replace("\n", "");

        qDebug()<<"Contenido de archivo qmloader.git: "<<gitUrl;
        QDir::setCurrent("../");
        qDebug()<<"Carpeta actual: "<<QDir::currentPath();
        QStringList m0=gitUrl.split("/");
        QString urlFolder=m0.at(m0.length()-1);
        urlFolder=urlFolder.replace(".git", "");
        QString gitFolder="";
        gitFolder.append(QDir::currentPath());
        gitFolder.append("/");
        gitFolder.append(urlFolder);
        bool d = u.downloadGit(gitUrl.toUtf8(), gitFolder.toUtf8());
        QDir::setCurrent(urlFolder);
        qDebug()<<"2 asdCarpeta actual: "<<QDir::currentPath();
        mainLocation="";
        mainLocation.append(QDir::currentPath());
        modulesPath.append(mainLocation);
        modulesPath.append("/modules");
        engine.addImportPath(modulesPath);
        mainLocation.append("/main.qml");
    }else{
        if(folderArg!=""){
            mainLocation="";
            mainLocation.append(folderArg);
            QDir::setCurrent(mainLocation);
            modulesPath.append(mainLocation);
            modulesPath.append("/modules");
            engine.addImportPath(modulesPath);
            mainLocation.append("/main.qml");
            qDebug()<<"Current folder seted from arg: "<<folderArg;
        }else if(urlGitArg!=""){
            //Git url form arg
            QString gitUrl="";
            gitUrl.append(urlGitArg);
            gitUrl=gitUrl.replace("\n", "");

            qDebug()<<"Contenido de archivo qmloader.git: "<<gitUrl;
            QDir::setCurrent("../");
            qDebug()<<"Carpeta actual: "<<QDir::currentPath();
            QStringList m0=gitUrl.split("/");
            QString urlFolder=m0.at(m0.length()-1);
            urlFolder=urlFolder.replace(".git", "");
            QString gitFolder="";
            gitFolder.append(QDir::currentPath());
            gitFolder.append("/");
            gitFolder.append(urlFolder);
            bool d = u.downloadGit(gitUrl.toUtf8(), gitFolder.toUtf8());
            QDir::setCurrent(urlFolder);
            qDebug()<<"2 asdCarpeta actual: "<<QDir::currentPath();
            mainLocation="";
            mainLocation.append(QDir::currentPath());
            modulesPath.append(mainLocation);
            modulesPath.append("/modules");
            engine.addImportPath(modulesPath);
            mainLocation.append("/main.qml");
        }else{
            QString gitUrl="";
            gitUrl.append("https://github.com/nextsigner/qmloaderdoc");
            gitUrl=gitUrl.replace("\n", "");

            qDebug()<<"Contenido de archivo qmloader.git: "<<gitUrl;
            QDir::setCurrent("../");
            qDebug()<<"Carpeta actual: "<<QDir::currentPath();
            QStringList m0=gitUrl.split("/");
            QString urlFolder=m0.at(m0.length()-1);
            urlFolder=urlFolder.replace(".git", "");
            QString gitFolder="";
            gitFolder.append(QDir::currentPath());
            gitFolder.append("/");
            gitFolder.append(urlFolder);
            bool d = u.downloadGit(gitUrl.toUtf8(), gitFolder.toUtf8());
            QDir::setCurrent(urlFolder);
            qDebug()<<"2 asdCarpeta actual: "<<QDir::currentPath();
            if(d){
                mainLocation="";
                mainLocation.append(QDir::currentPath());
                modulesPath.append(mainLocation);
                modulesPath.append("/modules");
                engine.addImportPath(modulesPath);
                mainLocation.append("/main.qml");
            }else{
                mainLocation="";
                mainLocation.append("qrc:/main.qml");
            }
        }
    }

    QByteArray documentsPath;
    documentsPath.append(u.getPath(3).toUtf8());
    documentsPath.append("/qmloader");
    engine.rootContext()->setContextProperty("documentsPath", documentsPath);
    engine.rootContext()->setContextProperty("isDev", isDev);
    QmlClipboardAdapter clipboard;
    engine.rootContext()->setContextProperty("clipboard", &clipboard);
    engine.rootContext()->setContextProperty("engine", &engine);
    engine.rootContext()->setContextProperty("argtitle", argtitle);

    const QUrl url(mainLocation);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
