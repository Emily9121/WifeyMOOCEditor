// =======================================================================
// editors/wordfilleditor.h
// =======================================================================
#ifndef WORDFILLEDITOR_H
#define WORDFILLEDITOR_H
#include "../basequestioneditor.h"
#include <vector>
class QLineEdit; class QTextEdit; class QGroupBox; class QVBoxLayout;
class WordFillEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit WordFillEditor(QWidget* parent = nullptr);
    void loadJson(const QJsonObject& question) override;
    QJsonObject getJson() override;
private slots:
    void addPart(); void addAnswer(); void refreshPartsUI(); void refreshAnswersUI();
private:
    QTextEdit* m_questionTextEdit; QGroupBox* m_partsGroup; QVBoxLayout* m_partsLayout;
    QGroupBox* m_answersGroup; QVBoxLayout* m_answersLayout;
    std::vector<QWidget*> m_partWidgets; std::vector<QTextEdit*> m_partEdits;
    std::vector<QWidget*> m_answerWidgets; std::vector<QLineEdit*> m_answerEdits;
};
#endif // WORDFILLEDITOR_H
