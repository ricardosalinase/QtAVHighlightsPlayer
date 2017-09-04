#include "nicexml.h"

#include <QMessageBox>
#define NULL_ROOT_TAGNAME "NULL_ROOT_TAGNAME"


niceXml::niceXml(QObject *parent):
    QObject(parent)
{
    modified = false;
    m_readOK = false;
    m_rootTagName = NULL_ROOT_TAGNAME;
}

niceXml::niceXml(QString filename, QObject *parent, QString rootTagName):
    QObject(parent)
  , m_rootTagName(rootTagName)
{
    modified = false;
    m_rootTagName = rootTagName;
    if(!openFile(filename)){
        qDebug()<<"xmlReader::Failed to set xml Reader";
        m_readOK = false;
        //QMessageBox messageBox;
        if(file.size() == 0){
            qDebug()<<"xmlReader::Remove empty file";
            //messageBox.critical(0,"niceXml::openFile","Removing empty xml file");
            //messageBox.setFixedSize(500,200);
            file.remove();
        }
    }else{
        m_readOK = true;
        qDebug()<<"xmlReader::Read File OK "<<QFileInfo(file).absoluteFilePath();
    }
}

niceXml::~niceXml()
{
//    file.close();
    qDebug()<<"destroy"<<QDir(file.fileName()).dirName()<<"modified:"<<modified;
    if(QDir(file.fileName()).dirName() == "tags.xml" && modified ){
        qDebug()<<"ABRIR ARCHIVO CULIAO";
        QMessageBox messageBox;
        if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug()<< "xmlReader::Failed to open file or not modified";
            messageBox.critical(0,"niceXml::openFile",file.fileName()+"fail open");
            messageBox.setFixedSize(500,200);
        }else{
            QTextStream stream(&file);
            stream << document.toString();
            file.flush();
            file.close();
        }
    }
    document.clear();
}

bool niceXml::openFile(QString filename)
{
    QMessageBox messageBox;

    qDebug()<<"niceXml "<<filename;
    file.setFileName(filename);
    bool exist = file.exists();

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        messageBox.critical(0,"niceXml::openFile",filename+"fail open");
        messageBox.setFixedSize(500,200);
        qDebug()<< "xmlReader::Failed to open file";
        return false;
    }else
    {
        QString errorMsg;
        int errorLine=0;
        int errorColumn=0;
        if(!document.setContent(&file,false,&errorMsg,&errorLine,&errorColumn))
        {
            if(exist){
                messageBox.critical(0,"niceXml::openFile",filename+' '+errorMsg+' '+errorLine+' '+errorColumn);
                messageBox.setFixedSize(500,200);
            }
            qDebug()<< "xmlReader::Failed to load document";
            file.close();
            return false;
        }
        file.close();
    }

    setFilename(filename);

    if(m_rootTagName != NULL_ROOT_TAGNAME)
    {
        QDomElement root = document.firstChildElement();
        if(root.tagName() == m_rootTagName)
            return true;
        else
            return false;
    }else
        return true;

    return true;
}

QString niceXml::getXmlFilePath()
{
    if(file.exists()){
        QFileInfo fi(file);
        return fi.filePath();
    }
    return QString();
}

void niceXml::listAllElements(QDomElement root)
{
    qDebug() << "List of all elements in node "<< root.attribute("id");
    QDomNodeList items = root.childNodes();
    qDebug()<<"Total Items = "<< items.count();

    for(int i = 0; i <  items.count() ;  i++)
    {
        QDomNode itemnode = items.at(i);
        //Convert to element
        if(itemnode.isElement())
        {
            QDomElement itemele = itemnode.toElement();
            qDebug() << "Nodo: " << itemele.text();
        }
    }
}

bool niceXml::getReadOK() const
{
    return m_readOK;
}
QString niceXml::rootTagName() const
{
    return m_rootTagName;
}

QDomNodeList niceXml::getXmlData(QString tagName){

    QDomElement root = document.firstChildElement();
    qDebug()<<"get "<<tagName<<"childs of "<<root.tagName();
    return root.elementsByTagName(tagName);
}

QStringList niceXml::getChildsListFromParentId(QString tagName,QString id){

    qDebug()<<"GENERAR LIST DE CHILDS DE"<<id<<"con root"<<tagName;
    QStringList ret;
    QDomNodeList matches = getChildsFromParentId(tagName,id);
    for(int i =0; i < matches.count(); i++){
        qDebug()<<matches.at(i).toElement().attribute("id");
        ret<<matches.at(i).toElement().attribute("id");
    }
    return ret;
}
QStringList niceXml::getLabelListFromElement(QString elemName){
    QDomElement elem2 = document.firstChildElement();

    qDebug()<<"tag a buscar hijos"<<elem2.tagName();
    QDomElement elem = elem2.firstChildElement(elemName);
    qDebug()<<"tag a buscar hijos"<<elem.tagName();

    QDomNodeList items = elem.childNodes();
    QStringList ret;
    qDebug()<<"Total Items = "<< items.count();

    for(int i = 0; i <  items.count() ;  i++)
    {
        QDomElement itemnode = items.at(i).toElement();
        qDebug()<<elemName<<i<<itemnode.attribute("id");
        ret << itemnode.attribute("id");
    }
    return ret;
}

