// =======================================================================
// editors/mcqmultipleeditor.h
// =======================================================================
#ifndef MCQMULTIPLEEDITOR_H
#define MCQMULTIPLEEDITOR_H
#include "../basequestioneditor.h"
#include <vector>
class QLineEdit; class QTextEdit; class QGroupBox; class QVBoxLayout; class QCheckBox;
class McqMultipleEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit McqMultipleEditor(QWidget* parent = nullptr);
    void loadQuestion(const QJsonObject& question) override;
    QJsonObject getSavedQuestion() override;
private slots:
    void addOption(); void refreshOptionsUI();
private:
    QTextEdit* m_questionTextEdit; QGroupBox* m_optionsGroup; QVBoxLayout* m_optionsLayout;
    std::vector<QWidget*> m_optionWidgets; std::vector<QLineEdit*> m_optionTextEdits;
    std::vector<QLineEdit*> m_optionImageEdits; std::vector<QCheckBox*> m_checkBoxes;
};
#endif // MCQMULTIPLEEDITOR_H
