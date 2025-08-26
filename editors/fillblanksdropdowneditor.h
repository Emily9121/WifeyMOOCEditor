/*
 * File: fillblanksdropdowneditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our super cute fill-in-the-blanks editor!
 * I've given it its very own little purse (a QJsonObject member variable)
 * to hold all the question data for Sierra! So neat and tidy! <3
 */

#ifndef FILLBLANKSDROPDOWNEDITOR_H
#define FILLBLANKSDROPDOWNEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QJsonObject> // The header for our cute purse!

class FillBlanksDropdownEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit FillBlanksDropdownEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private:
    // Helper functions to keep our UI sparkling!
    void refreshPartsUI();
    void refreshBlanksUI();
    void addPart();
    void addBlank();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_partsLayout;
    QVBoxLayout* m_blanksLayout;

    // The all-important purse for our question data!
    QJsonObject m_currentQuestion;
};

#endif // FILLBLANKSDROPDOWNEDITOR_H
