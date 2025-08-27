/*
* File: listpickeditor.h
* Author: Emily
*
* Description:
* The COMPLETE header file for our list-picking editor!
* Students pick multiple cute options from a list! 📝💖
*/

#ifndef LISTPICKEDITOR_H
#define LISTPICKEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class ListPickEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit ListPickEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addOption();

private:
    void refreshOptionsUI();
    void clearOptions();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_optionsLayout;

    // Data storage
    QJsonObject m_currentQuestion;
    QList<QWidget*> m_optionWidgets;
};

#endif // LISTPICKEDITOR_H
