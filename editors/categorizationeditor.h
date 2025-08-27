/*
* File: categorizationeditor.h  
* Author: Emily
*
* Description:
* The COMPLETE header file for our categorization editor!
* Students drag items into the perfect categories! 📂💖
*/

#ifndef CATEGORIZATIONEDITOR_H
#define CATEGORIZATIONEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class CategorizationEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit CategorizationEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addCategory();
    void addStimulus();
    void browseMedia();
    void browseStimulusImage(QLineEdit* imageEdit);

private:
    void refreshCategoriesUI();
    void refreshStimuliUI();
    void clearCategories();
    void clearStimuli();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_categoriesLayout;
    QVBoxLayout* m_stimuliLayout;
    QLineEdit* m_mediaEdit;
    QComboBox* m_mediaTypeCombo;

    // Data storage
    QJsonObject m_currentQuestion;
    QList<QWidget*> m_categoryWidgets;
    QList<QWidget*> m_stimuliWidgets;
};

#endif // CATEGORIZATIONEDITOR_H
