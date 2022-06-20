#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QString>

#include "about.h"
#include "ui_about.h"

QJsonObject config;
QString releaseData;


void readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    config = QJsonDocument::fromJson(data.toUtf8()).object();
}

void readReleaseData() {
    QFile releaseFile;
    releaseFile.setFileName("/usr/share/plainDE/release_data");
    releaseFile.open(QIODevice::ReadOnly | QIODevice::Text);
    releaseData = releaseFile.readAll();
    releaseFile.close();
}

about::about(QWidget *parent) : QWidget(parent), ui(new Ui::about) {
    ui->setupUi(this);

    readConfig();
    readReleaseData();

    this->setWindowTitle("About plainDE");
    QStringList args = QCoreApplication::arguments();
    if (args.contains("--plainPanel")) {
        ui->appNameLabel->setText("plainPanel");
    }
    else if (args.contains("--plainControlCenter")) {
        ui->appNameLabel->setText("plainControlCenter");
    }
    ui->versionLabel->setText(releaseData);
    ui->logoLabel->setPixmap(QPixmap("/usr/share/plainDE/menuIcon.png"));

    QString stylesheetPath = "/usr/share/plainDE/styles/" + config["theme"].toString();
    QFile stylesheetReader(stylesheetPath);
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    this->setStyleSheet(styleSheet.readAll());
}

about::~about() {
    delete ui;
}

