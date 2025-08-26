// =======================================================================
// editors/categorizationeditor.h
// =======================================================================
#ifndef CATEGORIZATIONEDITOR_H
#define CATEGORIZATIONEDITOR_H
#include "../basequestioneditor.h"
#include <vector>
class QLineEdit; class QTextEdit; class QGroupBox; class QVBoxLayout; class QComboBox;
class CategorizationEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit CategorizationEditor(QWidget* parent = nullptr);
    void loadQuestion(const QJsonObject& question) override;
    QJsonObject getSavedQuestion() override;
private slots:
    void addCategory(); void addStimulus(); void refreshCategoriesUI(); void refreshStimuliUI();
private:
    struct StimulusWidgets { QWidget* container; QLineEdit* textEdit; QLineEdit* imageEdit; QComboBox* categoryCombo; };
    QTextEdit* m_questionTextEdit; QGroupBox* m_categoriesGroup; QVBoxLayout* m_categoriesLayout;
    QGroupBox* m_stimuliGroup; QVBoxLayout* m_stimuliLayout;
    std::vector<QWidget*> m_categoryWidgets; std::vector<QLineEdit*> m_categoryEdits;
    std::vector<StimulusWidgets> m_stimulusWidgets;
};
#endif // CATEGORIZATIONEDITOR_H
