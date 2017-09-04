#ifndef SLICEFRAME_H
#define SLICEFRAME_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QPushButton;
class QSpacerItem;
QT_END_NAMESPACE

/*!
 \brief

*/
class sliceFrame : public QFrame
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    sliceFrame(QWidget *parent = 0);

    /*!
     \brief

     \param w
     \param useMoveButtons
    */
    void insertCenterWidget(QWidget *w, bool useMoveButtons = true);

    /*!
     \brief

     \return QWidget
    */
    QWidget *getMyWidget() const;

    QPushButton *showButton; /*!< TODO: describe */

    /*!
     \brief

    */
    void removeCenterWidget();
    /*!
     \brief

     \param isDisabled
    */
    void disableArrows(bool isDisabled);
public slots:
    /*!
     \brief

     \param checked
    */
    void showButtonClicked(bool checked);

signals:

    /*!
     \brief

     \param isBackward
    */
    void buttonClicked(bool isBackward);

private slots:

    /*!
     \brief

    */
    void rightButtonClicked();
    /*!
     \brief

    */
    void leftButtonClicked();
private:

    /*!
     \brief

    */
    enum Zone{
        leftzone,
        rightzone,
        centerzone
    };

    QPushButton *leftButton; /*!< TODO: describe */
    QPushButton *rightButton; /*!< TODO: describe */

    QWidget *myWidget; /*!< TODO: describe */

    /*!
     \brief

     \param rectSize
     \param czone
     \param def
     \return QIcon
    */
    QIcon getButtonIcon(QSize rectSize, Zone czone, bool def = false);
};

#endif // SLICEFRAME_H
