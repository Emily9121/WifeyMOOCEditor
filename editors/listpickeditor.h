/*
 * File: listpickeditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our super-fun list-picking editor!
 * I've given it a pretty pink purse (a QJsonObject member variable)
 * to hold all the list options for Sierra's quizzes! <3
 */

#ifndef LISTPICKEDITOR_H
#define LISTPICKEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QJsonObject> // The header for our purse!

class ListPickEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit ListPickEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private:
    // Helper functions to keep the UI looking fresh!
    void refreshOptionsUI();
    void addOption();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_optionsLayout;

    // Our editor's brand new purse!
    QJsonObject m_currentQuestion;
};

#endif // LISTPICKEDITOR_H
