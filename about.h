#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QApplication>
#include <QScreen>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QPalette>

class About : public QWidget {
    Q_OBJECT

public:
    About(QWidget *parent = nullptr);
    void readConfig();
    void setAppearance();
    void setUI();
    void setReleaseData();
    void setComponent();
    void setWindowGeometry();
    void setTransparency();
    ~About();

private:
    QStringList mArguments;

    QJsonObject mConfig;
    QString mReleaseData;

    QLabel* mComponentLabel;
    QLabel* mVersionLabel;

    QString mFontFamily;
};
#endif // ABOUT_H
