#include "about.h"

void About::readConfig() {
    QString homePath = QDir::homePath();
    QString configPath = homePath + "/.config/plainDE/config.json";

    QFile configReader(configPath);
    configReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data = configReader.readAll();
    configReader.close();
    mConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void About::setAppearance() {
    // Stylesheet
    QString stylesheetPath = "/usr/share/plainDE/styles/" + mConfig["theme"].toString();
    QFile stylesheetReader(stylesheetPath);
    stylesheetReader.open(QIODevice::Text | QIODevice::ReadOnly);
    QString stylesheet = stylesheetReader.readAll();
    stylesheetReader.close();
    this->setStyleSheet(stylesheet);

    // Font
    mFontFamily = mConfig["fontFamily"].toString();
    this->setFont(mFontFamily);
}

void About::setUI() {
    this->setWindowTitle("plainAbout");

    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* upperLayout = new QHBoxLayout();

    QVBoxLayout* appInfoLayout = new QVBoxLayout();

    mComponentLabel = new QLabel("plainDE");
    mComponentLabel->setFont(QFont(mFontFamily, 25));
    appInfoLayout->addWidget(mComponentLabel);

    mVersionLabel = new QLabel("Version");
    mVersionLabel->setFont(QFont(mFontFamily, 12));
    appInfoLayout->addWidget(mVersionLabel);

    appInfoLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                 QSizePolicy::Ignored,
                                                 QSizePolicy::MinimumExpanding));

    upperLayout->addLayout(appInfoLayout);

    QLabel* logoLabel = new QLabel();
    logoLabel->setMaximumSize(120, 120);
    logoLabel->setScaledContents(true);
    logoLabel->setPixmap(QPixmap("/usr/share/plainDE/menuIcon.png"));
    upperLayout->addWidget(logoLabel);

    layout->addLayout(upperLayout);

    QTextEdit* additionalInfoTextEdit = new QTextEdit();
    additionalInfoTextEdit->setFont(QFont(mFontFamily, 11));
    additionalInfoTextEdit->setHtml("plainDE was brought to you by <b>thm-unix</b>.<br>"
                                    "Found bug/want to help us/have an idea? Leave feedback via GitHub Issues.<br><br>"
                                    "<a style=\"color:#378edf;\" href=\"https://plainDE.github.io/\">https://plainDE.github.io/</a><br>"
                                    "<a style=\"color:#378edf;\" href=\"https://github.com/plainDE/\">https://github.com/plainDE/</a><br>"
                                    "<a style=\"color:#378edf;\" href=\"https://github.com/thm-unix/\">https://github.com/thm-unix/</a><br><br>"
                                    "<a style=\"color:#378edf;\" href=\"https://www.gnu.org/licenses/gpl-3.0.html\">Learn more about GNU General Public License v3...</a><br>"
                                    "2022-2024");
    additionalInfoTextEdit->setReadOnly(true);
    layout->addWidget(additionalInfoTextEdit);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    buttonsLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));

    QPushButton* okButton = new QPushButton("OK");
    okButton->setFont(QFont(mFontFamily));
    buttonsLayout->addWidget(okButton);

    layout->addLayout(buttonsLayout);

    connect(okButton, &QPushButton::clicked, this, []() {
        exit(0);
    });
}

void About::setReleaseData() {
    QFile releaseFile("/usr/share/plainDE/release_data");
    releaseFile.open(QIODevice::Text | QIODevice::ReadOnly);
    QString releaseData = releaseFile.readAll();
    releaseFile.close();
    mVersionLabel->setText(releaseData);
}

void About::setComponent() {
    if (mArguments.contains("--plainPanel")) {
        mComponentLabel->setText("plainPanel");
    }
    else if (mArguments.contains("--plainControlCenter")) {
        mComponentLabel->setText("plainControlCenter");
    }
}

void About::setWindowGeometry() {
    QScreen* primaryScreen = qApp->primaryScreen();
    QRect screenGeometry = primaryScreen->geometry();
    int width = 550, height = 360;
    int x = (screenGeometry.width() - width) / 2, y = (screenGeometry.height() - height) / 2;
    if (mArguments.contains("-x")) {
        int xIndex = mArguments.indexOf("-x") + 1;
        x = mArguments.at(xIndex).toInt();
    }
    if (mArguments.contains("-y")) {
        int yIndex = mArguments.indexOf("-y") + 1;
        y = mArguments.at(yIndex).toInt();
    }
    this->setGeometry(x, y, width, height);
}

void About::setTransparency() {
    QScreen* screen = this->screen();
    QRect widgetGeometry = this->geometry();
    QPixmap pixmap = screen->grabWindow(0,
                                        widgetGeometry.x(),
                                        widgetGeometry.y(),
                                        widgetGeometry.width(),
                                        widgetGeometry.height());
    QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect();
    blurEffect->setBlurRadius(15);
    blurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);

    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem item;
    item.setPixmap(pixmap);
    item.setGraphicsEffect(blurEffect);
    scene->addItem(&item);
    QImage res(QSize(widgetGeometry.width(), widgetGeometry.height()), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene->render(&ptr, QRectF(), QRectF(0, 0, widgetGeometry.width(), widgetGeometry.height()));

    QPalette palette;
    palette.setBrush(this->backgroundRole(),
                     QBrush(QPixmap::fromImage(res)));
    this->setPalette(palette);
}

About::About(QWidget* parent) : QWidget(parent) {
    mArguments = QCoreApplication::arguments();
    readConfig();
    setAppearance();
    setUI();
    setReleaseData();
    setComponent();
    setWindowGeometry();
    if (mConfig["theme"].toString().contains("transparent")) {
        setTransparency();
    }
}

About::~About() {

}
