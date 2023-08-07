#include "secdialog.h"
#include "ui_secdialog.h"
#include <QImageReader>
#include <QPixmap>

SecDialog::SecDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Parse Tree");
    QImageReader reader("dotLang.dot.png");
    QSize imageSize = reader.size();
    int imageHeight = imageSize.height();
    int imageWidth = imageSize.width();
    QPixmap p(".\\dotLang.dot.png");
    QSize* windowSize = new QSize();
    windowSize->setHeight(imageHeight + 20);
    windowSize->setWidth(imageWidth + 40);
    resize(*windowSize);
    ui->label->setMinimumHeight(imageHeight);
    ui->label->setMinimumWidth(imageWidth);
    ui->label->setPixmap(p);
}

SecDialog::~SecDialog()
{
    delete ui;
}
