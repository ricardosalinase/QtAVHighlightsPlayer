#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H

#include <QPushButton>

/*!
 \brief Custom Button with border added in paint method
*/
class CustomPushButton : public QPushButton
{
    Q_OBJECT

public:
    /*!
     \brief

     \param parent
    */
    explicit CustomPushButton(QWidget *parent = 0);

    void setBorder(int value);

private:
    int border; /*!< TODO: describe */

protected:

    /*!
     \brief

     \param event
    */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // CUSTOMPUSHBUTTON_H
