#ifndef QUESTIONEDITOR_H
#define QUESTIONEDITOR_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QFrame>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>
#include <QtCore/QList>
#include "questiontypes.h"

class QuestionEditor : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionEditor(QWidget *parent = nullptr);
    virtual ~QuestionEditor() = default;
    virtual void setQuestion(const QJsonObject &question) = 0;
    virtual QJsonObject getQuestion() const = 0;
    virtual QuestionType getType() const = 0;

signals:
    void questionChanged();

protected:
    void setupCommonUI();
    void createQuestionTextSection();
    void createMediaSection();
    QString browseForFile(const QString &title, const QString &filter);

    // Common UI elements
    QVBoxLayout *m_mainLayout;
    QGroupBox *m_questionGroup;
    QLineEdit *m_questionEdit;
    QGroupBox *m_mediaGroup;
    QLineEdit *m_videoEdit;
    QLineEdit *m_audioEdit;
    QLineEdit *m_imageEdit;

    // Current question data
    QJsonObject m_question;

    // Colors (matching main window)
    struct Colors {
        QString bg = "#FFB6C1";
        QString secondary = "#FFC0CB";
        QString accent = "#FF69B4";
        QString text = "#8B008B";
        QString white = "#FFFFFF";
        QString button = "#FF1493";
        QString entry = "#FFEFD5";
    } m_colors;
};

// Specific question type editors

class ListPickEditor : public QuestionEditor
{
    Q_OBJECT

public:
    explicit ListPickEditor(QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::ListPick; }

private slots:
    void addOption();
    void removeOption();
    void saveOptions();

private:
    void setupUI();
    void refreshOptions();

    QGroupBox *m_optionsGroup;
    QVBoxLayout *m_optionsLayout;
    QList<QLineEdit*> m_optionEdits;
    QList<QCheckBox*> m_correctChecks;
};

class McqSingleEditor : public QuestionEditor
{
    Q_OBJECT

public:
    explicit McqSingleEditor(QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::McqSingle; }

private slots:
    void addOption();
    void removeOption();
    void saveOptions();

private:
    void setupUI();
    void refreshOptions();

    QGroupBox *m_optionsGroup;
    QVBoxLayout *m_optionsLayout;
    QList<QLineEdit*> m_optionEdits;
    QButtonGroup *m_correctGroup;
    QList<QRadioButton*> m_correctRadios;
};

class McqMultipleEditor : public QuestionEditor
{
    Q_OBJECT

public:
    explicit McqMultipleEditor(QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::McqMultiple; }

private slots:
    void addOption();
    void removeOption();
    void saveOptions();

private:
    void setupUI();
    void refreshOptions();

    QGroupBox *m_optionsGroup;
    QVBoxLayout *m_optionsLayout;
    QList<QLineEdit*> m_optionEdits;
    QList<QCheckBox*> m_correctChecks;
};

class WordFillEditor : public QuestionEditor
{
    Q_OBJECT

public:
    explicit WordFillEditor(QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::WordFill; }

private slots:
    void addPart();
    void removePart();
    void addAnswer();
    void removeAnswer();
    void saveAll();

private:
    void setupUI();
    void refreshParts();
    void refreshAnswers();

    QGroupBox *m_partsGroup;
    QGroupBox *m_answersGroup;
    QVBoxLayout *m_partsLayout;
    QVBoxLayout *m_answersLayout;
    QList<QLineEdit*> m_partEdits;
    QList<QLineEdit*> m_answerEdits;
};

class SequenceAudioEditor : public QuestionEditor
{
    Q_OBJECT
public:
    explicit SequenceAudioEditor(QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::SequenceAudio; }

private slots:
    void addOption();
    void removeOption();
    void saveOptions();
    void moveOptionUp();
    void moveOptionDown();

private:
    void setupUI();
    void refreshOptions();
    
    QGroupBox *m_optionsGroup;
    QVBoxLayout *m_optionsLayout;
    QList<QLineEdit*> m_optionEdits;
    QList<QSpinBox*> m_orderSpinBoxes;
};

