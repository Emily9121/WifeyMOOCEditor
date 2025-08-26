/*
 * File: orderphraseeditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our super-organized phrase-ordering editor!
 * I've given it a sparkly new purse (a QJsonObject member variable)
 * so it can remember the correct order for Sierra's sentences! <3
 */

#ifndef ORDERPHRASEEDITOR_H
#define ORDERPHRASEEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QJsonObject> // The header for our purse!

class OrderPhraseEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit OrderPhraseEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private:
    // Helper functions to keep the UI looking perfect!
    void refreshPhrasesUI();
    void addPhrase();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_phrasesLayout;

    // The brand new purse for our editor!
    QJsonObject m_currentQuestion;
};

#endif // ORDERPHRASEEDITOR_H