QStringList niceXml::getChildPropertiesById(QString root, QString tagName)
{
    qDebug()<<"BUSCAR METRICAS ESPECIFICAS DE"<<tagName<<"con root"<<root;

    QDomElement rootElem = document.firstChildElement().firstChildElement(root);
    QDomNodeList items = rootElem.childNodes();
    QStringList ret;

    for(int i = 0; i <  items.count() ;  i++)
    {
        QDomElement itemnode = items.at(i).toElement();
        if(itemnode.attribute("id") == tagName){
            QDomNodeList retList = itemnode.childNodes();
            for( int j = 0; j < retList.count();j++){
                ret<< retList.at(j).toElement().attribute("id");
            }
            return ret;
        }

    }
    return ret;

}

QStringList niceXml::getRetElemFromNodeByCompOtherElement(QString rootName, QString compelem, QString tag, QString retelem)
{
     QDomElement root = document.firstChildElement().firstChildElement(rootName);
     qDebug()<<"en"<<root.tagName()<<"buscar si hay match";

     QDomNodeList items = root.elementsByTagName(compelem);
     QStringList ret;
     qDebug()<<"Total Items = "<< items.count();
     for(int i = 0; i <  items.count() ;  i++)
     {
         QDomElement itemnode = items.at(i).toElement();
         if(itemnode.attribute("id") == tag){
            qDebug()<<"es padre";
            QDomNodeList retList = root.elementsByTagName(retelem);
            qDebug()<<"Total Items ret"<<retList.count();
            for(int j = 0; j < retList.count();j++)
                ret << retList.at(j).toElement().attribute("id");
            return ret;
         }
     }
     return QStringList();

}

void niceXml::setRootTagName(const QString &rootTagName)
{
    document.appendChild(document.createElement(rootTagName));
    m_rootTagName = rootTagName;
}


QString niceXml::getFilename() const
{
    return m_filename;
}

void niceXml::setFilename(const QString &value)
{
    m_filename = value;
}

QDomElement niceXml::getClassById(QDomElement root, QString id)
{
    QDomNodeList items = root.elementsByTagName("class");
    for(int i = 0; i <  items.count() ;  i++)
    {
        QDomNode itemnode = items.at(i);
        if(itemnode.isElement())
        {
            QDomElement itemele = itemnode.toElement();
            if(itemele.attribute("id") == id){
                return itemele;
            }
        }
    }
    return QDomElement();
}

QDomNodeList niceXml::getChildsFromParentId(QString tagName,QString id){

    qDebug()<<"BUSCAR LABELS ESPECIFICAS DE"<<tagName<<"con id"<<id;

    QDomElement root = document.firstChildElement();
    QDomNodeList items = root.elementsByTagName(tagName);
    for(int i = 0; i <  items.count() ;  i++)
    {
        QDomNode itemnode = items.at(i);
        if(itemnode.isElement())
        {
            QDomElement itemele = itemnode.toElement();

            if(itemele.attribute("id") == id){
                qDebug()<< "Clase encontrada: " << itemele.attribute("id");
                return itemele.childNodes();
            }
        }
    }
    return QDomNodeList();
}

QString niceXml::getClassTextElement(QDomElement node, QString tag)
{
    QDomNodeList items = node.elementsByTagName(tag);
    QString value = "NULL_PARAMETER";
    int i;
    for(i = 0; i <  items.count() ;  i++)
    {
        QDomNode itemnode = items.at(i);
        //Convert to element
        if(itemnode.isElement())
        {
            QDomElement itemele = itemnode.toElement();
            value = itemele.text();
        }
    }
    if(i > 0 && i <= 1)
        return value;
    else
        return "NULL_PARAMETER";
}

QDomElement niceXml::getClassElement(QDomElement node, QString tag)
{
//    qDebug() << "Get element "<< tag<<" for "<< node.attribute("id");
    QDomNodeList items = node.elementsByTagName(tag);
//    qDebug()<<"Total Items = "<< items.count();
    QDomElement value = QDomElement();
    int i;
    for(i = 0; i <  items.count() ;  i++)
    {
        QDomNode itemnode = items.at(i);
        //Convert to element
        if(itemnode.isElement())
        {
            QDomElement itemele = itemnode.toElement();
//            qDebug() << "Nodo: " << itemele.text();
            value = itemele;
        }
    }
    if(i > 0 && i <= 1)
        return value;
    else
        return QDomElement();
}

QDomNode niceXml::findElementByPath(const QString &path){
    QDomNodeList list = getXmlData("Clip");
    for(int i = 0; i < list.count(); i++){
        QDomNode elm = list.at(i);
        if(elm.toElement().attribute("path") == path){

            return elm;
        }

    }
    return QDomNode();
}
