#include "mainwindow.h"
#include ".\ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handlePicker);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleConvert);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::handleAbout);
}

void MainWindow::handleAbout() {
    QMessageBox::information(this, "FFmpeg-Qt", "Simple Qt frontend for converting files using FFmpeg.\nuncognic\nMIT License");
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
   
    QString ffmpegFolder = QCoreApplication::applicationDirPath() + "/ffmpeg/";

#ifdef Q_OS_WIN
    QString ffmpegPath = ffmpegFolder + "ffmpeg.exe";
#else
    QString ffmpegPath = ffmpegFolder + "ffmpeg";
#endif

    if (!QFile::exists(ffmpegPath)) {
        QMessageBox::warning(this, "FFmpeg not found",
            "FFmpeg was not found in the ffmpeg folder.\n\n"
            "Please place the FFmpeg binary here:\n" + ffmpegPath);
        return;
    }

    
    format = ui->comboBox_2->currentText().toLower();
    output = QFileInfo(file).path() + "/" + QFileInfo(file).completeBaseName() + "_converted." + format;
    if (file.isEmpty() || format.isEmpty() || output.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file or format selected");
        return;
    }
    QProcess *ffmpeg = new QProcess(this);
    ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    QStringList arguments;
    arguments << "-y"
         << "-i" << file
         << output;
    ui->textEdit->clear();
    connect(ffmpeg, &QProcess::readyRead, this, [=]() {
        QString text = ffmpeg->readAll();
        ui->textEdit->append(text);
        });
    std::cout << arguments.join(" ").toStdString() << std::endl;
    connect(ffmpeg, &QProcess::readyReadStandardError, this, [=]() {
        QByteArray data = ffmpeg->readAllStandardError();
        std::cout << data.toStdString();
    });

    ffmpeg->start(ffmpegPath, arguments);
}

MainWindow::~MainWindow()
{
    delete ui;
}

