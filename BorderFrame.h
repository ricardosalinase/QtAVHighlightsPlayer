#ifndef BORDERFRAME_H
#define BORDERFRAME_H

#include <QFrame>

/*!
 \brief

*/
class BorderFrame : public QFrame
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    BorderFrame(QWidget *parent = 0);
    /*!
     \brief

     \param value
    */
    void setBorder(int value);

protected:
    /*!
     \brief

     \param e
    */
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
private:
    int border; /*!< TODO: describe */
};

#endif // BORDERFRAME_H
