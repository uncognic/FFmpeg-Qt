#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handlePicker);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleConvert);
}
void MainWindow::handlePicker() {
    file = QFileDialog::getOpenFileName(
        this,
        "Select media to convert",
        QString(),
        "Videos (*.mp4 *.mov *.avi *.mkv *.mpg *.mpeg);;"
        "All Media (*.*)"
        );
    if (file.isEmpty()) {
        return;
    }
    ui->lineEdit->setText(file);

}

void MainWindow::handleConvert() {
    format = ui->comboBox_2->currentText();
    output = QFileInfo(file).path() + "/" + QFileInfo(file).completeBaseName() + "_converted." + format;
    if (file.isEmpty() || format.isEmpty() || output.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file or format selected.");
        return;
    }
    QProcess *ffmpeg = new QProcess(this);
    QStringList arguments;
    arguments << "-y"
         << "-i" << file
         << "-f" << format
         << output;
    std::cout << arguments.join(" ").toStdString() << std::endl;
    connect(ffmpeg, &QProcess::readyReadStandardError, this, [=]() {
        QByteArray data = ffmpeg->readAllStandardError();
        std::cout << data.toStdString();
    });

    ffmpeg->start("ffmpeg", arguments);
}

MainWindow::~MainWindow()
{
    delete ui;
}

