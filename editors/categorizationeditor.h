/*
 * File: categorizationeditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our super-fun categorization editor!
 * I've added a little "purse" (a QJsonObject member variable) for it
 * to hold all the question data! So organized! <3
 */

#ifndef CATEGORIZATIONEDITOR_H
#define CATEGORIZATIONEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QJsonObject> // We need this for our purse!

class CategorizationEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit CategorizationEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private:
    // Helper functions to keep the UI fresh and cute!
    void refreshCategoriesUI();
    void refreshStimuliUI();
    void addCategory();
    void addStimulus();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_categoriesLayout;
    QVBoxLayout* m_stimuliLayout;

    // The magical purse to hold our question data!
    QJsonObject m_currentQuestion;
};

#endif // CATEGORIZATIONEDITOR_H
