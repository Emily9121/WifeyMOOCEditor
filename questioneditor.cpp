#include "questioneditor.h"
#include <QJsonArray>

// Base QuestionEditor implementation
QuestionEditor::QuestionEditor(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_questionGroup(nullptr)
    , m_questionEdit(nullptr)
    , m_mediaGroup(nullptr)
    , m_videoEdit(nullptr)
    , m_audioEdit(nullptr)
    , m_imageEdit(nullptr)
{
    setupCommonUI();
}

void QuestionEditor::setupCommonUI()
{
    m_mainLayout = new QVBoxLayout(this);

    // Apply styling
    setStyleSheet(QString(
        "QGroupBox { "
        "   font-weight: bold; "
        "   border: 2px solid %1; "
        "   border-radius: 5px; "
        "   margin-top: 1ex; "
        "   color: %2; "
        "}"
        "QGroupBox::title { "
        "   subcontrol-origin: margin; "
        "   left: 10px; "
        "   padding: 0 5px 0 5px; "
        "}"
        "QPushButton { "
        "   background-color: %3; "
        "   color: black; "
        "   border: 2px solid %1; "
        "   border-radius: 5px; "
        "   padding: 6px 12px; "
        "   font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: %1; }"
        "QLineEdit, QTextEdit, QComboBox { "
        "   background-color: %4; "
        "   border: 1px solid %1; "
        "   border-radius: 3px; "
        "   padding: 5px; "
        "}"
        "QCheckBox, QRadioButton { color: %2; }"
        "QLabel { color: %2; }"
    ).arg(m_colors.accent, m_colors.text, m_colors.button, m_colors.entry));

    createQuestionTextSection();
    createMediaSection();
}

void QuestionEditor::createQuestionTextSection()
{
    m_questionGroup = new QGroupBox("📝 Question Text 📝", this);
    QVBoxLayout *layout = new QVBoxLayout(m_questionGroup);

    QLabel *label = new QLabel("💡 Write your question here, sweetie!", this);
    label->setStyleSheet("font-style: italic; margin: 5px;");
    layout->addWidget(label);

    m_questionEdit = new QLineEdit(this);
    m_questionEdit->setPlaceholderText("Enter your amazing question here...");
    layout->addWidget(m_questionEdit);

    QPushButton *saveButton = new QPushButton("💾 Save Question Text", this);
    connect(saveButton, &QPushButton::clicked, [this]() {
        m_question["question"] = m_questionEdit->text();
        emit questionChanged();
    });
    layout->addWidget(saveButton);

    m_mainLayout->addWidget(m_questionGroup);
}

void QuestionEditor::createMediaSection()
{
    m_mediaGroup = new QGroupBox("🎬 Media Files (Video, Audio, Images) 🎬", this);
    QVBoxLayout *layout = new QVBoxLayout(m_mediaGroup);

    // Video section
    QHBoxLayout *videoLayout = new QHBoxLayout();
    videoLayout->addWidget(new QLabel("📹 Video File:", this));
    m_videoEdit = new QLineEdit(this);
    videoLayout->addWidget(m_videoEdit);
    QPushButton *videoBrowse = new QPushButton("📁", this);
    connect(videoBrowse, &QPushButton::clicked, [this]() {
        QString file = browseForFile("Select Video File", 
            "Video files (*.mp4 *.avi *.mov *.wmv *.flv *.mkv);;All files (*.*)");
        if (!file.isEmpty()) m_videoEdit->setText(file);
    });
    videoLayout->addWidget(videoBrowse);
    layout->addLayout(videoLayout);

    // Audio section
    QHBoxLayout *audioLayout = new QHBoxLayout();
    audioLayout->addWidget(new QLabel("🎵 Audio File:", this));
    m_audioEdit = new QLineEdit(this);
    audioLayout->addWidget(m_audioEdit);
    QPushButton *audioBrowse = new QPushButton("📁", this);
    connect(audioBrowse, &QPushButton::clicked, [this]() {
        QString file = browseForFile("Select Audio File", 
            "Audio files (*.mp3 *.wav *.ogg *.m4a *.aac *.flac);;All files (*.*)");
        if (!file.isEmpty()) m_audioEdit->setText(file);
    });
    audioLayout->addWidget(audioBrowse);
    layout->addLayout(audioLayout);

    // Image section
    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addWidget(new QLabel("🖼️ Image File:", this));
    m_imageEdit = new QLineEdit(this);
    imageLayout->addWidget(m_imageEdit);
    QPushButton *imageBrowse = new QPushButton("📁", this);
    connect(imageBrowse, &QPushButton::clicked, [this]() {
        QString file = browseForFile("Select Image File", 
            "Image files (*.jpg *.jpeg *.png *.gif *.bmp *.svg *.webp);;All files (*.*)");
        if (!file.isEmpty()) m_imageEdit->setText(file);
    });
    imageLayout->addWidget(imageBrowse);
    layout->addLayout(imageLayout);

    QPushButton *saveMediaButton = new QPushButton("💾 Save Media Files", this);
    connect(saveMediaButton, &QPushButton::clicked, [this]() {
        QJsonObject media;
        if (!m_videoEdit->text().trimmed().isEmpty()) {
            media["video"] = m_videoEdit->text().trimmed();
        }
        if (!m_audioEdit->text().trimmed().isEmpty()) {
            media["audio"] = m_audioEdit->text().trimmed();
        }
        if (!m_imageEdit->text().trimmed().isEmpty()) {
            media["image"] = m_imageEdit->text().trimmed();
        }

        if (media.isEmpty()) {
            m_question["media"] = QJsonValue::Null;
        } else {
            m_question["media"] = media;
        }
        emit questionChanged();
    });
    layout->addWidget(saveMediaButton);

    m_mainLayout->addWidget(m_mediaGroup);
}

QString QuestionEditor::browseForFile(const QString &title, const QString &filter)
{
    return QFileDialog::getOpenFileName(this, title, QString(), filter);
}

// ListPickEditor implementation
ListPickEditor::ListPickEditor(QWidget *parent)
    : QuestionEditor(parent)
    , m_optionsGroup(nullptr)
    , m_optionsLayout(nullptr)
{
    setupUI();
}

void ListPickEditor::setupUI()
{
    m_optionsGroup = new QGroupBox("📋 Answer Options (Students can pick multiple) 📋", this);
    QVBoxLayout *groupLayout = new QVBoxLayout(m_optionsGroup);

    QLabel *infoLabel = new QLabel("💡 Check the boxes next to correct answers!", this);
    infoLabel->setStyleSheet("font-style: italic; margin: 5px;");
    groupLayout->addWidget(infoLabel);

    // Scrollable area for options
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollWidget = new QWidget();
    m_optionsLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMaximumHeight(300);
    groupLayout->addWidget(scrollArea);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("➕ Add Option", this);
    QPushButton *saveButton = new QPushButton("💾 Save All Options", this);

    connect(addButton, &QPushButton::clicked, this, &ListPickEditor::addOption);
    connect(saveButton, &QPushButton::clicked, this, &ListPickEditor::saveOptions);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    groupLayout->addLayout(buttonLayout);

    m_mainLayout->addWidget(m_optionsGroup);
    m_mainLayout->addStretch();
}

void ListPickEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;

    // Set common fields
    m_questionEdit->setText(question["question"].toString());

    // Set media fields
    QJsonObject media = question["media"].toObject();
    m_videoEdit->setText(media["video"].toString());
    m_audioEdit->setText(media["audio"].toString());
    m_imageEdit->setText(media["image"].toString());

    refreshOptions();
}

QJsonObject ListPickEditor::getQuestion() const
{
    return m_question;
}

