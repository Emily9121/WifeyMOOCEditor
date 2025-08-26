// =======================================================================
// editors/listpickeditor.h
// =======================================================================
#ifndef LISTPICKEDITOR_H
#define LISTPICKEDITOR_H
#include "../basequestioneditor.h"
#include <vector>
class QLineEdit; class QTextEdit; class QGroupBox; class QVBoxLayout; class QCheckBox;
class ListPickEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit ListPickEditor(QWidget* parent = nullptr);
    void loadJson(const QJsonObject& question) override;
    QJsonObject getJson() override;
private slots:
    void addOption(); void refreshOptionsUI();
private:
    QTextEdit* m_questionTextEdit; QGroupBox* m_optionsGroup; QVBoxLayout* m_optionsLayout;
    std::vector<QWidget*> m_optionWidgets; std::vector<QLineEdit*> m_optionEdits; std::vector<QCheckBox*> m_checkBoxes;
};
#endif // LISTPICKEDITOR_H