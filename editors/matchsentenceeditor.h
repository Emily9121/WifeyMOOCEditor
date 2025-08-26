/*
 * File: matchsentenceeditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our lovely sentence-matching editor!
 * I've given it its own special purse (a QJsonObject member variable)
 * and all the right names so it's not confused anymore! <3
 */

#ifndef MATCHSENTENCEEDITOR_H
#define MATCHSENTENCEEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QJsonObject> // The header for our purse!

class MatchSentenceEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit MatchSentenceEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private:
    // Helper functions to keep the UI beautiful!
    void refreshSentencesUI();
    void addSentence();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_sentencesLayout;

    // The brand new purse for our sentence editor!
    QJsonObject m_currentQuestion;
};

#endif // MATCHSENTENCEEDITOR_H