class MatchSentenceEditor : public QuestionEditor
{
    Q_OBJECT
public:
    explicit MatchSentenceEditor(QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::MatchSentence; }

private slots:
    void addPair();
    void removePair();
    void savePairs();
    void browseImage();

private:
    void setupUI();
    void refreshPairs();
    
    QGroupBox *m_pairsGroup;
    QVBoxLayout *m_pairsLayout;
    QList<QLineEdit*> m_sentenceEdits;
    QList<QLineEdit*> m_imageEdits;
};

class FillBlanksDropdownEditor : public QuestionEditor
{
    Q_OBJECT
public:
    explicit FillBlanksDropdownEditor(QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::FillBlanksDropdown; }

private slots:
    void addSentencePart();
    void removeSentencePart();
    void addBlankOption();
    void removeBlankOption();
    void saveAll();

private:
    void setupUI();
    void refreshSentenceParts();
    void refreshBlankOptions();
    
    QGroupBox *m_partsGroup;
    QGroupBox *m_optionsGroup;
    QVBoxLayout *m_partsLayout;
    QVBoxLayout *m_optionsLayout;
    QList<QLineEdit*> m_partEdits;
    QList<QList<QLineEdit*>> m_blankOptionEdits;
    QList<QComboBox*> m_correctAnswerCombos;
};

class OrderPhraseEditor : public QuestionEditor
{
    Q_OBJECT
public:
    explicit OrderPhraseEditor(QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::OrderPhrase; }

private slots:
    void addPhrase();
    void removePhrase();
    void savePhrases();
    void moveUp();
    void moveDown();

private:
    void setupUI();
    void refreshPhrases();
    
    QGroupBox *m_phrasesGroup;
    QVBoxLayout *m_phrasesLayout;
    QList<QLineEdit*> m_phraseEdits;
    QList<QSpinBox*> m_orderSpinBoxes;
};

class CategorizationMultipleEditor : public QuestionEditor
{
    Q_OBJECT
public:
    explicit CategorizationMultipleEditor(QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::CategorizationMultiple; }


private slots:
    void addStimulus();
    void removeStimulus();
    void addCategory();
    void removeCategory();
    void saveAll();
    void browseImageForStimulus(int index);
    void toggleStimulusInputMode(int index, bool useImage);

private:
    void setupUI();
    void refreshStimuli();
    void refreshCategories();
    struct StimulusWidget {
        QRadioButton *textRadio;
        QRadioButton *imageRadio;
        QLineEdit *textEdit;
        QLineEdit *imagePathEdit;
        QPushButton *browseButton;
        QWidget *containerWidget;
        QComboBox *categoryCombo;
    };
    QVector<StimulusWidget> m_stimuliWidgets;
    
    QGroupBox *m_stimuliGroup;
    QGroupBox *m_categoriesGroup;
    QVBoxLayout *m_stimuliLayout;
    QVBoxLayout *m_categoriesLayout;
    QList<QLineEdit*> m_stimuliEdits;
    QList<QComboBox*> m_stimuliCategoryCombo;
    QList<QLineEdit*> m_categoryEdits;
};

class MatchPhrasesEditor : public QuestionEditor
{
    Q_OBJECT
public:
    explicit MatchPhrasesEditor(QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::MatchPhrases; }

private slots:
    void addPair();
    void removePair();
    void addTarget();
    void removeTarget();
    void saveAll();

private:
    void setupUI();
    void refreshPairs();
    
    QGroupBox *m_pairsGroup;
    QVBoxLayout *m_pairsLayout;
    QList<QLineEdit*> m_sourceEdits;
    QList<QList<QLineEdit*>> m_targetEdits;
    QList<QComboBox*> m_correctTargetCombo;
};

class UnknownQuestionEditor : public QuestionEditor
{
    Q_OBJECT

public:
    explicit UnknownQuestionEditor(const QString &typeName, QWidget *parent = nullptr);
    void setQuestion(const QJsonObject &question) override;
    QJsonObject getQuestion() const override;
    QuestionType getType() const override { return QuestionType::Unknown; }

private:
    void setupUI();
    QString m_typeName;
    QTextEdit *m_jsonEdit;
};

#endif // QUESTIONEDITOR_H