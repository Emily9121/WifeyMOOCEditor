/*
* File: mcqsingleeditor.h
* Author: Emily
* 
* Description:
* The COMPLETE header file for our very first and most adorable editor! 
* Now with full support for both string options and image+text options,
* just like the Python version! So versatile and cute! 💖
*/

#ifndef MCQSINGLEEDITOR_H
#define MCQSINGLEEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class MCQSingleEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit MCQSingleEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addOption();
    void browseImage(QLineEdit* imageEdit);
    void browseMedia();

private:
    void createOptionRow(bool correct = false, const QString &text = "", const QString &image = "");
    void clearOptions();

    // UI Elements  
    QTextEdit *questionPromptEdit;
    QVBoxLayout *optionsLayout;
    QLineEdit *mediaEdit;
    QComboBox *mediaTypeCombo;

    // Data storage
    QJsonObject m_currentQuestion;
    QList<QWidget*> optionWidgets;
};

#endif // MCQSINGLEEDITOR_H
