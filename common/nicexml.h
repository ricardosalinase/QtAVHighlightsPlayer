#ifndef XMLREADER_H
#define XMLREADER_H

#include <QtXml>
#include <QDebug>
#include <QObject>
/*
 *  XML framework for nice xmls
 *  classes are identified by 'id' attribute in root nodes and have setting childs
 *  settings values are a text element or <name,value> pair with childs
 */
class niceXml : public QObject
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    niceXml(QObject *parent = 0);

    /*!
     \brief

     \param filename
     \param parent
     \param rootTagName
    */
    niceXml(QString filename, QObject *parent = 0, QString rootTagName = "NULL_ROOT_TAGNAME");

    /*!
     \brief

    */
    ~niceXml();
    /*!
     \brief

     \param filename
     \return bool
    */
    bool openFile(QString filename);

    QDomDocument document; /*!< TODO: describe */

    /*!
     \brief

     \return QString
    */
    QString getFilename() const;
    /*!
     \brief

     \param value
    */
    void setFilename(const QString &value);

    /*!
     \brief

     \param root
     \param id
     \return QDomElement
    */
    QDomElement getClassById(QDomElement root, QString id);
    /*!
     \brief

     \param node
     \param tag
     \return QString
    */
    QString getClassTextElement(QDomElement node, QString tag);
    /*!
     \brief

     \param node
     \param tag
     \return QDomElement
    */
    QDomElement getClassElement(QDomElement node, QString tag);

    //print methods
    /*!
     \brief

     \param root
     \param tagname
     \param attribute
    */
    void listElementsByAttribute(QDomElement root, QString tagname, QString attribute);
    /*!
     \brief

     \param root
    */
    void listAllElements(QDomElement root);


    /*!
     \brief

     \return bool
    */
    bool getReadOK() const;
    /*!
     \brief

     \return QString
    */
    QString rootTagName() const;
    /*!
     \brief

     \param rootTagName
    */
    void setRootTagName(const QString &rootTagName);

    /*!
     \brief

     \param tagName
     \return QDomNodeList
    */
    QDomNodeList getXmlData(QString tagName);
    /*!
     \brief

     \param QString
     \param QString
     \param QString
     \param QString
     \return QStringList
    */
    QStringList getRetElemFromNodeByCompOtherElement(QString,QString,QString,QString);
    /*!
     \brief

     \param elemName
     \return QStringList
    */
    QStringList getLabelListFromElement(QString elemName);
    /*!
     \brief

     \param root
     \param tagName
     \return QStringList
    */
    QStringList getChildPropertiesById(QString root,QString tagName);
    //    QStringList get    
    /*!
     \brief

     \param tagName
     \param id
     \return QDomNodeList
    */
    QDomNodeList getChildsFromParentId(QString tagName, QString id);
    /*!
     \brief

     \param tagName
     \param id
     \return QStringList
    */
    QStringList getChildsListFromParentId(QString tagName, QString id);
//    QDomNode findSingleTag(const unsigned int &tagID, QDomNode elm);
    /*!
     \brief

     \param path
     \return QDomNode
    */
    QDomNode findElementByPath(const QString &path);
    /*!
     \brief

     \return QString
    */
    QString getXmlFilePath();

signals:
    /*!
     \brief

     \param QDomElement
    */
    void nodeAdded_niceClips(QDomElement);
    /*!
     \brief

     \param QString
     \param tag
    */
    void nodeRemoved_niceXml(QString,unsigned int tag = 0);
    /*!
     \brief
     \param QString
     \param int
     \param QString
     \param QVariant
    */
    void nodeModified_niceXml(QString,unsigned int ,QString , QVariant);
private:

    bool modified ; /*!< TODO: describe */
    QFile file; /*!< TODO: describe */
    QString m_filename; /*!< TODO: describe */
    bool m_readOK; /*!< TODO: describe */
    QString m_rootTagName; /*!< TODO: describe */
    bool prevent; /*!< TODO: describe */
};

#endif // XMLREADER_H
