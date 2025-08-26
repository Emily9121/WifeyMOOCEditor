// =======================================================================
// editors/fillblanksdropdowneditor.h
// =======================================================================
#ifndef FILLBLANKSDROPDOWNEDITOR_H
#define FILLBLANKSDROPDOWNEDITOR_H
#include "../basequestioneditor.h"
#include <vector>
class QLineEdit; class QTextEdit; class QGroupBox; class QVBoxLayout; class QComboBox;
class FillBlanksDropdownEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit FillBlanksDropdownEditor(QWidget* parent = nullptr);
    void loadJson(const QJsonObject& question) override;
    QJsonObject getJson() override;
private slots:
    void addPart(); void addBlank(); void refreshPartsUI(); void refreshBlanksUI();
private:
    struct BlankWidgets { QWidget* container; QLineEdit* optionsEdit; QComboBox* answerCombo; };
    QTextEdit* m_questionTextEdit; QGroupBox* m_partsGroup; QVBoxLayout* m_partsLayout;
    QGroupBox* m_blanksGroup; QVBoxLayout* m_blanksLayout;
    std::vector<QWidget*> m_partWidgets; std::vector<QTextEdit*> m_partEdits;
    std::vector<BlankWidgets> m_blankWidgets;
};
#endif // FILLBLANKSDROPDOWNEDITOR_H
