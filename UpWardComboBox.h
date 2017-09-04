#ifndef UPWARDCOMBOBOX_H
#define UPWARDCOMBOBOX_H

#include <QComboBox>
#include <QObject>

/*!
 \brief

*/
class upWardComboBox : public QComboBox
{
    Q_OBJECT

public:
    /*!
     \brief

     \param parent
    */
    upWardComboBox(QWidget* parent = 0);

    /*!
     \brief

    */
    void showPopup();

    /*!
     \brief

     \param e
    */
    void leaveEvent(QEvent *e);
signals:
    /*!
     \brief

    */
    void leave();
private slots:
    /*!
     \brief Aumenta en uno el indice actual del combobox de velocidad.

    */
    void upBoxIndex();

    /*!
     \brief Disminuye en uno el indice actual del combobox de velocidad.

    */
    void downBoxIndex();
};

#endif // UPWARDCOMBOBOX_H
