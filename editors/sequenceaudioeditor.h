// =======================================================================
// editors/sequenceaudioeditor.h
// =======================================================================
#ifndef SEQUENCEAUDIOEDITOR_H
#define SEQUENCEAUDIOEDITOR_H
#include "../basequestioneditor.h"
#include <vector>
class QLineEdit; class QTextEdit; class QGroupBox; class QVBoxLayout;
class SequenceAudioEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit SequenceAudioEditor(QWidget* parent = nullptr);
    void loadJson(const QJsonObject& question) override;
    QJsonObject getJson() override;
private slots:
    void addOption(); void refreshOptionsUI();
private:
    QTextEdit* m_questionTextEdit; QGroupBox* m_optionsGroup; QVBoxLayout* m_optionsLayout;
    QLineEdit* m_answerEdit;
    std::vector<QWidget*> m_optionWidgets; std::vector<QLineEdit*> m_optionEdits;
};
#endif // SEQUENCEAUDIOEDITOR_H

