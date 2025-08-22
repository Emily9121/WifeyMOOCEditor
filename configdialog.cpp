#include "configdialog.h"
#include <QApplication>

ConfigDialog::ConfigDialog(QWidget *parent)
    : QDialog(parent)
    , m_settings(new QSettings(this))
{
    setWindowTitle("⚙️ WifeyMOOC Editor Settings ⚙️");
    setModal(true);
    resize(500, 200);

    setupUI();
    loadSettings();
}

void ConfigDialog::setupUI()
{
    // Apply pink theme
    setStyleSheet(QString(
        "QDialog { background-color: %1; }"
        "QGroupBox { "
        "   font-weight: bold; "
        "   border: 2px solid %2; "
        "   border-radius: 5px; "
        "   margin-top: 1ex; "
        "   color: %3; "
        "}"
        "QGroupBox::title { "
        "   subcontrol-origin: margin; "
        "   left: 10px; "
        "   padding: 0 5px 0 5px; "
        "}"
        "QPushButton { "
        "   background-color: %4; "
        "   color: black; "
        "   border: 2px solid %2; "
        "   border-radius: 5px; "
        "   padding: 8px 16px; "
        "   font-weight: bold; "
        "   min-width: 80px; "
        "}"
        "QPushButton:hover { background-color: %2; }"
        "QLineEdit { "
        "   background-color: %5; "
        "   border: 1px solid %2; "
        "   border-radius: 3px; "
        "   padding: 5px; "
        "}"
        "QLabel { color: %3; }"
    ).arg(m_colors.bg, m_colors.accent, m_colors.text, m_colors.button, m_colors.entry));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Title
    QLabel *titleLabel = new QLabel("💖 Configure WifeyMOOC Editor Settings 💖", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(titleLabel);

    // Application path group
    QGroupBox *pathGroup = new QGroupBox("🚀 WifeyMOOC Application Path", this);
    QVBoxLayout *pathLayout = new QVBoxLayout(pathGroup);

    QLabel *pathLabel = new QLabel("💡 Set the path to the WifeyMOOC application executable:", this);
    pathLabel->setWordWrap(true);
    pathLayout->addWidget(pathLabel);

    QHBoxLayout *pathInputLayout = new QHBoxLayout();
    m_appPathEdit = new QLineEdit(this);
    m_appPathEdit->setPlaceholderText("e.g., /Applications/WifeyMOOC.app/Contents/MacOS/WifeyMOOC");
    m_browseButton = new QPushButton("📁 Browse", this);

    pathInputLayout->addWidget(m_appPathEdit);
    pathInputLayout->addWidget(m_browseButton);
    pathLayout->addLayout(pathInputLayout);

    mainLayout->addWidget(pathGroup);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_saveButton = new QPushButton("💖 Save", this);
    m_cancelButton = new QPushButton("❌ Cancel", this);
    m_resetButton = new QPushButton("🔄 Reset to Defaults", this);

    m_saveButton->setDefault(true);

    buttonLayout->addWidget(m_resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(m_browseButton, &QPushButton::clicked, this, &ConfigDialog::browseForApp);
    connect(m_saveButton, &QPushButton::clicked, this, &ConfigDialog::saveSettings);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_resetButton, &QPushButton::clicked, this, &ConfigDialog::resetToDefaults);
}

void ConfigDialog::loadSettings()
{
    QString appPath = m_settings->value("wifeymooc_path", "").toString();
    m_appPathEdit->setText(appPath);
}

void ConfigDialog::browseForApp()
{
    QString filter;
#ifdef Q_OS_WIN
    filter = "Executable files (*.exe);;All files (*.*)";
#elif defined(Q_OS_MAC)
    filter = "Application bundles (*.app);;Executable files (*);;All files (*.*)";
#else
    filter = "Executable files (*);;All files (*.*)";
#endif

    QString fileName = QFileDialog::getOpenFileName(
        this,
        "💖 Select WifeyMOOC Application 💖",
        QString(),
        filter
    );

    if (!fileName.isEmpty()) {
#ifdef Q_OS_MAC
        // On macOS, if user selected .app bundle, find the actual executable
        if (fileName.endsWith(".app")) {
            QString appName = QFileInfo(fileName).baseName();
            QString executablePath = fileName + "/Contents/MacOS/" + appName;
            if (QFile::exists(executablePath)) {
                fileName = executablePath;
            }
        }
#endif
        m_appPathEdit->setText(fileName);
    }
}

void ConfigDialog::saveSettings()
{
    QString appPath = m_appPathEdit->text().trimmed();

    if (appPath.isEmpty()) {
        QMessageBox::warning(this, "Invalid Path", 
            "Please specify the path to the WifeyMOOC application, sweetie! 💕");
        return;
    }

    if (!QFile::exists(appPath)) {
        int ret = QMessageBox::question(this, "File Not Found", 
            "The specified file does not exist. Do you want to save this path anyway?\n\n" + appPath,
            QMessageBox::Yes | QMessageBox::No);

        if (ret != QMessageBox::Yes) {
            return;
        }
    }

    m_settings->setValue("wifeymooc_path", appPath);

    QMessageBox::information(this, "Settings Saved! 💖", 
        "Your settings have been saved successfully, darling!");

    accept();
}

void ConfigDialog::resetToDefaults()
{
    int ret = QMessageBox::question(this, "Reset Settings", 
        "Are you sure you want to reset all settings to defaults?",
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        m_appPathEdit->clear();
        QMessageBox::information(this, "Reset Complete! 🔄", 
            "Settings have been reset to defaults, babe!");
    }
}

QString ConfigDialog::getWifeyMoocPath() const
{
    return m_appPathEdit->text().trimmed();
}

void ConfigDialog::setWifeyMoocPath(const QString &path)
{
    m_appPathEdit->setText(path);
}
