/*
 * File: multiquestionseditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our amazing multi-question editor!
 * I've given it the last purse we had (a QJsonObject member variable)
 * so it can hold all the nested questions for Sierra's quizzes! <3
 */

#ifndef MULTIQUESTIONSEDITOR_H
#define MULTIQUESTIONSEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QJsonObject> // The header for our final purse!

class MultiQuestionsEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit MultiQuestionsEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private:
    // UI Elements
    QTextEdit* m_jsonTextEdit;

    // The very last purse for our final forgetful editor!
    QJsonObject m_currentQuestion;
};

#endif // MULTIQUESTIONSEDITOR_H
