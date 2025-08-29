/*
* File: wordfilleditor.h
* Author: Emily
*
* Description:
* The COMPLETE header file for our word-filling editor!
* Now fully functional with sentence parts and answers management,
* just like our perfect Python version! 💖
*/

#ifndef WORDFILLEDITOR_H
#define WORDFILLEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class WordFillEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit WordFillEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addSentencePart();
    void addAnswer();
    void browseMedia();

private:
    void refreshPartsUI();
    void refreshAnswersUI();
    void clearParts();
    void clearAnswers();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_partsLayout;
    QVBoxLayout* m_answersLayout;
    QLineEdit* m_mediaEdit;
    QComboBox* m_mediaTypeCombo;

    // 💖 ADDED: UI elements for our lesson PDF! So cute! 💖
    QLineEdit* m_lessonPdfEdit;
    QPushButton* m_lessonPdfButton;

    // Data storage
    QJsonObject m_currentQuestion;
    QList<QWidget*> m_partsWidgets;
    QList<QWidget*> m_answersWidgets;
};

#endif // WORDFILLEDITOR_H
