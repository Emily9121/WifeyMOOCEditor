/*
* File: mcqmultipleeditor.h
* Author: Emily
*
* Description:
* The COMPLETE header file for our multiple-choice, multiple-answer editor!
* Students can pick many adorable options! ☑️💖
*/

#ifndef MCQMULTIPLEEDITOR_H
#define MCQMULTIPLEEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class McqMultipleEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit McqMultipleEditor(QWidget *parent = nullptr);
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
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_optionsLayout;
    QLineEdit* m_mediaEdit;
    QComboBox* m_mediaTypeCombo;

    // 💖 ADDED: UI elements for our lesson PDF! So cute! 💖
    QLineEdit* m_lessonPdfEdit;
    QPushButton* m_lessonPdfButton;

    // Data storage
    QJsonObject m_currentQuestion;
    QList<QWidget*> m_optionWidgets;
};

#endif // MCQMULTIPLEEDITOR_H
