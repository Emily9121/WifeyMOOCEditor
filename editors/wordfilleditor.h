/*
 * File: wordfilleditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our wonderful word-filling editor!
 * I've given it the very last purse we have (a QJsonObject member variable)
 * so it can remember all the words for Sierra's quizzes! <3
 */

#ifndef WORDFILLEDITOR_H
#define WORDFILLEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QJsonObject> // The header for our final purse!

class WordFillEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit WordFillEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private:
    // Helper functions to keep the UI looking fab!
    void refreshPartsUI();
    void refreshAnswersUI();
    void addPart();
    void addAnswer();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_partsLayout;
    QVBoxLayout* m_answersLayout;

    // The final purse for our last forgetful editor!
    QJsonObject m_currentQuestion;
};

#endif // WORDFILLEDITOR_H
