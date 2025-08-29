/*
* File: categorizationeditor.h  
* Author: Emily
*
* Description:
* The COMPLETE header file for our categorization editor!
* Students drag items into the perfect categories! 📂💖
* Now with a new slot to handle real-time UI updates! ✨
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
    // 💖 Our new magical slot to update the UI when a category name changes! 💖
    void onCategoryNameChanged(int index, const QString& newText);

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

    // 💖 ADDED: UI elements for our lesson PDF! So cute! 💖
    QLineEdit* m_lessonPdfEdit;
    QPushButton* m_lessonPdfButton;

    // Data storage
    QJsonObject m_currentQuestion;
    QList<QWidget*> m_categoryWidgets;
    QList<QWidget*> m_stimuliWidgets;
};

#endif // CATEGORIZATIONEDITOR_H
