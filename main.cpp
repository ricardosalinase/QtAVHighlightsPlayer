#include "hltplayerctl.h"
#include <QApplication>
#include <QLoggingCategory>
#include <QDialog>
#include <QDir>
#include <QStringList>
#include <QList>
//recibe argumento de entrada lista de hlt::
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //set openGLES for this app
    a.setAttribute(Qt::AA_UseOpenGLES);
    qputenv("QT_OPENGL","angle");
    QList<highlightData>hdata;
    QString path;
    for(int i = 1; i < argc; i++){
        char *data = argv[i];
        QString str;
        str.sprintf("%s",data);
        if(QDir(str).isReadable())
            path = str;
        else{

            highlightData cData(path,"RUN",hltPar(str.split('-').first().toInt(),str.split('-').last().toInt()));
            qDebug()<<cData.path<<cData.realTime;
            hdata.push_back(cData);
        }
    }
    HltPlayerCtl playerCtl;
    playerCtl.setVideoList2(hdata);
    return a.exec();
}
