// =======================================================================
// editors/orderphraseeditor.h
// =======================================================================
#ifndef ORDERPHRASEEDITOR_H
#define ORDERPHRASEEDITOR_H
#include "../basequestioneditor.h"
class QTextEdit;
class OrderPhraseEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit OrderPhraseEditor(QWidget* parent = nullptr);
    void loadQuestion(const QJsonObject& question) override;
    QJsonObject getSavedQuestion() override;
private:
    QTextEdit* m_questionTextEdit; QTextEdit* m_shuffledTextEdit; QTextEdit* m_answerTextEdit;
};
#endif // ORDERPHRASEEDITOR_H
