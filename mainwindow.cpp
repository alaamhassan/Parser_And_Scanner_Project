#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Parser.h"
#include "SCANNER.h"
#include <QFileDialog>
#include <QMessageBox>
#include "secdialog.h"
#include <vector>
#include <iostream>
using namespace std;
//string error_msg;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_uploadFile_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Choose a File", QDir::homePath(), "Text files (*.txt)");
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0, "info", file.errorString());
    QTextStream in(&file);
    ui->textBrowser->setText(in.readAll());
}


void MainWindow::on_generate_pushButton_clicked()
{
    if (ui->textBrowser->toPlainText() == "") {
        QMessageBox::information(0, "info", "Empty text!\nNo parse tree generated.");
    } else {
        //Parser_T* parser = new Parser_T();
        vector<SyntaxTree_T*> treeVector = generate_programm_tree(ui->textBrowser->toPlainText().toStdString());
        cout << ui->textBrowser->toPlainText().toStdString() << endl;
        if (is_error == 1) {
            string msg = "Syntax Error!\nNo parse tree generated.\n" + error_message;
            QMessageBox::information(0, "Error", QString::fromStdString(msg));
        } else {
            //generate_programm_tree(ui->textBrowser->toPlainText().toStdString());
            //cout << "generated Treeeeeeeee" << endl;
            SecDialog secdialog;
            secdialog.setModal(true);
            secdialog.exec();
        }
    }
}

