#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QGroupBox>
#include <QSettings>
#include <QMessageBox>

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);

    QString getWifeyMoocPath() const;
    void setWifeyMoocPath(const QString &path);

private slots:
    void browseForApp();
    void saveSettings();
    void resetToDefaults();

private:
    void setupUI();
    void loadSettings();

    QLineEdit *m_appPathEdit;
    QPushButton *m_browseButton;
    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;
    QPushButton *m_resetButton;

    QSettings *m_settings;

    // Colors
    struct Colors {
        QString bg = "#FFB6C1";
        QString secondary = "#FFC0CB";
        QString accent = "#FF69B4";
        QString text = "#8B008B";
        QString white = "#FFFFFF";
        QString button = "#FF1493";
        QString entry = "#FFEFD5";
    } m_colors;
};

#endif // CONFIGDIALOG_H
