#include "unikqprocess.h"

UnikQProcess::UnikQProcess(QObject *parent) : QProcess(parent)
{
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(logOutProcess()));
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(logOutProcessErr()));
}

void UnikQProcess::run(const QByteArray cmd)
{
    run(cmd, false);
}

void UnikQProcess::run(const QByteArray cmd, bool detached)
{
    if(!detached){
         start(cmd);
    }else{
        startDetached(cmd);
    }
}

void UnikQProcess::logOutProcess()
{
    //QString data;
    //data.append(this->readAllStandardOutput());
    //qDebug()<<"------------>"<<data;
    setLogData(this->readAllStandardOutput());
}
void UnikQProcess::logOutProcessErr()
{
    setLogData(this->readAllStandardError());
}


