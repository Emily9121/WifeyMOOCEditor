// =======================================================================
// editors/mcqsingleeditor.h
// =======================================================================
#ifndef MCQSINGLEEDITOR_H
#define MCQSINGLEEDITOR_H
#include "../basequestioneditor.h"
#include <vector>
class QLineEdit; class QTextEdit; class QGroupBox; class QVBoxLayout; class QRadioButton;
class McqSingleEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit McqSingleEditor(QWidget* parent = nullptr);
    void loadQuestion(const QJsonObject& question) override;
    QJsonObject getSavedQuestion() override;
private slots:
    void addOption(); void refreshOptionsUI();
private:
    QTextEdit* m_questionTextEdit; QGroupBox* m_optionsGroup; QVBoxLayout* m_optionsLayout;
    std::vector<QWidget*> m_optionWidgets; std::vector<QLineEdit*> m_optionTextEdits;
    std::vector<QLineEdit*> m_optionImageEdits; std::vector<QRadioButton*> m_radioButtons;
};
#endif // MCQSINGLEEDITOR_H