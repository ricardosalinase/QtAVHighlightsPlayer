#ifndef SEARCHFRAME_H
#define SEARCHFRAME_H

#include "BorderFrame.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

/*!
 \brief

*/
class SearchFrame : public BorderFrame
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    SearchFrame(QWidget *parent = 0);
    QPushButton *lupeButton; /*!< TODO: describe */
    QPushButton *searchButton; /*!< TODO: describe */
    QLineEdit *lineEdit; /*!< TODO: describe */
    /*!
     \brief

     \return QString
    */
    QString getLineEditValue();

    /*!
     \brief

     \param active
    */
    void setupSearch(bool active);

    int getWidthWithOutSearchButton();
private:
    bool enabled; /*!< TODO: describe */

signals:
    /*!
     \brief

    */
    void lupeButtonClicked();
    /*!
     \brief

     \param QString
    */
    void searchButtonClicked(QString );
    /*!
     \brief

    */
    void backNoSearch();

    void showSuggestion(bool visible);
private slots:
    /*!
     \brief

     \param toggle
    */
    void toggleSearchBar(bool toggle);
    /*!
     \brief

    */
    void sendDataText();
    /*!
     \brief

     \param text
    */
    void checkCleared(const QString &text);
public slots:
    /*!
     \brief

     \param visible
    */
    void setVisibleSearchMode(bool visible);

};

#endif // SEARCHFRAME_H
