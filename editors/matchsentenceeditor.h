// =======================================================================
// editors/matchphraseseditor.h
// =======================================================================
#ifndef MATCHPHRASESEDITOR_H
#define MATCHPHRASESEDITOR_H
#include "../basequestioneditor.h"
#include <vector>
class QLineEdit; class QTextEdit; class QGroupBox; class QVBoxLayout; class QComboBox;
class MatchPhrasesEditor : public BaseQuestionEditor {
    Q_OBJECT
public:
    explicit MatchPhrasesEditor(QWidget* parent = nullptr);
    void loadJson(const QJsonObject& question) override;
    QJsonObject getJson() override;
private slots:
    void addPair(); void refreshPairsUI();
private:
    struct PairWidgets { QWidget* container; QLineEdit* sourceEdit; QLineEdit* targetsEdit; QComboBox* answerCombo; };
    QTextEdit* m_questionTextEdit; QGroupBox* m_pairsGroup; QVBoxLayout* m_pairsLayout;
    std::vector<PairWidgets> m_pairWidgets;
};
#endif // MATCHPHRASESEDITOR_H