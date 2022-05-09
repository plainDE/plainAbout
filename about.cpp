#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QString>

#include "about.h"
#include "ui_about.h"


QJsonObject config;

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


about::about(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::about)
{
    ui->setupUi(this);

    this->setWindowTitle("About plainDE");
    ui->versionLabel->setText("0.1.3");

    ui->logoLabel->setPixmap(QPixmap("/usr/share/plainDE/menuIcon.png"));

    readConfig();

    QString stylesheetPath = "/usr/share/plainDE/styles/" + config["theme"].toString();

    QFile stylesheetReader(stylesheetPath);
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    this->setStyleSheet(styleSheet.readAll());
}

about::~about()
{
    delete ui;
}

