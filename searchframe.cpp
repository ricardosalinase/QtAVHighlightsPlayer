#include "searchframe.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QKeyEvent>
#include <QDebug>
SearchFrame::SearchFrame(QWidget *parent):
    BorderFrame(parent)
  , lupeButton(0)
  , searchButton(0)
  , lineEdit(0)
  , enabled(false)
{

    this->setBorder(4);
//    this->setStyleSheet("QFrame {"
//                            "background-color: rgb(26,26,26);"
//                        "}"
//    );
    this->setStyleSheet("QFrame {"
                            "background-color: blue;"
                        "}"
    );

    this->setFrameShape(StyledPanel);
    this->setFrameShadow(Raised);
    int itemsHeight = 51;

    lupeButton = new QPushButton(this);
    lupeButton->setCursor(Qt::PointingHandCursor);
    lupeButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    lupeButton->setFixedSize(itemsHeight,itemsHeight);
    lupeButton->setStyleSheet("QPushButton {"
                              "background-color: transparent;"
                              "border-image: url(:/resources/lupe-activo.png)}"
                              "QPushButton:hover { border-image: url(:/resources/lupe-activo.png)"
                             "}");
    lupeButton->setCursor(Qt::PointingHandCursor);
    lupeButton->setFlat(true);
    lupeButton->setCheckable(true);

    searchButton = new QPushButton(this);
    searchButton->setFont(QFont("Frutiger CE 45 Light",10));
    searchButton->setFlat(true);
    searchButton->setCursor(Qt::PointingHandCursor);
    searchButton->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed));
    searchButton->setMinimumWidth(127);
    searchButton->setFixedHeight(itemsHeight);
    searchButton->setText("Search Now");
    searchButton->setStyleSheet("QPushButton{"
                                "border-radius: 4px;"
                                "background-color: rgb(241,196,13);"
                                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0  rgb(191,155,10), stop: 1  rgb(254,216,58));"
                                "}"
                                "QPushButton::hover{"
                                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0  rgb(239,197,26), stop: 1  rgb(254,216,102));"
                                "}");
    lineEdit = new QLineEdit(this);
    lineEdit->setFont(QFont("Frutiger CE 45 Light",10));
    lineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
    lineEdit->setFixedHeight(itemsHeight);
    lineEdit->setStyleSheet("QLineEdit{"
                             "background-color: black;"
                             "color: white;"
                             "border-radius: 10px;"
                             "border-style: outset;"
                             "border-width: 8px;"
                             "}");

    lineEdit->setClearButtonEnabled(true);
    lineEdit->setPlaceholderText("What are you looking for?");
    lineEdit->setFocusPolicy(Qt::StrongFocus);
//    lineEdit->
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(14);
    layout->setMargin(6);

    layout->addWidget(lupeButton, 0, Qt::AlignLeft);
    layout->addWidget(lineEdit);
    layout->addWidget(searchButton);
    this->setLayout(layout);

    lineEdit->hide();
    searchButton->hide();
    setupSearch(true);
    this->toggleSearchBar(false);
}

void SearchFrame::setupSearch(bool active)
{
    if(active && !enabled)
    {
        connect(lupeButton, SIGNAL(clicked(bool)), lineEdit, SLOT(setVisible(bool)));
        connect(lupeButton, SIGNAL(clicked(bool)), searchButton, SLOT(setVisible(bool)));
        connect(lupeButton, SIGNAL(clicked(bool)), this, SLOT(toggleSearchBar(bool)));
        connect(searchButton, SIGNAL(clicked()),this,SLOT(sendDataText()));
        connect(lineEdit,SIGNAL(textChanged(QString)),this,SLOT(checkCleared(QString)));
        enabled = true;

    }

    if(!active && enabled){
        disconnect(lupeButton, SIGNAL(clicked(bool)), lineEdit, SLOT(setVisible(bool)));
        disconnect(lupeButton, SIGNAL(clicked(bool)), searchButton, SLOT(setVisible(bool)));
        disconnect(lupeButton, SIGNAL(clicked(bool)), this, SLOT(toggleSearchBar(bool)));
        disconnect(searchButton, SIGNAL(clicked()),this,SLOT(sendDataText()));
        disconnect(lineEdit,SIGNAL(textChanged(QString)),this,SLOT(checkCleared(QString)));
        enabled = false;
    }

}

int SearchFrame::getWidthWithOutSearchButton()
{
    return (this->width() - searchButton->width() - 8);
}

void SearchFrame::setVisibleSearchMode(bool visible)
{
    lineEdit->setVisible(visible);
    searchButton->setVisible(visible);
    toggleSearchBar(visible);
    lupeButton->setChecked(visible);
}

void SearchFrame::checkCleared(const QString &text){
    if(text == ""){
        emit backNoSearch();
    }
}

QString SearchFrame::getLineEditValue()
{
    if(lineEdit->isVisible())
        return this->lineEdit->text();
    else
        return "";
}

void SearchFrame::toggleSearchBar(bool toggle)
{
    if(toggle){
        this->setStyleSheet("QFrame {"
                            "background-color: rgb(26,26,26);"
                            "}"
                            );
        lineEdit->setFocus();
    }
    else{
        this->setStyleSheet("QFrame {"
                            "background-color: transparent;"
                            "}"
                            );
        lineEdit->clearFocus();

    }
    emit showSuggestion(toggle);

}

void SearchFrame::sendDataText()
{
    emit searchButtonClicked(this->getLineEditValue());
}
