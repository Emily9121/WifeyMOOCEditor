/*
 * File: matchphraseseditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our super-smart phrase-matching editor!
 * I've given it a lovely new purse (a QJsonObject member variable)
 * so it can remember all the matching pairs for Sierra! <3
 */

#ifndef MATCHPHRASESEDITOR_H
#define MATCHPHRASESEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QJsonObject> // The header for our purse!

class MatchPhrasesEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit MatchPhrasesEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private:
    // Helper functions to keep the UI cute and updated!
    void refreshPairsUI();
    void addPair();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_pairsLayout;

    // The brand new purse for our forgetful editor!
    QJsonObject m_currentQuestion;
};

#endif // MATCHPHRASESEDITOR_H
