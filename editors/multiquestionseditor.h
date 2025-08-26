// =======================================================================
// editors/multiquestionseditor.h
// =======================================================================
#ifndef MULTIQUESTIONSEDITOR_H
#define MULTIQUESTIONSEDITOR_H
#include "../basequestioneditor.h"
class QTextEdit;
class MultiQuestionsEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit MultiQuestionsEditor(QWidget* parent = nullptr);
    void loadQuestion(const QJsonObject& question) override;
    QJsonObject getSavedQuestion() override;
private:
    QTextEdit* m_jsonEditor;
};
#endif // MULTIQUESTIONSEDITOR_H