void ListPickEditor::refreshOptions()
{
    // Clear existing option widgets
    for (QLineEdit *edit : m_optionEdits) {
        delete edit;
    }
    for (QCheckBox *check : m_correctChecks) {
        delete check;
    }
    m_optionEdits.clear();
    m_correctChecks.clear();

    // Clear layout
    QLayoutItem *item;
    while ((item = m_optionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray options = m_question["options"].toArray();
    QJsonArray answers = m_question["answer"].toArray();

    for (int i = 0; i < options.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout();

        // Correct checkbox
        QCheckBox *correctCheck = new QCheckBox("✅ Correct", this);

        // Check if this option index is in the answers array
        bool isCorrect = false;
        for (const QJsonValue &answerValue : answers) {
            if (answerValue.toInt() == i) {
                isCorrect = true;
                break;
            }
        }
        correctCheck->setChecked(isCorrect);
        m_correctChecks.append(correctCheck);

        // Option text
        QLabel *label = new QLabel(QString("Option %1:").arg(i + 1), this);
        QLineEdit *optionEdit = new QLineEdit(options[i].toString(), this);
        m_optionEdits.append(optionEdit);

        // Remove button (if more than 2 options)
        QPushButton *removeButton = nullptr;
        if (options.size() > 2) {
            removeButton = new QPushButton("🗑️", this);
            removeButton->setMaximumWidth(30);
            connect(removeButton, &QPushButton::clicked, [this, i]() {
                removeOption();
            });
        }

        rowLayout->addWidget(correctCheck);
        rowLayout->addWidget(label);
        rowLayout->addWidget(optionEdit, 1);
        if (removeButton) {
            rowLayout->addWidget(removeButton);
        }

        QWidget *rowWidget = new QWidget();
        rowWidget->setLayout(rowLayout);
        m_optionsLayout->addWidget(rowWidget);
    }

    m_optionsLayout->addStretch();
}

void ListPickEditor::addOption()
{
    QJsonArray options = m_question["options"].toArray();
    options.append("New Cute Option");
    m_question["options"] = options;

    refreshOptions();
    emit questionChanged();
}

void ListPickEditor::removeOption()
{
    QJsonArray options = m_question["options"].toArray();
    if (options.size() <= 2) return;

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    // Find which option to remove (simplified approach - remove last)
    options.removeLast();
    m_question["options"] = options;

    // Update answer indices
    QJsonArray answers = m_question["answer"].toArray();
    QJsonArray newAnswers;
    for (const QJsonValue &answer : answers) {
        int index = answer.toInt();
        if (index < options.size()) {
            newAnswers.append(index);
        }
    }
    m_question["answer"] = newAnswers;

    refreshOptions();
    emit questionChanged();
}

void ListPickEditor::saveOptions()
{
    // Save option texts
    QJsonArray options;
    for (QLineEdit *edit : m_optionEdits) {
        if (!edit->text().trimmed().isEmpty()) {
            options.append(edit->text().trimmed());
        }
    }
    m_question["options"] = options;

    // Save correct answers
    QJsonArray answers;
    for (int i = 0; i < m_correctChecks.size(); ++i) {
        if (m_correctChecks[i]->isChecked()) {
            answers.append(i);
        }
    }
    m_question["answer"] = answers;

    emit questionChanged();
    QMessageBox::information(this, "Success! 💖", "Options saved successfully, darling!");
}

// Continue with other editor implementations...

// McqSingleEditor implementation
McqSingleEditor::McqSingleEditor(QWidget *parent)
    : QuestionEditor(parent)
    , m_optionsGroup(nullptr)
    , m_optionsLayout(nullptr)
    , m_correctGroup(new QButtonGroup(this))
{
    setupUI();
}

void McqSingleEditor::setupUI()
{
    m_optionsGroup = new QGroupBox("🔘 Answer Options (Students pick only ONE) 🔘", this);
    QVBoxLayout *groupLayout = new QVBoxLayout(m_optionsGroup);

    QLabel *infoLabel = new QLabel("💡 Select the radio button next to the correct answer!", this);
    infoLabel->setStyleSheet("font-style: italic; margin: 5px;");
    groupLayout->addWidget(infoLabel);

    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollWidget = new QWidget();
    m_optionsLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMaximumHeight(300);
    groupLayout->addWidget(scrollArea);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("➕ Add Option", this);
    QPushButton *saveButton = new QPushButton("💾 Save All Options", this);

    connect(addButton, &QPushButton::clicked, this, &McqSingleEditor::addOption);
    connect(saveButton, &QPushButton::clicked, this, &McqSingleEditor::saveOptions);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    groupLayout->addLayout(buttonLayout);

    m_mainLayout->addWidget(m_optionsGroup);
    m_mainLayout->addStretch();
}

void McqSingleEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;
    m_questionEdit->setText(question["question"].toString());

    QJsonObject media = question["media"].toObject();
    m_videoEdit->setText(media["video"].toString());
    m_audioEdit->setText(media["audio"].toString());
    m_imageEdit->setText(media["image"].toString());

    refreshOptions();
}

QJsonObject McqSingleEditor::getQuestion() const
{
    return m_question;
}

void McqSingleEditor::refreshOptions()
{
    // Clear existing widgets
    for (QLineEdit *edit : m_optionEdits) {
        delete edit;
    }
    for (QRadioButton *radio : m_correctRadios) {
        m_correctGroup->removeButton(radio);
        delete radio;
    }
    m_optionEdits.clear();
    m_correctRadios.clear();

    QLayoutItem *item;
    while ((item = m_optionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray options = m_question["options"].toArray();
    QJsonArray answers = m_question["answer"].toArray();
    int correctAnswer = answers.isEmpty() ? 0 : answers[0].toInt();

    for (int i = 0; i < options.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout();

        QRadioButton *correctRadio = new QRadioButton("✅ Correct", this);
        correctRadio->setChecked(i == correctAnswer);
        m_correctGroup->addButton(correctRadio, i);
        m_correctRadios.append(correctRadio);

        QLabel *label = new QLabel(QString("Option %1:").arg(i + 1), this);
        QLineEdit *optionEdit = new QLineEdit(options[i].toString(), this);
        m_optionEdits.append(optionEdit);

        rowLayout->addWidget(correctRadio);
        rowLayout->addWidget(label);
        rowLayout->addWidget(optionEdit, 1);

        if (options.size() > 2) {
            QPushButton *removeButton = new QPushButton("🗑️", this);
            removeButton->setMaximumWidth(30);
            connect(removeButton, &QPushButton::clicked, this, &McqSingleEditor::removeOption);
            rowLayout->addWidget(removeButton);
        }

        QWidget *rowWidget = new QWidget();
        rowWidget->setLayout(rowLayout);
        m_optionsLayout->addWidget(rowWidget);
    }

    m_optionsLayout->addStretch();
}

void McqSingleEditor::addOption()
{
    QJsonArray options = m_question["options"].toArray();
    options.append("New Amazing Option");
    m_question["options"] = options;

    refreshOptions();
    emit questionChanged();
}

void McqSingleEditor::removeOption()
{
    QJsonArray options = m_question["options"].toArray();
    if (options.size() <= 2) return;

    options.removeLast();
    m_question["options"] = options;

    // Adjust correct answer if necessary
    QJsonArray answers = m_question["answer"].toArray();
    if (!answers.isEmpty()) {
        int correctAnswer = answers[0].toInt();
        if (correctAnswer >= options.size()) {
            answers[0] = 0;
            m_question["answer"] = answers;
        }
    }

    refreshOptions();
    emit questionChanged();
}

void McqSingleEditor::saveOptions()
{
    QJsonArray options;
    for (QLineEdit *edit : m_optionEdits) {
        if (!edit->text().trimmed().isEmpty()) {
            options.append(edit->text().trimmed());
        }
    }
    m_question["options"] = options;

    int checkedId = m_correctGroup->checkedId();
    if (checkedId >= 0) {
        QJsonArray answers;
        answers.append(checkedId);
        m_question["answer"] = answers;
    }

    emit questionChanged();
    QMessageBox::information(this, "Success! 💖", "Single choice options saved perfectly!");
}

// McqMultipleEditor implementation
McqMultipleEditor::McqMultipleEditor(QWidget *parent)
    : QuestionEditor(parent)
    , m_optionsGroup(nullptr)
    , m_optionsLayout(nullptr)
{
    setupUI();
}

void McqMultipleEditor::setupUI()
{
    m_optionsGroup = new QGroupBox("☑️ Answer Options (Students can pick MULTIPLE) ☑️", this);
    QVBoxLayout *groupLayout = new QVBoxLayout(m_optionsGroup);

    QLabel *infoLabel = new QLabel("💡 Check all the boxes next to correct answers!", this);
    infoLabel->setStyleSheet("font-style: italic; margin: 5px;");
    groupLayout->addWidget(infoLabel);

    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollWidget = new QWidget();
    m_optionsLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMaximumHeight(300);
    groupLayout->addWidget(scrollArea);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("➕ Add Option", this);
    QPushButton *saveButton = new QPushButton("💾 Save All Options", this);

    connect(addButton, &QPushButton::clicked, this, &McqMultipleEditor::addOption);
    connect(saveButton, &QPushButton::clicked, this, &McqMultipleEditor::saveOptions);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    groupLayout->addLayout(buttonLayout);

    m_mainLayout->addWidget(m_optionsGroup);
    m_mainLayout->addStretch();
}

void McqMultipleEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;
    m_questionEdit->setText(question["question"].toString());

    QJsonObject media = question["media"].toObject();
    m_videoEdit->setText(media["video"].toString());
    m_audioEdit->setText(media["audio"].toString());
    m_imageEdit->setText(media["image"].toString());

    refreshOptions();
}

QJsonObject McqMultipleEditor::getQuestion() const
{
    return m_question;
}

void McqMultipleEditor::refreshOptions()
{
    // Clear existing widgets
    for (QLineEdit *edit : m_optionEdits) {
        delete edit;
    }
    for (QCheckBox *check : m_correctChecks) {
        delete check;
    }
    m_optionEdits.clear();
    m_correctChecks.clear();

    QLayoutItem *item;
    while ((item = m_optionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray options = m_question["options"].toArray();
    QJsonArray answers = m_question["answer"].toArray();

    for (int i = 0; i < options.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout();

        QCheckBox *correctCheck = new QCheckBox("✅ Correct", this);

        // Check if this option index is in the answers array
        bool isCorrect = false;
        for (const QJsonValue &answerValue : answers) {
            if (answerValue.toInt() == i) {
                isCorrect = true;
                break;
            }
        }
        correctCheck->setChecked(isCorrect);
        m_correctChecks.append(correctCheck);

        QLabel *label = new QLabel(QString("Option %1:").arg(i + 1), this);
        QLineEdit *optionEdit = new QLineEdit(options[i].toString(), this);
        m_optionEdits.append(optionEdit);

        rowLayout->addWidget(correctCheck);
        rowLayout->addWidget(label);
        rowLayout->addWidget(optionEdit, 1);

        if (options.size() > 2) {
            QPushButton *removeButton = new QPushButton("🗑️", this);
            removeButton->setMaximumWidth(30);
            connect(removeButton, &QPushButton::clicked, this, &McqMultipleEditor::removeOption);
            rowLayout->addWidget(removeButton);
        }

        QWidget *rowWidget = new QWidget();
        rowWidget->setLayout(rowLayout);
        m_optionsLayout->addWidget(rowWidget);
    }

    m_optionsLayout->addStretch();
}

void McqMultipleEditor::addOption()
{
    QJsonArray options = m_question["options"].toArray();
    options.append("New Fantastic Option");
    m_question["options"] = options;

    refreshOptions();
    emit questionChanged();
}

void McqMultipleEditor::removeOption()
{
    QJsonArray options = m_question["options"].toArray();
    if (options.size() <= 2) return;

    options.removeLast();
    m_question["options"] = options;

    // Update answer indices
    QJsonArray answers = m_question["answer"].toArray();
    QJsonArray newAnswers;
    for (const QJsonValue &answer : answers) {
        int index = answer.toInt();
        if (index < options.size()) {
            newAnswers.append(index);
        }
    }
    m_question["answer"] = newAnswers;

    refreshOptions();
    emit questionChanged();
}

void McqMultipleEditor::saveOptions()
{
    QJsonArray options;
    for (QLineEdit *edit : m_optionEdits) {
        if (!edit->text().trimmed().isEmpty()) {
            options.append(edit->text().trimmed());
        }
    }
    m_question["options"] = options;

    QJsonArray answers;
    for (int i = 0; i < m_correctChecks.size(); ++i) {
        if (m_correctChecks[i]->isChecked()) {
            answers.append(i);
        }
    }
    m_question["answer"] = answers;

    emit questionChanged();
    QMessageBox::information(this, "Success! 💖", "Multiple choice options saved beautifully!");
}

// WordFillEditor implementation
WordFillEditor::WordFillEditor(QWidget *parent)
    : QuestionEditor(parent)
    , m_partsGroup(nullptr)
    , m_answersGroup(nullptr)
    , m_partsLayout(nullptr)
    , m_answersLayout(nullptr)
{
    setupUI();
}

void WordFillEditor::setupUI()
{
    // Sentence parts section
    m_partsGroup = new QGroupBox("📝 Sentence Parts (Text before and after blanks) 📝", this);
    QVBoxLayout *partsGroupLayout = new QVBoxLayout(m_partsGroup);

    QLabel *partsInfo = new QLabel("💡 Example: 'The cat is ' [BLANK] ' and very ' [BLANK] ' today.'\nCreate parts: ['The cat is ', ' and very ', ' today.']", this);
    partsInfo->setStyleSheet("font-style: italic; margin: 5px;");
    partsInfo->setWordWrap(true);
    partsGroupLayout->addWidget(partsInfo);

    QScrollArea *partsScrollArea = new QScrollArea(this);
    QWidget *partsScrollWidget = new QWidget();
    m_partsLayout = new QVBoxLayout(partsScrollWidget);
    partsScrollArea->setWidget(partsScrollWidget);
    partsScrollArea->setWidgetResizable(true);
    partsScrollArea->setMaximumHeight(200);
    partsGroupLayout->addWidget(partsScrollArea);

    // Answers section
    m_answersGroup = new QGroupBox("✅ Correct Answers for Blanks ✅", this);
    QVBoxLayout *answersGroupLayout = new QVBoxLayout(m_answersGroup);

    QLabel *answersInfo = new QLabel("💡 Enter the correct word/phrase for each blank!", this);
    answersInfo->setStyleSheet("font-style: italic; margin: 5px;");
    answersGroupLayout->addWidget(answersInfo);

    QScrollArea *answersScrollArea = new QScrollArea(this);
    QWidget *answersScrollWidget = new QWidget();
    m_answersLayout = new QVBoxLayout(answersScrollWidget);
    answersScrollArea->setWidget(answersScrollWidget);
    answersScrollArea->setWidgetResizable(true);
    answersScrollArea->setMaximumHeight(200);
    answersGroupLayout->addWidget(answersScrollArea);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addPartButton = new QPushButton("➕ Add Part", this);
    QPushButton *addAnswerButton = new QPushButton("➕ Add Answer", this);
    QPushButton *saveAllButton = new QPushButton("💾 Save Everything", this);

    connect(addPartButton, &QPushButton::clicked, this, &WordFillEditor::addPart);
    connect(addAnswerButton, &QPushButton::clicked, this, &WordFillEditor::addAnswer);
    connect(saveAllButton, &QPushButton::clicked, this, &WordFillEditor::saveAll);

    buttonLayout->addWidget(addPartButton);
    buttonLayout->addWidget(addAnswerButton);
    buttonLayout->addWidget(saveAllButton);
    buttonLayout->addStretch();

    partsGroupLayout->addLayout(buttonLayout);

    m_mainLayout->addWidget(m_partsGroup);
    m_mainLayout->addWidget(m_answersGroup);
    m_mainLayout->addStretch();
}

void WordFillEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;
    m_questionEdit->setText(question["question"].toString());

    QJsonObject media = question["media"].toObject();
    m_videoEdit->setText(media["video"].toString());
    m_audioEdit->setText(media["audio"].toString());
    m_imageEdit->setText(media["image"].toString());

    refreshParts();
    refreshAnswers();
}

QJsonObject WordFillEditor::getQuestion() const
{
    return m_question;
}

void WordFillEditor::refreshParts()
{
    // Clear existing widgets
    for (QLineEdit *edit : m_partEdits) {
        delete edit;
    }
    m_partEdits.clear();

    QLayoutItem *item;
    while ((item = m_partsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray parts = m_question["sentence_parts"].toArray();

    for (int i = 0; i < parts.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout();

        QLabel *label = new QLabel(QString("Part %1:").arg(i + 1), this);
        QLineEdit *partEdit = new QLineEdit(parts[i].toString(), this);
        m_partEdits.append(partEdit);

        rowLayout->addWidget(label);
        rowLayout->addWidget(partEdit, 1);

        if (parts.size() > 2) {
            QPushButton *removeButton = new QPushButton("🗑️", this);
            removeButton->setMaximumWidth(30);
            connect(removeButton, &QPushButton::clicked, this, &WordFillEditor::removePart);
            rowLayout->addWidget(removeButton);
        }

        QWidget *rowWidget = new QWidget();
        rowWidget->setLayout(rowLayout);
        m_partsLayout->addWidget(rowWidget);
    }

    m_partsLayout->addStretch();
}

void WordFillEditor::refreshAnswers()
{
    // Clear existing widgets
    for (QLineEdit *edit : m_answerEdits) {
        delete edit;
    }
    m_answerEdits.clear();

    QLayoutItem *item;
    while ((item = m_answersLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray answers = m_question["answers"].toArray();

    for (int i = 0; i < answers.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout();

        QLabel *label = new QLabel(QString("Answer for Blank %1:").arg(i + 1), this);
        QLineEdit *answerEdit = new QLineEdit(answers[i].toString(), this);
        m_answerEdits.append(answerEdit);

        rowLayout->addWidget(label);
        rowLayout->addWidget(answerEdit, 1);

        if (answers.size() > 1) {
            QPushButton *removeButton = new QPushButton("🗑️", this);
            removeButton->setMaximumWidth(30);
            connect(removeButton, &QPushButton::clicked, this, &WordFillEditor::removeAnswer);
            rowLayout->addWidget(removeButton);
        }

        QWidget *rowWidget = new QWidget();
        rowWidget->setLayout(rowLayout);
        m_answersLayout->addWidget(rowWidget);
    }

    m_answersLayout->addStretch();
}

void WordFillEditor::addPart()
{
    QJsonArray parts = m_question["sentence_parts"].toArray();
    parts.append("");
    m_question["sentence_parts"] = parts;

    refreshParts();
    emit questionChanged();
}

void WordFillEditor::removePart()
{
    QJsonArray parts = m_question["sentence_parts"].toArray();
    if (parts.size() <= 2) return;

    parts.removeLast();
    m_question["sentence_parts"] = parts;

    refreshParts();
    emit questionChanged();
}

void WordFillEditor::addAnswer()
{
    QJsonArray answers = m_question["answers"].toArray();
    answers.append("");
    m_question["answers"] = answers;

    refreshAnswers();
    emit questionChanged();
}

void WordFillEditor::removeAnswer()
{
    QJsonArray answers = m_question["answers"].toArray();
    if (answers.size() <= 1) return;

    answers.removeLast();
    m_question["answers"] = answers;

    refreshAnswers();
    emit questionChanged();
}

void WordFillEditor::saveAll()
{
    QJsonArray parts;
    for (QLineEdit *edit : m_partEdits) {
        parts.append(edit->text());
    }
    m_question["sentence_parts"] = parts;

    QJsonArray answers;
    for (QLineEdit *edit : m_answerEdits) {
        if (!edit->text().trimmed().isEmpty()) {
            answers.append(edit->text().trimmed());
        }
    }
    m_question["answers"] = answers;

    emit questionChanged();
    QMessageBox::information(this, "Success! 💖", "Word fill data saved perfectly, babe!");
}

// UnknownQuestionEditor implementation
UnknownQuestionEditor::UnknownQuestionEditor(const QString &typeName, QWidget *parent)
    : QuestionEditor(parent)
    , m_typeName(typeName)
    , m_jsonEdit(nullptr)
{
    setupUI();
}

void UnknownQuestionEditor::setupUI()
{
    QGroupBox *warningGroup = new QGroupBox("⚠️ Unknown Question Type ⚠️", this);
    QVBoxLayout *layout = new QVBoxLayout(warningGroup);

    QLabel *warningLabel = new QLabel(
        QString("💔 The question type '%1' is not supported by this editor.\n\n"
               "💡 You can edit the JSON directly below, but please be careful!\n\n"
               "📝 The editor supports these types: list_pick, mcq_single, mcq_multiple, "
               "word_fill, match_sentence, sequence_audio, order_phrase, "
               "categorization_multiple, fill_blanks_dropdown, match_phrases").arg(m_typeName), 
        this);
    warningLabel->setWordWrap(true);
    warningLabel->setStyleSheet("color: #8B008B; margin: 10px;");
    layout->addWidget(warningLabel);

    QLabel *jsonLabel = new QLabel("📄 Raw JSON Data:", this);
    layout->addWidget(jsonLabel);

    m_jsonEdit = new QTextEdit(this);
    m_jsonEdit->setMinimumHeight(300);
    m_jsonEdit->setStyleSheet("font-family: monospace;");
    layout->addWidget(m_jsonEdit);

    QPushButton *saveButton = new QPushButton("💾 Save JSON", this);
    connect(saveButton, &QPushButton::clicked, [this]() {
        QString jsonText = m_jsonEdit->toPlainText();
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8(), &error);

        if (error.error != QJsonParseError::NoError) {
            QMessageBox::warning(this, "Invalid JSON", 
                "The JSON is not valid. Please check your syntax:\n" + error.errorString());
            return;
        }

        if (!doc.isObject()) {
            QMessageBox::warning(this, "Invalid JSON", 
                "The JSON must be an object representing a question.");
            return;
        }

        m_question = doc.object();
        emit questionChanged();
        QMessageBox::information(this, "Success! 💖", "JSON saved successfully!");
    });
    layout->addWidget(saveButton);

    m_mainLayout->addWidget(warningGroup);
    m_mainLayout->addStretch();
}

void UnknownQuestionEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;
    m_questionEdit->setText(question["question"].toString());

    // Set media fields if they exist
    if (question.contains("media") && question["media"].isObject()) {
        QJsonObject media = question["media"].toObject();
        m_videoEdit->setText(media["video"].toString());
        m_audioEdit->setText(media["audio"].toString());
        m_imageEdit->setText(media["image"].toString());
    }

    // Display the full JSON in the text editor
    QJsonDocument doc(question);
    m_jsonEdit->setPlainText(doc.toJson(QJsonDocument::Indented));
}

QJsonObject UnknownQuestionEditor::getQuestion() const
{
    return m_question;
}

// Additional implementations for missing question editors
// Append this to questioneditor.cpp

// SequenceAudioEditor implementation
SequenceAudioEditor::SequenceAudioEditor(QWidget *parent)
    : QuestionEditor(parent)
    , m_optionsGroup(nullptr)
    , m_optionsLayout(nullptr)
{
    setupUI();
}

void SequenceAudioEditor::setupUI()
{
    m_optionsGroup = new QGroupBox("🎵 Audio Sequence Options (Order by sequence) 🎵", this);
    QVBoxLayout *groupLayout = new QVBoxLayout(m_optionsGroup);
    
    QLabel *infoLabel = new QLabel("💡 Set the correct order for each audio option!", this);
    infoLabel->setStyleSheet("font-style: italic; margin: 5px;");
    groupLayout->addWidget(infoLabel);
    
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollWidget = new QWidget();
    m_optionsLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMaximumHeight(300);
    groupLayout->addWidget(scrollArea);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("➕ Add Option", this);
    QPushButton *saveButton = new QPushButton("💾 Save All Options", this);
    connect(addButton, &QPushButton::clicked, this, &SequenceAudioEditor::addOption);
    connect(saveButton, &QPushButton::clicked, this, &SequenceAudioEditor::saveOptions);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    groupLayout->addLayout(buttonLayout);
    
    m_mainLayout->addWidget(m_optionsGroup);
    m_mainLayout->addStretch();
}

void SequenceAudioEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;
    m_questionEdit->setText(question["question"].toString());
    
    QJsonObject media = question["media"].toObject();
    m_videoEdit->setText(media["video"].toString());
    m_audioEdit->setText(media["audio"].toString());
    m_imageEdit->setText(media["image"].toString());
    
    refreshOptions();
}

QJsonObject SequenceAudioEditor::getQuestion() const
{
    return m_question;
}

void SequenceAudioEditor::refreshOptions()
{
    for (QLineEdit *edit : m_optionEdits) delete edit;
    for (QSpinBox *spin : m_orderSpinBoxes) delete spin;
    
    m_optionEdits.clear();
    m_orderSpinBoxes.clear();
    
    QLayoutItem *item;
    while ((item = m_optionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    QJsonArray audioOptions = m_question["audio_options"].toArray();
    QJsonArray answers = m_question["answer"].toArray();
    
    for (int i = 0; i < audioOptions.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout();
        
        QLabel *orderLabel = new QLabel("Order:", this);
        QSpinBox *orderSpin = new QSpinBox(this);
        orderSpin->setMinimum(1);
        orderSpin->setMaximum(audioOptions.size());
        
        int currentOrder = 1;
        for (int j = 0; j < answers.size(); ++j) {
            if (answers[j].toInt() == i) {
                currentOrder = j + 1;
                break;
            }
        }
        orderSpin->setValue(currentOrder);
        m_orderSpinBoxes.append(orderSpin);
        
        QLabel *label = new QLabel(QString("Audio %1:").arg(i + 1), this);
        QJsonObject optionObj = audioOptions[i].toObject();
        QLineEdit *optionEdit = new QLineEdit(optionObj["option"].toString(), this);
        m_optionEdits.append(optionEdit);
        
        rowLayout->addWidget(orderLabel);
        rowLayout->addWidget(orderSpin);
        rowLayout->addWidget(label);
        rowLayout->addWidget(optionEdit, 1);
        
        if (audioOptions.size() > 1) {
            QPushButton *removeButton = new QPushButton("🗑️", this);
            removeButton->setMaximumWidth(30);
            connect(removeButton, &QPushButton::clicked, this, &SequenceAudioEditor::removeOption);
            rowLayout->addWidget(removeButton);
        }
        
        QWidget *rowWidget = new QWidget();
        rowWidget->setLayout(rowLayout);
        m_optionsLayout->addWidget(rowWidget);
    }
    
    m_optionsLayout->addStretch();
}

void SequenceAudioEditor::addOption()
{
    QJsonArray audioOptions = m_question["audio_options"].toArray();
    QJsonObject newOption;
    newOption["option"] = "New Audio Option";
    audioOptions.append(newOption);
    m_question["audio_options"] = audioOptions;
    
    QJsonArray answers = m_question["answer"].toArray();
    answers.append(audioOptions.size() - 1);
    m_question["answer"] = answers;
    
    refreshOptions();
    emit questionChanged();
}

void SequenceAudioEditor::removeOption()
{
    QJsonArray audioOptions = m_question["audio_options"].toArray();
    if (audioOptions.size() <= 1) return;
    
    audioOptions.removeLast();
    m_question["audio_options"] = audioOptions;
    
    QJsonArray answers = m_question["answer"].toArray();
    QJsonArray newAnswers;
    for (const QJsonValue &ans : answers) {
        int index = ans.toInt();
        if (index < audioOptions.size()) {
            newAnswers.append(index);
        }
    }
    m_question["answer"] = newAnswers;
    
    refreshOptions();
    emit questionChanged();
}

void SequenceAudioEditor::saveOptions()
{
    QJsonArray audioOptions;
    for (QLineEdit *edit : m_optionEdits) {
        QJsonObject optionObj;
        optionObj["option"] = edit->text().trimmed();
        audioOptions.append(optionObj);
    }
    m_question["audio_options"] = audioOptions;
    
    QJsonArray answers;
    QJsonArray newAnswers;
    int targetSize = m_orderSpinBoxes.size();
    for (int i = 0; i < targetSize; ++i) {
        if (i < answers.size()) {
            newAnswers.append(answers[i]);
        } else {
            newAnswers.append(QJsonValue()); // Append default empty values
        }
    }
    answers = newAnswers;
    
    for (int i = 0; i < m_orderSpinBoxes.size(); ++i) {
        int order = m_orderSpinBoxes[i]->value() - 1;
        if (order >= 0 && order < answers.size()) {
            answers[order] = i;
        }
    }
    m_question["answer"] = answers;
    
    emit questionChanged();
    QMessageBox::information(this, "Success! 💖", "Audio sequence saved successfully!");
}

void SequenceAudioEditor::moveOptionUp() {}
void SequenceAudioEditor::moveOptionDown() {}

// MatchSentenceEditor implementation
MatchSentenceEditor::MatchSentenceEditor(QWidget *parent)
    : QuestionEditor(parent)
    , m_pairsGroup(nullptr)
    , m_pairsLayout(nullptr)
{
    setupUI();
}

void MatchSentenceEditor::setupUI()
{
    m_pairsGroup = new QGroupBox("🔗 Sentence-Image Pairs 🔗", this);
    QVBoxLayout *groupLayout = new QVBoxLayout(m_pairsGroup);
    
    QLabel *infoLabel = new QLabel("💡 Create pairs of sentences and their corresponding images!", this);
    infoLabel->setStyleSheet("font-style: italic; margin: 5px;");
    groupLayout->addWidget(infoLabel);
    
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollWidget = new QWidget();
    m_pairsLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMaximumHeight(400);
    groupLayout->addWidget(scrollArea);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("➕ Add Pair", this);
    QPushButton *saveButton = new QPushButton("💾 Save All Pairs", this);
    connect(addButton, &QPushButton::clicked, this, &MatchSentenceEditor::addPair);
    connect(saveButton, &QPushButton::clicked, this, &MatchSentenceEditor::savePairs);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    groupLayout->addLayout(buttonLayout);
    
    m_mainLayout->addWidget(m_pairsGroup);
    m_mainLayout->addStretch();
}

void MatchSentenceEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;
    m_questionEdit->setText(question["question"].toString());
    
    QJsonObject media = question["media"].toObject();
    m_videoEdit->setText(media["video"].toString());
    m_audioEdit->setText(media["audio"].toString());
    m_imageEdit->setText(media["image"].toString());
    
    refreshPairs();
}

QJsonObject MatchSentenceEditor::getQuestion() const
{
    return m_question;
}

void MatchSentenceEditor::refreshPairs()
{
    for (QLineEdit *edit : m_sentenceEdits) delete edit;
    for (QLineEdit *edit : m_imageEdits) delete edit;
    
    m_sentenceEdits.clear();
    m_imageEdits.clear();
    
    QLayoutItem *item;
    while ((item = m_pairsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    QJsonArray pairs = m_question["pairs"].toArray();
    
    for (int i = 0; i < pairs.size(); ++i) {
        QJsonObject pair = pairs[i].toObject();
        
        QVBoxLayout *pairLayout = new QVBoxLayout();
        
        QHBoxLayout *sentenceLayout = new QHBoxLayout();
        QLabel *sentenceLabel = new QLabel(QString("Sentence %1:").arg(i + 1), this);
        QLineEdit *sentenceEdit = new QLineEdit(pair["sentence"].toString(), this);
        m_sentenceEdits.append(sentenceEdit);
        
        sentenceLayout->addWidget(sentenceLabel);
        sentenceLayout->addWidget(sentenceEdit, 1);
        
        QHBoxLayout *imageLayout = new QHBoxLayout();
        QLabel *imageLabel = new QLabel("Image Path:", this);
        QLineEdit *imageEdit = new QLineEdit(pair["image_path"].toString(), this);
        m_imageEdits.append(imageEdit);
        
        QPushButton *browseButton = new QPushButton("📁", this);
        browseButton->setMaximumWidth(30);
        connect(browseButton, &QPushButton::clicked, [this, imageEdit]() {
            QString file = browseForFile("Select Image File",
                "Image files (*.jpg *.jpeg *.png *.gif *.bmp *.svg *.webp);;All files (*.*)");
            if (!file.isEmpty()) imageEdit->setText(file);
        });
        
        imageLayout->addWidget(imageLabel);
        imageLayout->addWidget(imageEdit, 1);
        imageLayout->addWidget(browseButton);
        
        pairLayout->addLayout(sentenceLayout);
        pairLayout->addLayout(imageLayout);
        
        if (pairs.size() > 1) {
            QPushButton *removeButton = new QPushButton("🗑️ Remove Pair", this);
            connect(removeButton, &QPushButton::clicked, this, &MatchSentenceEditor::removePair);
            pairLayout->addWidget(removeButton);
        }
        
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        pairLayout->addWidget(line);
        
        QWidget *pairWidget = new QWidget();
        pairWidget->setLayout(pairLayout);
        m_pairsLayout->addWidget(pairWidget);
    }
    
    m_pairsLayout->addStretch();
}

void MatchSentenceEditor::addPair()
{
    QJsonArray pairs = m_question["pairs"].toArray();
    QJsonObject newPair;
    newPair["sentence"] = "New sentence";
    newPair["image_path"] = "image.jpg";
    pairs.append(newPair);
    m_question["pairs"] = pairs;
    
    refreshPairs();
    emit questionChanged();
}

void MatchSentenceEditor::removePair()
{
    QJsonArray pairs = m_question["pairs"].toArray();
    if (pairs.size() <= 1) return;
    
    pairs.removeLast();
    m_question["pairs"] = pairs;
    
    refreshPairs();
    emit questionChanged();
}

void MatchSentenceEditor::savePairs()
{
    QJsonArray pairs;
    for (int i = 0; i < m_sentenceEdits.size(); ++i) {
        QJsonObject pair;
        pair["sentence"] = m_sentenceEdits[i]->text().trimmed();
        pair["image_path"] = m_imageEdits[i]->text().trimmed();
        pairs.append(pair);
    }
    m_question["pairs"] = pairs;
    
    QJsonObject answer;
    for (int i = 0; i < m_sentenceEdits.size(); ++i) {
        QString imagePath = m_imageEdits[i]->text().trimmed();
        QString sentence = m_sentenceEdits[i]->text().trimmed();
        answer[imagePath] = sentence;
    }
    m_question["answer"] = answer;
    
    emit questionChanged();
    QMessageBox::information(this, "Success! 💖", "Sentence-image pairs saved successfully!");
}

void MatchSentenceEditor::browseImage() {}

// FillBlanksDropdownEditor implementation
FillBlanksDropdownEditor::FillBlanksDropdownEditor(QWidget *parent)
    : QuestionEditor(parent)
    , m_partsGroup(nullptr)
    , m_optionsGroup(nullptr)
    , m_partsLayout(nullptr)
    , m_optionsLayout(nullptr)
{
    setupUI();
}

void FillBlanksDropdownEditor::setupUI()
{
    m_partsGroup = new QGroupBox("📝 Sentence Parts 📝", this);
    QVBoxLayout *partsGroupLayout = new QVBoxLayout(m_partsGroup);
    
    QLabel *partsInfo = new QLabel("💡 Text parts that appear between dropdown blanks", this);
    partsInfo->setStyleSheet("font-style: italic; margin: 5px;");
    partsGroupLayout->addWidget(partsInfo);
    
    QScrollArea *partsScrollArea = new QScrollArea(this);
    QWidget *partsScrollWidget = new QWidget();
    m_partsLayout = new QVBoxLayout(partsScrollWidget);
    partsScrollArea->setWidget(partsScrollWidget);
    partsScrollArea->setWidgetResizable(true);
    partsScrollArea->setMaximumHeight(150);
    partsGroupLayout->addWidget(partsScrollArea);
    
    m_optionsGroup = new QGroupBox("⬇️ Dropdown Options ⬇️", this);
    QVBoxLayout *optionsGroupLayout = new QVBoxLayout(m_optionsGroup);
    
    QLabel *optionsInfo = new QLabel("💡 Set dropdown options and correct answers for each blank", this);
    optionsInfo->setStyleSheet("font-style: italic; margin: 5px;");
    optionsGroupLayout->addWidget(optionsInfo);
    
    QScrollArea *optionsScrollArea = new QScrollArea(this);
    QWidget *optionsScrollWidget = new QWidget();
    m_optionsLayout = new QVBoxLayout(optionsScrollWidget);
    optionsScrollArea->setWidget(optionsScrollWidget);
    optionsScrollArea->setWidgetResizable(true);
    optionsScrollArea->setMaximumHeight(300);
    optionsGroupLayout->addWidget(optionsScrollArea);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addPartButton = new QPushButton("➕ Add Part", this);
    QPushButton *addBlankButton = new QPushButton("➕ Add Blank", this);
    QPushButton *saveAllButton = new QPushButton("💾 Save Everything", this);
    
    connect(addPartButton, &QPushButton::clicked, this, &FillBlanksDropdownEditor::addSentencePart);
    connect(addBlankButton, &QPushButton::clicked, this, &FillBlanksDropdownEditor::addBlankOption);
    connect(saveAllButton, &QPushButton::clicked, this, &FillBlanksDropdownEditor::saveAll);
    
    buttonLayout->addWidget(addPartButton);
    buttonLayout->addWidget(addBlankButton);
    buttonLayout->addWidget(saveAllButton);
    buttonLayout->addStretch();
    optionsGroupLayout->addLayout(buttonLayout);
    
    m_mainLayout->addWidget(m_partsGroup);
    m_mainLayout->addWidget(m_optionsGroup);
    m_mainLayout->addStretch();
}

void FillBlanksDropdownEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;
    m_questionEdit->setText(question["question"].toString());
    
    QJsonObject media = question["media"].toObject();
    m_videoEdit->setText(media["video"].toString());
    m_audioEdit->setText(media["audio"].toString());
    m_imageEdit->setText(media["image"].toString());
    
    refreshSentenceParts();
    refreshBlankOptions();
}

QJsonObject FillBlanksDropdownEditor::getQuestion() const
{
    return m_question;
}

void FillBlanksDropdownEditor::refreshSentenceParts()
{
    for (QLineEdit *edit : m_partEdits) delete edit;
    m_partEdits.clear();
    
    QLayoutItem *item;
    while ((item = m_partsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    QJsonArray parts = m_question["sentence_parts"].toArray();
    
    for (int i = 0; i < parts.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout();
        
        QLabel *label = new QLabel(QString("Part %1:").arg(i + 1), this);
        QLineEdit *partEdit = new QLineEdit(parts[i].toString(), this);
        m_partEdits.append(partEdit);
        
        rowLayout->addWidget(label);
        rowLayout->addWidget(partEdit, 1);
        
        if (parts.size() > 1) {
            QPushButton *removeButton = new QPushButton("🗑️", this);
            removeButton->setMaximumWidth(30);
            connect(removeButton, &QPushButton::clicked, this, &FillBlanksDropdownEditor::removeSentencePart);
            rowLayout->addWidget(removeButton);
        }
        
        QWidget *rowWidget = new QWidget();
        rowWidget->setLayout(rowLayout);
        m_partsLayout->addWidget(rowWidget);
    }
    
    m_partsLayout->addStretch();
}

void FillBlanksDropdownEditor::refreshBlankOptions()
{
    for (auto &optionList : m_blankOptionEdits) {
        for (QLineEdit *edit : optionList) delete edit;
    }
    for (QComboBox *combo : m_correctAnswerCombos) delete combo;
    
    m_blankOptionEdits.clear();
    m_correctAnswerCombos.clear();
    
    QLayoutItem *item;
    while ((item = m_optionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    QJsonArray optionsForBlanks = m_question["options_for_blanks"].toArray();
    QJsonArray answers = m_question["answers"].toArray();
    
    for (int i = 0; i < optionsForBlanks.size(); ++i) {
        QVBoxLayout *blankLayout = new QVBoxLayout();
        
        QLabel *blankLabel = new QLabel(QString("Blank %1 Options:").arg(i + 1), this);
        blankLabel->setStyleSheet("font-weight: bold;");
        blankLayout->addWidget(blankLabel);
        
        QJsonArray options = optionsForBlanks[i].toArray();
        QList<QLineEdit*> currentBlankEdits;
        
        for (int j = 0; j < options.size(); ++j) {
            QHBoxLayout *optionLayout = new QHBoxLayout();
            
            QLabel *optionLabel = new QLabel(QString("Option %1:").arg(j + 1), this);
            QLineEdit *optionEdit = new QLineEdit(options[j].toString(), this);
            currentBlankEdits.append(optionEdit);
            
            optionLayout->addWidget(optionLabel);
            optionLayout->addWidget(optionEdit, 1);
            
            QWidget *optionWidget = new QWidget();
            optionWidget->setLayout(optionLayout);
            blankLayout->addWidget(optionWidget);
        }
        
        m_blankOptionEdits.append(currentBlankEdits);
        
        QHBoxLayout *correctLayout = new QHBoxLayout();
        QLabel *correctLabel = new QLabel("Correct Answer:", this);
        QComboBox *correctCombo = new QComboBox(this);
        
        for (int j = 0; j < options.size(); ++j) {
            correctCombo->addItem(options[j].toString());
        }
        
        if (i < answers.size()) {
            QString correctAnswer = answers[i].toString();
            int correctIndex = correctCombo->findText(correctAnswer);
            if (correctIndex >= 0) {
                correctCombo->setCurrentIndex(correctIndex);
            }
        }
        
        m_correctAnswerCombos.append(correctCombo);
        
        correctLayout->addWidget(correctLabel);
        correctLayout->addWidget(correctCombo);
        correctLayout->addStretch();
        
        QWidget *correctWidget = new QWidget();
        correctWidget->setLayout(correctLayout);
        blankLayout->addWidget(correctWidget);
        
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        blankLayout->addWidget(line);
        
        QWidget *blankWidget = new QWidget();
        blankWidget->setLayout(blankLayout);
        m_optionsLayout->addWidget(blankWidget);
    }
    
    m_optionsLayout->addStretch();
}

void FillBlanksDropdownEditor::addSentencePart()
{
    QJsonArray parts = m_question["sentence_parts"].toArray();
    parts.append("New part");
    m_question["sentence_parts"] = parts;
    
    refreshSentenceParts();
    emit questionChanged();
}

void FillBlanksDropdownEditor::removeSentencePart()
{
    QJsonArray parts = m_question["sentence_parts"].toArray();
    if (parts.size() <= 1) return;
    
    parts.removeLast();
    m_question["sentence_parts"] = parts;
    
    refreshSentenceParts();
    emit questionChanged();
}

void FillBlanksDropdownEditor::addBlankOption()
{
    QJsonArray optionsForBlanks = m_question["options_for_blanks"].toArray();
    QJsonArray newOptions;
    newOptions.append(" ");
    newOptions.append("option1");
    newOptions.append("option2");
    optionsForBlanks.append(newOptions);
    m_question["options_for_blanks"] = optionsForBlanks;
    
    QJsonArray answers = m_question["answers"].toArray();
    answers.append("option1");
    m_question["answers"] = answers;
    
    refreshBlankOptions();
    emit questionChanged();
}

void FillBlanksDropdownEditor::removeBlankOption()
{
    QJsonArray optionsForBlanks = m_question["options_for_blanks"].toArray();
    if (optionsForBlanks.size() <= 1) return;
    
    optionsForBlanks.removeLast();
    m_question["options_for_blanks"] = optionsForBlanks;
    
    QJsonArray answers = m_question["answers"].toArray();
    if (answers.size() > 0) {
        answers.removeLast();
        m_question["answers"] = answers;
    }
    
    refreshBlankOptions();
    emit questionChanged();
}

void FillBlanksDropdownEditor::saveAll()
{
    QJsonArray parts;
    for (QLineEdit *edit : m_partEdits) {
        parts.append(edit->text());
    }
    m_question["sentence_parts"] = parts;
    
    QJsonArray optionsForBlanks;
    for (const auto &optionList : m_blankOptionEdits) {
        QJsonArray options;
        for (QLineEdit *edit : optionList) {
            options.append(edit->text());
        }
        optionsForBlanks.append(options);
    }
    m_question["options_for_blanks"] = optionsForBlanks;
    
    QJsonArray answers;
    for (QComboBox *combo : m_correctAnswerCombos) {
        answers.append(combo->currentText());
    }
    m_question["answers"] = answers;
    
    emit questionChanged();
    QMessageBox::information(this, "Success! 💖", "Fill blanks dropdown saved successfully!");
}

// OrderPhraseEditor implementation
OrderPhraseEditor::OrderPhraseEditor(QWidget *parent)
    : QuestionEditor(parent)
    , m_phrasesGroup(nullptr)
    , m_phrasesLayout(nullptr)
{
    setupUI();
}

void OrderPhraseEditor::setupUI()
{
    m_phrasesGroup = new QGroupBox("📋 Phrase Ordering 📋", this);
    QVBoxLayout *groupLayout = new QVBoxLayout(m_phrasesGroup);
    
    QLabel *infoLabel = new QLabel("💡 Set the correct order for each phrase!", this);
    infoLabel->setStyleSheet("font-style: italic; margin: 5px;");
    groupLayout->addWidget(infoLabel);
    
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollWidget = new QWidget();
    m_phrasesLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMaximumHeight(400);
    groupLayout->addWidget(scrollArea);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("➕ Add Phrase", this);
    QPushButton *saveButton = new QPushButton("💾 Save All Phrases", this);
    connect(addButton, &QPushButton::clicked, this, &OrderPhraseEditor::addPhrase);
    connect(saveButton, &QPushButton::clicked, this, &OrderPhraseEditor::savePhrases);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    groupLayout->addLayout(buttonLayout);
    
    m_mainLayout->addWidget(m_phrasesGroup);
    m_mainLayout->addStretch();
}

void OrderPhraseEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;
    m_questionEdit->setText(question["question"].toString());
    
    QJsonObject media = question["media"].toObject();
    m_videoEdit->setText(media["video"].toString());
    m_audioEdit->setText(media["audio"].toString());
    m_imageEdit->setText(media["image"].toString());
    
    refreshPhrases();
}

QJsonObject OrderPhraseEditor::getQuestion() const
{
    return m_question;
}

void OrderPhraseEditor::refreshPhrases()
{
    for (QLineEdit *edit : m_phraseEdits) delete edit;
    for (QSpinBox *spin : m_orderSpinBoxes) delete spin;
    
    m_phraseEdits.clear();
    m_orderSpinBoxes.clear();
    
    QLayoutItem *item;
    while ((item = m_phrasesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    QJsonArray shuffledPhrases = m_question["phrase_shuffled"].toArray();
    QJsonArray correctOrder = m_question["answer"].toArray();
    
    for (int i = 0; i < shuffledPhrases.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout();
        
        QLabel *orderLabel = new QLabel("Order:", this);
        QSpinBox *orderSpin = new QSpinBox(this);
        orderSpin->setMinimum(1);
        orderSpin->setMaximum(shuffledPhrases.size());
        
        QString currentPhrase = shuffledPhrases[i].toString();
        int currentOrder = 1;
        for (int j = 0; j < correctOrder.size(); ++j) {
            if (correctOrder[j].toString() == currentPhrase) {
                currentOrder = j + 1;
                break;
            }
        }
        orderSpin->setValue(currentOrder);
        m_orderSpinBoxes.append(orderSpin);
        
        QLabel *label = new QLabel(QString("Phrase %1:").arg(i + 1), this);
        QLineEdit *phraseEdit = new QLineEdit(currentPhrase, this);
        m_phraseEdits.append(phraseEdit);
        
        rowLayout->addWidget(orderLabel);
        rowLayout->addWidget(orderSpin);
        rowLayout->addWidget(label);
        rowLayout->addWidget(phraseEdit, 1);
        
        if (shuffledPhrases.size() > 1) {
            QPushButton *removeButton = new QPushButton("🗑️", this);
            removeButton->setMaximumWidth(30);
            connect(removeButton, &QPushButton::clicked, this, &OrderPhraseEditor::removePhrase);
            rowLayout->addWidget(removeButton);
        }
        
        QWidget *rowWidget = new QWidget();
        rowWidget->setLayout(rowLayout);
        m_phrasesLayout->addWidget(rowWidget);
    }
    
    m_phrasesLayout->addStretch();
}

void OrderPhraseEditor::addPhrase()
{
    QJsonArray phrases = m_question["phrase_shuffled"].toArray();
    phrases.append("New phrase");
    m_question["phrase_shuffled"] = phrases;
    
    QJsonArray answer = m_question["answer"].toArray();
    answer.append("New phrase");
    m_question["answer"] = answer;
    
    refreshPhrases();
    emit questionChanged();
}

void OrderPhraseEditor::removePhrase()
{
    QJsonArray phrases = m_question["phrase_shuffled"].toArray();
    if (phrases.size() <= 1) return;
    
    QString removedPhrase = phrases.last().toString();
    phrases.removeAt(phrases.size() - 1);
    m_question["phrase_shuffled"] = phrases;
    
    QJsonArray answer = m_question["answer"].toArray();
    for (int i = 0; i < answer.size(); ++i) {
        if (answer[i].toString() == removedPhrase) {
            answer.removeAt(i);
            break;
        }
    }
    m_question["answer"] = answer;
    
    refreshPhrases();
    emit questionChanged();
}

void OrderPhraseEditor::savePhrases()
{
    QJsonArray shuffledPhrases;
    for (QLineEdit *edit : m_phraseEdits) {
        shuffledPhrases.append(edit->text().trimmed());
    }
    m_question["phrase_shuffled"] = shuffledPhrases;
    
    QJsonArray correctOrder;
    QJsonArray newCorrectOrder;
    int size = m_phraseEdits.size();
    for (int i = 0; i < size; ++i) {
        if (i < correctOrder.size()) {
            newCorrectOrder.append(correctOrder[i]);
        } else {
            newCorrectOrder.append(QJsonValue());
        }
    }
    correctOrder = newCorrectOrder;
    
    for (int i = 0; i < m_orderSpinBoxes.size(); ++i) {
        int order = m_orderSpinBoxes[i]->value() - 1;
        if (order >= 0 && order < correctOrder.size()) {
            correctOrder[order] = m_phraseEdits[i]->text().trimmed();
        }
    }
    m_question["answer"] = correctOrder;
    
    emit questionChanged();
    QMessageBox::information(this, "Success! 💖", "Phrase order saved successfully!");
}

void OrderPhraseEditor::moveUp() {}
void OrderPhraseEditor::moveDown() {}

// CategorizationMultipleEditor implementation
CategorizationMultipleEditor::CategorizationMultipleEditor(QWidget *parent)
    : QuestionEditor(parent)
    , m_stimuliGroup(nullptr)
    , m_categoriesGroup(nullptr)
    , m_stimuliLayout(nullptr)
    , m_categoriesLayout(nullptr)
{
    setupUI();
}

void CategorizationMultipleEditor::setupUI()
{
    m_stimuliGroup = new QGroupBox("📂 Stimuli (Items to categorize) 📂", this);
    QVBoxLayout *stimuliGroupLayout = new QVBoxLayout(m_stimuliGroup);
    
    QLabel *stimuliInfo = new QLabel("💡 Items that students will categorize", this);
    stimuliInfo->setStyleSheet("font-style: italic; margin: 5px;");
    stimuliGroupLayout->addWidget(stimuliInfo);
    
    QScrollArea *stimuliScrollArea = new QScrollArea(this);
    QWidget *stimuliScrollWidget = new QWidget();
    m_stimuliLayout = new QVBoxLayout(stimuliScrollWidget);
    stimuliScrollArea->setWidget(stimuliScrollWidget);
    stimuliScrollArea->setWidgetResizable(true);
    stimuliScrollArea->setMaximumHeight(200);
    stimuliGroupLayout->addWidget(stimuliScrollArea);
    
    m_categoriesGroup = new QGroupBox("📋 Categories 📋", this);
    QVBoxLayout *categoriesGroupLayout = new QVBoxLayout(m_categoriesGroup);
    
    QLabel *categoriesInfo = new QLabel("💡 Available categories for classification", this);
    categoriesInfo->setStyleSheet("font-style: italic; margin: 5px;");
    categoriesGroupLayout->addWidget(categoriesInfo);
    
    QScrollArea *categoriesScrollArea = new QScrollArea(this);
    QWidget *categoriesScrollWidget = new QWidget();
    m_categoriesLayout = new QVBoxLayout(categoriesScrollWidget);
    categoriesScrollArea->setWidget(categoriesScrollWidget);
    categoriesScrollArea->setWidgetResizable(true);
    categoriesScrollArea->setMaximumHeight(150);
    categoriesGroupLayout->addWidget(categoriesScrollArea);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addStimulusButton = new QPushButton("➕ Add Stimulus", this);
    QPushButton *addCategoryButton = new QPushButton("➕ Add Category", this);
    QPushButton *saveAllButton = new QPushButton("💾 Save Everything", this);
    
    connect(addStimulusButton, &QPushButton::clicked, this, &CategorizationMultipleEditor::addStimulus);
    connect(addCategoryButton, &QPushButton::clicked, this, &CategorizationMultipleEditor::addCategory);
    connect(saveAllButton, &QPushButton::clicked, this, &CategorizationMultipleEditor::saveAll);
    
    buttonLayout->addWidget(addStimulusButton);
    buttonLayout->addWidget(addCategoryButton);
    buttonLayout->addWidget(saveAllButton);
    buttonLayout->addStretch();
    categoriesGroupLayout->addLayout(buttonLayout);
    
    m_mainLayout->addWidget(m_stimuliGroup);
    m_mainLayout->addWidget(m_categoriesGroup);
    m_mainLayout->addStretch();
}

void CategorizationMultipleEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;
    m_questionEdit->setText(question["question"].toString());
    
    QJsonObject media = question["media"].toObject();
    m_videoEdit->setText(media["video"].toString());
    m_audioEdit->setText(media["audio"].toString());
    m_imageEdit->setText(media["image"].toString());
    
    refreshStimuli();
    refreshCategories();
}

QJsonObject CategorizationMultipleEditor::getQuestion() const
{
    return m_question;
}

void CategorizationMultipleEditor::refreshStimuli()
{
    // Clear previous widgets
    for (auto &sw : m_stimuliWidgets) {
        if (sw.containerWidget) {
            delete sw.containerWidget;
        }
    }
    m_stimuliWidgets.clear();

    QLayoutItem *item;
    while ((item = m_stimuliLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray stimuli = m_question["stimuli"].toArray();
    QJsonArray categories = m_question["categories"].toArray();
    QJsonObject answer = m_question["answer"].toObject();

    for (int i = 0; i < stimuli.size(); ++i) {
        QJsonObject stimulus = stimuli[i].toObject();

        QString imagePath = stimulus.value("image").toString();
        QString text = stimulus.value("text").toString();

        bool isImage = !imagePath.isEmpty();

        // Create UI container
        QWidget *container = new QWidget(this);
        QHBoxLayout *layout = new QHBoxLayout(container);

        QLabel *itemLabel = new QLabel(QString("Item %1:").arg(i + 1), this);
        layout->addWidget(itemLabel);

        // Radio buttons for mode selection
        QRadioButton *textRadio = new QRadioButton("Text", this);
        QRadioButton *imageRadio = new QRadioButton("Image", this);
        layout->addWidget(textRadio);
        layout->addWidget(imageRadio);

        // Text input
        QLineEdit *textEdit = new QLineEdit(this);
        textEdit->setPlaceholderText("Enter text here");
        textEdit->setText(text);
        layout->addWidget(textEdit);

        // Image path input + browse button
        QLineEdit *imagePathEdit = new QLineEdit(this);
        imagePathEdit->setPlaceholderText("Select image path");
        imagePathEdit->setText(imagePath);
        QPushButton *browseButton = new QPushButton("📁", this);
        layout->addWidget(imagePathEdit);
        layout->addWidget(browseButton);

        // Category selector
        QLabel *categoryLabel = new QLabel("Category:", this);
        QComboBox *categoryCombo = new QComboBox(this);
        for (const auto &cat : categories) {
            categoryCombo->addItem(cat.toString());
        }

        // Lookup answer by filename key for image, or by text otherwise
        QString key;
        if (isImage) {
            key = QFileInfo(imagePath).fileName();  // Filename only
        } else {
            key = text;
        }
        QString category = answer.value(key).toString();
        int catIndex = categoryCombo->findText(category);
        if (catIndex >= 0) {
            categoryCombo->setCurrentIndex(catIndex);
        }

        layout->addWidget(categoryLabel);
        layout->addWidget(categoryCombo);

        // Set visibility based on mode
        textRadio->setChecked(!isImage);
        imageRadio->setChecked(isImage);
        textEdit->setVisible(!isImage);
        imagePathEdit->setVisible(isImage);
        browseButton->setVisible(isImage);

        // Connect browse button
        connect(browseButton, &QPushButton::clicked, this, [this, i]() {
            browseImageForStimulus(i);
        });

        // Connect radio toggles to switch modes
        connect(textRadio, &QRadioButton::toggled, this, [this, i](bool checked){
            if (checked) toggleStimulusInputMode(i, false);
        });
        connect(imageRadio, &QRadioButton::toggled, this, [this, i](bool checked){
            if (checked) toggleStimulusInputMode(i, true);
        });

        // Save widgets for later
        m_stimuliWidgets.append(StimulusWidget{ textRadio, imageRadio, textEdit, imagePathEdit, browseButton, container, categoryCombo });

        m_stimuliLayout->addWidget(container);
    }
    m_stimuliLayout->addStretch();
}

void CategorizationMultipleEditor::toggleStimulusInputMode(int index, bool useImage)
{
    if (index < 0 || index >= m_stimuliWidgets.size()) return;
    auto &sw = m_stimuliWidgets[index];

    sw.textEdit->setVisible(!useImage);
    sw.imagePathEdit->setVisible(useImage);
    sw.browseButton->setVisible(useImage);

    emit questionChanged();
}

void CategorizationMultipleEditor::browseImageForStimulus(int index)
{
    if (index < 0 || index >= m_stimuliWidgets.size()) return;
    auto &sw = m_stimuliWidgets[index];

    QString file = QFileDialog::getOpenFileName(this, "Select Image",
                                                QString(),
                                                "Image files (*.jpg *.jpeg *.png *.gif *.bmp *.svg *.webp);;All files (*.*)");
    if (!file.isEmpty()) {
        sw.imagePathEdit->setText(file);
        emit questionChanged();
    }
}


void CategorizationMultipleEditor::refreshCategories()
{
    for (QLineEdit *edit : m_categoryEdits) delete edit;
    m_categoryEdits.clear();
    
    QLayoutItem *item;
    while ((item = m_categoriesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    QJsonArray categories = m_question["categories"].toArray();
    
    for (int i = 0; i < categories.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout();
        
        QLabel *label = new QLabel(QString("Category %1:").arg(i + 1), this);
        QLineEdit *categoryEdit = new QLineEdit(categories[i].toString(), this);
        m_categoryEdits.append(categoryEdit);
        
        rowLayout->addWidget(label);
        rowLayout->addWidget(categoryEdit, 1);
        
        if (categories.size() > 2) { // Keep at least empty + one category
            QPushButton *removeButton = new QPushButton("🗑️", this);
            removeButton->setMaximumWidth(30);
            connect(removeButton, &QPushButton::clicked, this, &CategorizationMultipleEditor::removeCategory);
            rowLayout->addWidget(removeButton);
        }
        
        QWidget *rowWidget = new QWidget();
        rowWidget->setLayout(rowLayout);
        m_categoriesLayout->addWidget(rowWidget);
    }
    
    m_categoriesLayout->addStretch();
}

void CategorizationMultipleEditor::addStimulus()
{
    QJsonArray stimuli = m_question["stimuli"].toArray();
    QJsonObject newStimulus;
    newStimulus["text"] = "New item";
    newStimulus["image"] = QJsonValue::Null;
    stimuli.append(newStimulus);
    m_question["stimuli"] = stimuli;
    
    refreshStimuli();
    emit questionChanged();
}

void CategorizationMultipleEditor::removeStimulus()
{
    QJsonArray stimuli = m_question["stimuli"].toArray();
    if (stimuli.size() <= 1) return;
    
    stimuli.removeLast();
    m_question["stimuli"] = stimuli;
    
    refreshStimuli();
    emit questionChanged();
}

void CategorizationMultipleEditor::addCategory()
{
    QJsonArray categories = m_question["categories"].toArray();
    categories.append("New category");
    m_question["categories"] = categories;
    
    refreshCategories();
    refreshStimuli(); // Refresh stimuli to update category combos
    emit questionChanged();
}

void CategorizationMultipleEditor::removeCategory()
{
    QJsonArray categories = m_question["categories"].toArray();
    if (categories.size() <= 2) return; // Keep at least empty + one category
    
    categories.removeLast();
    m_question["categories"] = categories;
    
    refreshCategories();
    refreshStimuli(); // Refresh stimuli to update category combos
    emit questionChanged();
}

void CategorizationMultipleEditor::saveAll()
{
    // Save stimuli with text or image
    QJsonArray stimuli;
    for (const auto &sw : m_stimuliWidgets) {
        QJsonObject stimulus;
        if (sw.imageRadio->isChecked()) {
            stimulus["image"] = sw.imagePathEdit->text().trimmed();
            stimulus["text"] = QJsonValue::Null;
        } else {
            stimulus["text"] = sw.textEdit->text().trimmed();
            stimulus["image"] = QJsonValue::Null;
        }
        stimuli.append(stimulus);
    }
    m_question["stimuli"] = stimuli;

    // Save categories
    QJsonArray categories;
    for (QLineEdit *edit : m_categoryEdits) {
        categories.append(edit->text().trimmed());
    }
    m_question["categories"] = categories;

    // Save answers with keys normalized to filename for images
    QJsonObject answer;
    for (const auto &sw : m_stimuliWidgets) {
        QString key;
        if (sw.imageRadio->isChecked()) {
            key = QFileInfo(sw.imagePathEdit->text().trimmed()).fileName();
        } else {
            key = sw.textEdit->text().trimmed();
        }
        if (!key.isEmpty()) {
            answer[key] = sw.categoryCombo->currentText();
        }
    }
    m_question["answer"] = answer;

    emit questionChanged();

    QMessageBox::information(this, "Success! 💖", "Categorization saved successfully!");
}


// MatchPhrasesEditor implementation
MatchPhrasesEditor::MatchPhrasesEditor(QWidget *parent)
    : QuestionEditor(parent)
    , m_pairsGroup(nullptr)
    , m_pairsLayout(nullptr)
{
    setupUI();
}

void MatchPhrasesEditor::setupUI()
{
    m_pairsGroup = new QGroupBox("🔗 Phrase Matching Pairs 🔗", this);
    QVBoxLayout *groupLayout = new QVBoxLayout(m_pairsGroup);
    
    QLabel *infoLabel = new QLabel("💡 Create source phrases and their possible targets!", this);
    infoLabel->setStyleSheet("font-style: italic; margin: 5px;");
    groupLayout->addWidget(infoLabel);
    
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollWidget = new QWidget();
    m_pairsLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMaximumHeight(500);
    groupLayout->addWidget(scrollArea);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("➕ Add Pair", this);
    QPushButton *saveButton = new QPushButton("💾 Save All Pairs", this);
    connect(addButton, &QPushButton::clicked, this, &MatchPhrasesEditor::addPair);
    connect(saveButton, &QPushButton::clicked, this, &MatchPhrasesEditor::saveAll);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    groupLayout->addLayout(buttonLayout);
    
    m_mainLayout->addWidget(m_pairsGroup);
    m_mainLayout->addStretch();
}

void MatchPhrasesEditor::setQuestion(const QJsonObject &question)
{
    m_question = question;
    m_questionEdit->setText(question["question"].toString());
    
    QJsonObject media = question["media"].toObject();
    m_videoEdit->setText(media["video"].toString());
    m_audioEdit->setText(media["audio"].toString());
    m_imageEdit->setText(media["image"].toString());
    
    refreshPairs();
}

QJsonObject MatchPhrasesEditor::getQuestion() const
{
    return m_question;
}

void MatchPhrasesEditor::refreshPairs()
{
    for (QLineEdit *edit : m_sourceEdits) delete edit;
    for (auto &targetList : m_targetEdits) {
        for (QLineEdit *edit : targetList) delete edit;
    }
    for (QComboBox *combo : m_correctTargetCombo) delete combo;
    
    m_sourceEdits.clear();
    m_targetEdits.clear();
    m_correctTargetCombo.clear();
    
    QLayoutItem *item;
    while ((item = m_pairsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    QJsonArray pairs = m_question["pairs"].toArray();
    QJsonObject answer = m_question["answer"].toObject();
    
    for (int i = 0; i < pairs.size(); ++i) {
        QJsonObject pair = pairs[i].toObject();
        
        QVBoxLayout *pairLayout = new QVBoxLayout();
        
        // Source phrase
        QHBoxLayout *sourceLayout = new QHBoxLayout();
        QLabel *sourceLabel = new QLabel(QString("Source %1:").arg(i + 1), this);
        QLineEdit *sourceEdit = new QLineEdit(pair["source"].toString(), this);
        m_sourceEdits.append(sourceEdit);
        
        sourceLayout->addWidget(sourceLabel);
        sourceLayout->addWidget(sourceEdit, 1);
        pairLayout->addLayout(sourceLayout);
        
        // Target options
        QLabel *targetsLabel = new QLabel("Target Options:", this);
        targetsLabel->setStyleSheet("font-weight: bold; margin-top: 5px;");
        pairLayout->addWidget(targetsLabel);
        
        QJsonArray targets = pair["targets"].toArray();
        QList<QLineEdit*> currentTargetEdits;
        
        for (int j = 0; j < targets.size(); ++j) {
            QHBoxLayout *targetLayout = new QHBoxLayout();
            
            QLabel *targetLabel = new QLabel(QString("Target %1:").arg(j + 1), this);
            QLineEdit *targetEdit = new QLineEdit(targets[j].toString(), this);
            currentTargetEdits.append(targetEdit);
            
            targetLayout->addWidget(targetLabel);
            targetLayout->addWidget(targetEdit, 1);
            
            QWidget *targetWidget = new QWidget();
            targetWidget->setLayout(targetLayout);
            pairLayout->addWidget(targetWidget);
        }
        
        m_targetEdits.append(currentTargetEdits);
        
        // Correct target combo
        QHBoxLayout *correctLayout = new QHBoxLayout();
        QLabel *correctLabel = new QLabel("Correct Target:", this);
        QComboBox *correctCombo = new QComboBox(this);
        
        for (int j = 0; j < targets.size(); ++j) {
            correctCombo->addItem(targets[j].toString());
        }
        
        QString correctTarget = answer[pair["source"].toString()].toString();
        int correctIndex = correctCombo->findText(correctTarget);
        if (correctIndex >= 0) {
            correctCombo->setCurrentIndex(correctIndex);
        }
        
        m_correctTargetCombo.append(correctCombo);
        
        correctLayout->addWidget(correctLabel);
        correctLayout->addWidget(correctCombo);
        correctLayout->addStretch();
        
        QWidget *correctWidget = new QWidget();
        correctWidget->setLayout(correctLayout);
        pairLayout->addWidget(correctWidget);
        
        // Remove button
        if (pairs.size() > 1) {
            QPushButton *removeButton = new QPushButton("🗑️ Remove Pair", this);
            connect(removeButton, &QPushButton::clicked, this, &MatchPhrasesEditor::removePair);
            pairLayout->addWidget(removeButton);
        }
        
        // Add separator
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        pairLayout->addWidget(line);
        
        QWidget *pairWidget = new QWidget();
        pairWidget->setLayout(pairLayout);
        m_pairsLayout->addWidget(pairWidget);
    }
    
    m_pairsLayout->addStretch();
}

void MatchPhrasesEditor::addPair()
{
    QJsonArray pairs = m_question["pairs"].toArray();
    QJsonObject newPair;
    newPair["source"] = "New source phrase";
    
    QJsonArray targets;
    targets.append(" ");
    targets.append("target1");
    targets.append("target2");
    newPair["targets"] = targets;
    
    pairs.append(newPair);
    m_question["pairs"] = pairs;
    
    refreshPairs();
    emit questionChanged();
}

void MatchPhrasesEditor::removePair()
{
    QJsonArray pairs = m_question["pairs"].toArray();
    if (pairs.size() <= 1) return;
    
    pairs.removeLast();
    m_question["pairs"] = pairs;
    
    refreshPairs();
    emit questionChanged();
}

void MatchPhrasesEditor::addTarget() {}
void MatchPhrasesEditor::removeTarget() {}

void MatchPhrasesEditor::saveAll()
{
    // Save pairs
    QJsonArray pairs;
    for (int i = 0; i < m_sourceEdits.size(); ++i) {
        QJsonObject pair;
        pair["source"] = m_sourceEdits[i]->text().trimmed();
        
        QJsonArray targets;
        for (QLineEdit *edit : m_targetEdits[i]) {
            targets.append(edit->text().trimmed());
        }
        pair["targets"] = targets;
        
        pairs.append(pair);
    }
    m_question["pairs"] = pairs;
    
    // Save answers (source -> correct target mapping)
    QJsonObject answer;
    for (int i = 0; i < m_sourceEdits.size(); ++i) {
        QString source = m_sourceEdits[i]->text().trimmed();
        QString correctTarget = m_correctTargetCombo[i]->currentText();
        answer[source] = correctTarget;
    }
    m_question["answer"] = answer;
    
    emit questionChanged();
    QMessageBox::information(this, "Success! 💖", "Phrase matching pairs saved successfully!");
}
