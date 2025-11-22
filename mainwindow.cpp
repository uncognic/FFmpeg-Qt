#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include "QFileDialog"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);
}
void MainWindow::handleButton() {
    QString file = QFileDialog::getOpenFileName(
        this,
        "Select media to convert",
        QString(),
        "Videos (*.mp4 *.mov *.avi *.mkv *.mpg *.mpeg);;"
        "Audio (*.mp3 *.wav *.aac *.flac *.ogg *.opus);;"
        "All Media (*.*)"
        );
    if (file.isEmpty()) {
        return;
    }
    ui->lineEdit->setText(file);

}

MainWindow::~MainWindow()
{
    delete ui;
}

