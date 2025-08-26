// =======================================================================
// editors/imagetaggingeditor.h
// =======================================================================
#ifndef IMAGETAGGINGEDITOR_H
#define IMAGETAGGINGEDITOR_H
#include "../basequestioneditor.h"
#include <vector>
class QLineEdit; class QTextEdit; class QGroupBox; class QVBoxLayout;
class ImageTaggingEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit ImageTaggingEditor(QWidget* parent = nullptr);
    void loadQuestion(const QJsonObject& question) override;
    QJsonObject getSavedQuestion() override;
private slots:
    void addTag(); void addAlternative(); void refreshUI();
private:
    struct TagWidgets { QWidget* container; QLineEdit* idEdit; QLineEdit* labelEdit; QLineEdit* xEdit; QLineEdit* yEdit; };
    struct AltWidgets { QGroupBox* container; QLineEdit* imageEdit; QLineEdit* buttonLabelEdit; std::vector<QWidget*> tagCoordWidgets; std::vector<QLineEdit*> xEdits; std::vector<QLineEdit*> yEdits; };
    void refreshMainTagsUI(); void refreshAlternativesUI();
    QTextEdit* m_questionTextEdit; QLineEdit* m_mainImageEdit; QLineEdit* m_mainButtonLabelEdit;
    QGroupBox* m_tagsGroup; QVBoxLayout* m_tagsLayout;
    QGroupBox* m_altsGroup; QVBoxLayout* m_altsLayout;
    std::vector<TagWidgets> m_tagWidgets; std::vector<AltWidgets> m_altWidgets;
};
#endif // IMAGETAGGINGEDITOR_H
