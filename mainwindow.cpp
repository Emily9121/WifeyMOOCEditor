#include "mainwindow.h"
#include "./ui_mainwindow.h" // Corrected the file extension! So sorry! ✨

#include "editors/mcqsingleeditor.h"
#include "editors/mcqmultipleeditor.h"
#include "editors/wordfilleditor.h"
#include "editors/orderphraseeditor.h"
#include "editors/matchphraseseditor.h"
#include "editors/categorizationeditor.h"
#include "editors/listpickeditor.h"
#include "editors/imagetaggingeditor.h"
#include "editors/matchsentenceeditor.h"
#include "editors/sequenceaudioeditor.h"
#include "editors/fillblanksdropdowneditor.h"
#include "editors/multiquestionseditor.h"

#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QRandomGenerator> // For modern shuffling! ✨
#include <algorithm>      // For std::shuffle!
#include "helpers.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentQuestionIndex(-1)
{
    setWindowTitle("💖 Wifey MOOC 2.1 Editor C++ Edition 💖");
    setMinimumSize(1200, 800);
    setupMainLayout();
    createActions();
    createMenus();
    applyStylesheet();

    newButton = findChild<QPushButton*>("newButton");
    saveButton = findChild<QPushButton*>("saveButton");
    deleteButton = findChild<QPushButton*>("deleteButton");
    questionListWidget = findChild<QListWidget*>("questionList");
    questionTypeSelector = findChild<QComboBox*>("questionTypeSelector");

    QFrame* mainEditorFrame = findChild<QFrame*>("mainEditorFrame");
    if (mainEditorFrame) {
        mainEditorFrameLayout = qobject_cast<QVBoxLayout*>(mainEditorFrame->layout());
    }

    if (newButton) connect(newButton, &QPushButton::clicked, this, &MainWindow::onAddQuestion);
    if (saveButton) connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveFile);
    if (deleteButton) connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteQuestion);

    if (questionTypeSelector) {
        questionTypeSelector->addItem("Multiple Choice (Single Answer)", "mcq_single");
        questionTypeSelector->addItem("Multiple Choice (Multiple Answers)", "mcq_multiple");
        questionTypeSelector->addItem("Word Fill", "word_fill");
        questionTypeSelector->addItem("Order Phrase", "order_phrase");
        questionTypeSelector->addItem("Match Phrases", "match_phrases");
        questionTypeSelector->addItem("Categorization", "categorization_multiple");
        questionTypeSelector->addItem("List Pick", "list_pick");
        questionTypeSelector->addItem("Image Tagging", "image_tagging");
        questionTypeSelector->addItem("Match Sentence", "match_sentence");
        questionTypeSelector->addItem("Sequence Audio", "sequence_audio");
        questionTypeSelector->addItem("Fill in the Blanks (Dropdown)", "fill_blanks_dropdown");
        questionTypeSelector->addItem("Multi-Questions", "multi_questions");
    }

    if (questionTypeSelector) {
        connect(questionTypeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::onQuestionTypeChanged);
    }
    if (questionListWidget) {
        connect(questionListWidget, &QListWidget::currentItemChanged,
                this, &MainWindow::onQuestionSelected);
    }

    // --- ✨ AI Feature Setup! ✨ ---
    aiManager = new QNetworkAccessManager(this);
    connect(aiManager, &QNetworkAccessManager::finished, this, &MainWindow::onAIRequestFinished);
    loadPrompts();

    // Find the button layout from the UI file to add our new AI button!
    QHBoxLayout *buttonLayout = findChild<QHBoxLayout*>("buttonLayout");
    if (buttonLayout) {
        aiButton = new QPushButton("✨ Generate with AI ✨");
        buttonLayout->insertWidget(1, aiButton); // Put it right after the "Add" button!
        connect(aiButton, &QPushButton::clicked, this, &MainWindow::showAiAssistantDialog);
    }
    // --- End AI Setup ---

    // --- Add our new Live Preview button right next to the AI button! ---
    m_mediaHandler = new MediaHandler(this); // 💖 ADD THIS LINE 💖
    QPushButton *livePreviewButton = new QPushButton("🚀 Live Preview");
    buttonLayout->insertWidget(2, livePreviewButton); // Put it after the AI button
    connect(livePreviewButton, &QPushButton::clicked, this, &MainWindow::onLivePreview);

    showWelcomeMessage();
}

MainWindow::~MainWindow()
{
    // The unique_ptr for currentEditor handles itself! So smart!
}

// --- ✨ New and Updated AI Functions! ✨ ---

void MainWindow::loadPrompts()
{
    QFile promptsFile(QApplication::applicationDirPath() + "/prompts.json");
    if (!promptsFile.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Prompts Not Found", "Oh no, sweetie! I couldn't find prompts.json in the application folder! The AI assistant might not work right. 😢");
        return;
    }
    QByteArray promptsData = promptsFile.readAll();
    QJsonDocument promptsDoc = QJsonDocument::fromJson(promptsData);
    if (promptsDoc.isNull() || !promptsDoc.isObject()) {
        QMessageBox::critical(this, "Prompt Error", "Babe, your prompts.json file seems to be broken! Please check it. 💔");
        return;
    }
    promptTemplates = promptsDoc.object();
}

void MainWindow::showAiAssistantDialog()
{
    aiDialog = new QDialog(this);
    aiDialog->setWindowTitle("✨ AI Question Generation ✨");
    aiDialog->setMinimumSize(700, 750);
    aiDialog->setStyleSheet("QDialog { background-color: #FFB6C1; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(aiDialog);

    // --- Top Controls ---
    QFormLayout *form = new QFormLayout();
    aiApiKeyInput = new QLineEdit();
    aiApiKeyInput->setPlaceholderText("Enter your Google AI API Key here, babe!");
    aiApiKeyInput->setEchoMode(QLineEdit::Password);
    form->addRow("🔑 API Key:", aiApiKeyInput);
    aiQuestionTypeCombo = new QComboBox();
    aiQuestionTypeCombo->addItems(promptTemplates.keys());
    form->addRow("🎀 Question Type:", aiQuestionTypeCombo);
    mainLayout->addLayout(form);

    mainLayout->addWidget(new QLabel("💖 Paste your French text for inspiration below:"));
    aiTopicTextEdit = new QTextEdit();
    aiTopicTextEdit->setPlaceholderText("Lola et Inès sont allées à la Marche des Fiertés...");
    mainLayout->addWidget(aiTopicTextEdit);

    // --- Offline Mode Checkbox ---
    aiOfflineCheckbox = new QCheckBox("📝 Offline Mode (Manual Copy/Paste)");
    mainLayout->addWidget(aiOfflineCheckbox);
    connect(aiOfflineCheckbox, &QCheckBox::toggled, this, &MainWindow::onOfflineModeToggled);

    // --- Online Mode Frame ---
    aiOnlineFrame = new QFrame();
    QVBoxLayout* onlineLayout = new QVBoxLayout(aiOnlineFrame);
    QPushButton *generateButton = new QPushButton("💖 Generate! 💖");
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::onAIGenerateClicked);
    aiStatusLabel = new QLabel("Ready to make some magic! ✨");
    aiStatusLabel->setAlignment(Qt::AlignCenter);
    onlineLayout->addWidget(generateButton);
    onlineLayout->addWidget(aiStatusLabel);
    mainLayout->addWidget(aiOnlineFrame);

    // --- Offline Mode Frame ---
    aiOfflineFrame = new QFrame();
    aiOfflineFrame->setVisible(false); // Hidden by default!
    QVBoxLayout* offlineLayout = new QVBoxLayout(aiOfflineFrame);
    offlineLayout->addWidget(new QLabel("Step 1: Copy this prompt and paste it into your AI tool 💕"));
    aiPromptOutputText = new QTextEdit();
    aiPromptOutputText->setReadOnly(true);
    offlineLayout->addWidget(aiPromptOutputText);
    offlineLayout->addWidget(new QLabel("Step 2: Paste the complete JSON response from your AI here ✨"));
    aiResponseInputText = new QTextEdit();
    offlineLayout->addWidget(aiResponseInputText);
    QPushButton* processPastedButton = new QPushButton("💖 Process Pasted JSON 💖");
    connect(processPastedButton, &QPushButton::clicked, this, &MainWindow::onProcessPastedJson);
    offlineLayout->addWidget(processPastedButton);
    mainLayout->addWidget(aiOfflineFrame);

    aiDialog->exec();
    delete aiDialog;
    aiDialog = nullptr;
}

void MainWindow::onOfflineModeToggled(bool checked)
{
    if (!aiDialog) return;
    aiOnlineFrame->setVisible(!checked);
    aiOfflineFrame->setVisible(checked);

    if (checked) {
        // We're in offline mode, let's generate the prompt for the user!
        QString qType = aiQuestionTypeCombo->currentText();
        QString topic = aiTopicTextEdit->toPlainText();
        QString promptTemplate = promptTemplates[qType].toString();
        QString fullPrompt = promptTemplate.replace("{text}", topic);
        aiPromptOutputText->setPlainText(fullPrompt);
    }
}

void MainWindow::onProcessPastedJson()
{
    if (!aiDialog) return;
    QString pastedText = aiResponseInputText->toPlainText();
    if (pastedText.isEmpty()) {
        QMessageBox::warning(aiDialog, "Oopsie!", "The text box is empty, sweetie! Please paste the AI's JSON response. 💕");
        return;
    }
    pastedText.remove("```json");
    pastedText.remove("```");

    QJsonDocument doc = QJsonDocument::fromJson(pastedText.toUtf8());
    if (doc.isNull() || !doc.isArray()) {
        QMessageBox::critical(aiDialog, "JSON Error 😢", "I couldn't understand the pasted text, babe. Please make sure it's valid JSON!");
        return;
    }
    processAIGeneratedQuestions(doc.array());
}

void MainWindow::onAIGenerateClicked()
{
    if (!aiDialog) return;
    QString apiKey = aiApiKeyInput->text();
    if (apiKey.isEmpty()) {
        aiStatusLabel->setText("Oops! You forgot your API key, sweetie! 🗝️");
        return;
    }
    QString topic = aiTopicTextEdit->toPlainText();
    if (topic.isEmpty()) {
        aiStatusLabel->setText("Tell me the topic, silly! I can't read your mind... yet! 😉");
        return;
    }
    aiStatusLabel->setText("Contacting the magical AI spirits... please wait... ✨");
    QString qType = aiQuestionTypeCombo->currentText();
    QString promptTemplate = promptTemplates[qType].toString();
    QString prompt = promptTemplate.replace("{text}", topic);

    QJsonObject textPart;
    textPart["text"] = prompt;
    QJsonObject payload;
    payload["contents"] = QJsonArray({QJsonObject({{"parts", QJsonArray({textPart})}})});
    payload["generationConfig"] = QJsonObject({{"response_mime_type", "application/json"}});

    // ✨ Using the correct model URL, just like in Python! ✨
    QString url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key=" + apiKey;
    
    // 💖 The fix for the vexing parse! Using curly braces! 💖
    QNetworkRequest request{QUrl(url)};
    
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    aiManager->post(request, QJsonDocument(payload).toJson());
}

void MainWindow::onAIRequestFinished(QNetworkReply *reply)
{
    if (!aiDialog) {
        reply->deleteLater();
        return;
    }
    if (reply->error() != QNetworkReply::NoError) {
        aiStatusLabel->setText("Oh no! A network error! 😱 " + reply->errorString());
        qWarning() << "Network error:" << reply->errorString() << reply->readAll();
        reply->deleteLater();
        return;
    }
    QByteArray responseData = reply->readAll();
    reply->deleteLater();
    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject rootObj = responseDoc.object();
    QJsonArray candidates = rootObj.value("candidates").toArray();
    if (candidates.isEmpty()) {
        aiStatusLabel->setText("The AI didn't have any ideas. Maybe try another topic? 🤔");
        return;
    }
    QString questionJsonText = candidates[0].toObject()["content"].toObject()["parts"].toArray()[0].toObject()["text"].toString();
    questionJsonText.remove("```json");
    questionJsonText.remove("```");

    QJsonDocument questionDoc = QJsonDocument::fromJson(questionJsonText.toUtf8());
    if (questionDoc.isNull() || !questionDoc.isArray()) {
        aiStatusLabel->setText("Couldn't understand the AI's JSON. Let's try again! 💖");
        qWarning() << "Failed to parse inner JSON array:" << questionJsonText;
        return;
    }
    processAIGeneratedQuestions(questionDoc.array());
}

void MainWindow::processAIGeneratedQuestions(const QJsonArray &items)
{
    if (items.isEmpty()) {
        QMessageBox::information(this, "AI Result", "The AI didn't return any questions, sweetie!");
        return;
    }
    saveCurrentQuestion();
    int questionsAdded = 0;
    for (const QJsonValue &val : items) {
        QJsonObject transformed = transformAiQuestion(val.toObject());
        if (!transformed.isEmpty()) {
            allQuestions.append(transformed);
            questionsAdded++;
        }
    }
    refreshQuestionList();
    if (questionsAdded > 0 && questionListWidget) {
        questionListWidget->setCurrentRow(allQuestions.size() - 1);
    }
    QMessageBox::information(this, "Success!", QString("So magical! ✨ Added %1 new questions for you, babe!").arg(questionsAdded));
    if (aiDialog) {
        aiDialog->accept();
    }
}

QJsonObject MainWindow::transformAiQuestion(const QJsonObject &aiItem)
{
    QString q_type = aiItem.value("q_type").toString();
    QJsonObject wifeyItem;

    if (q_type == "MCQ Single Choice") {
        wifeyItem["type"] = "mcq_single";
        wifeyItem["question"] = aiItem["question"].toString();
        QString correctAnswer = aiItem["réponse"].toString();
        QJsonArray distractors = aiItem["distracteurs"].toArray();
        QStringList optionsList;
        optionsList.append(correctAnswer);
        for(const auto& distractor : distractors) optionsList.append(distractor.toString());
        std::shuffle(optionsList.begin(), optionsList.end(), *QRandomGenerator::global());
        int answerIndex = optionsList.indexOf(correctAnswer);
        wifeyItem["options"] = QJsonArray::fromStringList(optionsList);
        wifeyItem["answer"] = QJsonArray({answerIndex});
    }
    else if (q_type == "MCQ Multiple Choice") {
        wifeyItem["type"] = "mcq_multiple";
        wifeyItem["question"] = aiItem["question"].toString();
        QJsonArray correctAnswers = aiItem["réponses"].toArray();
        QJsonArray distractors = aiItem["distracteurs"].toArray();
        QStringList optionsList;
        QList<int> answerIndices;
        for(const auto& ans : correctAnswers) optionsList.append(ans.toString());
        for(const auto& dis : distractors) optionsList.append(dis.toString());
        std::shuffle(optionsList.begin(), optionsList.end(), *QRandomGenerator::global());
        for(const auto& correctAns : correctAnswers) {
            answerIndices.append(optionsList.indexOf(correctAns.toString()));
        }
        std::sort(answerIndices.begin(), answerIndices.end());
        
        // 💖 The fix for the QVariant conversion! So much simpler! 💖
        QJsonArray answerArray;
        for(int index : answerIndices) {
            answerArray.append(index);
        }
        wifeyItem["options"] = QJsonArray::fromStringList(optionsList);
        wifeyItem["answer"] = answerArray;
    }
    else if (q_type == "Fill in the Blanks") {
        wifeyItem["type"] = "word_fill";
        wifeyItem["question"] = "Remplis les blancs, ma chérie!";
        wifeyItem["sentence_parts"] = aiItem["sentence_parts"].toArray();
        wifeyItem["answers"] = aiItem["answers"].toArray();
    }
    else if (q_type == "Order the Phrase") {
        wifeyItem["type"] = "order_phrase";
        wifeyItem["question"] = aiItem["question"].toString();
        wifeyItem["phrase_shuffled"] = aiItem["phrase_shuffled"].toArray();
        wifeyItem["answer"] = aiItem["réponse"].toArray();
    }
    else if (q_type == "Categorization") {
        wifeyItem["type"] = "categorization_multiple";
        wifeyItem["question"] = aiItem["question"].toString();
        QJsonArray stimuli;
        for(const auto& stim : aiItem["stimuli"].toArray()){
            stimuli.append(QJsonObject{{"text", stim.toString()}, {"image", QJsonValue::Null}});
        }
        wifeyItem["stimuli"] = stimuli;
        QJsonArray categories = aiItem["categories"].toArray();
        categories.prepend(" "); // Add the blank category
        wifeyItem["categories"] = categories;
        wifeyItem["answer"] = aiItem["answer"].toObject();
    }
    else if (q_type == "Fill in the Blanks (Dropdown)") {
        wifeyItem["type"] = "fill_blanks_dropdown";
        wifeyItem["question"] = "Choisis la bonne option dans les menus déroulants.";
        wifeyItem["sentence_parts"] = aiItem["sentence_parts"].toArray();
        wifeyItem["options_for_blanks"] = aiItem["options_for_blanks"].toArray();
        wifeyItem["answers"] = aiItem["answers"].toArray();
    }
    else if (q_type == "List Pick") {
        wifeyItem["type"] = "list_pick";
        wifeyItem["question"] = aiItem["question"].toString();
        QJsonArray correctAnswers = aiItem["réponses"].toArray();
        QJsonArray options = aiItem["options"].toArray();
        QList<int> answerIndices;
        for(int i=0; i < options.count(); ++i){
            for(const auto& correctAns : correctAnswers){
                if(options[i] == correctAns){
                    answerIndices.append(i);
                }
            }
        }
        
        // 💖 The fix for the QVariant conversion! So much simpler! 💖
        QJsonArray answerArray;
        for(int index : answerIndices) {
            answerArray.append(index);
        }
        wifeyItem["options"] = options;
        wifeyItem["answer"] = answerArray;
    }
    else {
        qWarning() << "Unsupported AI question type for transformation:" << q_type;
        return QJsonObject();
    }
    return wifeyItem;
}


// AI end.

void MainWindow::onLivePreview()
{
    if (currentQuestionIndex < 0 || !currentEditor) {
        QMessageBox::information(this, "Oopsie!", "Please select a question to preview, darling!");
        return;
    }

    BaseQuestionEditor *editor = qobject_cast<BaseQuestionEditor *>(currentEditor.get());
    if (!editor) return;
    QJsonObject questionJson = editor->getJson();

    // Create our super cute preview dialog!
    QDialog* previewDialog = new QDialog(this);
    previewDialog->setWindowTitle("💖 Live Preview 💖");
    previewDialog->setMinimumSize(600, 700);
    previewDialog->setStyleSheet("QDialog { background-color: #FFEFD5; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(previewDialog);
    QScrollArea *scrollArea = new QScrollArea(previewDialog);
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *previewLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    // --- Lesson PDF Link ---
    if (questionJson.contains("lesson") && questionJson["lesson"].toObject().contains("pdf")) {
        QString relativePdfPath = questionJson["lesson"].toObject()["pdf"].toString();
        if (!relativePdfPath.isEmpty()) {
            // Get the directory of the currently open JSON file
            QDir jsonDir(QFileInfo(currentFilePath).absolutePath());
            // Create the full, correct path to the PDF!
            QString absolutePdfPath = jsonDir.absoluteFilePath(relativePdfPath);

            QLabel* pdfLabel = new QLabel("📚 <b>Lesson PDF:</b> <a href=\"" + QUrl::fromLocalFile(absolutePdfPath).toString() + "\">" + QFileInfo(absolutePdfPath).fileName() + "</a>");
            pdfLabel->setOpenExternalLinks(true); // So it opens in the default PDF viewer!
            previewLayout->addWidget(pdfLabel);
        }
    }

    // --- Title and Hint ---
    QLabel* titleLabel = new QLabel(questionJson["question"].toString().replace("\n", "<br>"));
    titleLabel->setWordWrap(true);
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #8B008B;");
    previewLayout->addWidget(titleLabel);

    if (questionJson.contains("hint") && !questionJson["hint"].toString().isEmpty()) {
        QLabel* hintLabel = new QLabel("<i>Hint: " + questionJson["hint"].toString() + "</i>");
        hintLabel->setWordWrap(true);
        hintLabel->setStyleSheet("color: #C71585;");
        previewLayout->addWidget(hintLabel);
    }
    previewLayout->addSpacing(15);

    // --- Main Container for the Interactive Question ---
    QWidget *questionWidgetContainer = new QWidget(scrollWidget);
    previewLayout->addWidget(questionWidgetContainer);
    previewLayout->addStretch();

    // --- State for Image Tagging Alternatives ---
    int* imageTaggingAltIndex = new int(0);

    // --- The Magical Rendering Function! ---
    auto renderQuestion = [=]() {
        // Clear previous widget
        if (questionWidgetContainer->layout()) {
            QLayoutItem* item;
            while ((item = questionWidgetContainer->layout()->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            delete questionWidgetContainer->layout();
        }

        QVBoxLayout* containerLayout = new QVBoxLayout(questionWidgetContainer);
        containerLayout->setContentsMargins(0, 0, 0, 0);

        QuestionHandlers *previewHandler = new QuestionHandlers(questionWidgetContainer);
        QWidget *questionWidget = new QWidget(questionWidgetContainer);

        previewHandler->createQuestionWidget(
            questionJson,
            questionWidget,
            QFileInfo(currentFilePath).absolutePath(),
            m_mediaHandler,
            *imageTaggingAltIndex // Pass the current alternative index!
        );
        containerLayout->addWidget(questionWidget);
    };

    // --- Alternative Button for Image Tagging ---
    if (questionJson["type"].toString() == "image_tagging") {
        QPushButton* altButton = new QPushButton("🌈 Alternative Version", previewDialog);
        mainLayout->insertWidget(1, altButton); // Add it above the scroll area

        connect(altButton, &QPushButton::clicked, [=]() {
            int maxAlts = 1 + questionJson["alternatives"].toArray().size();
            *imageTaggingAltIndex = (*imageTaggingAltIndex + 1) % maxAlts;
            renderQuestion(); // Re-render with the new index!
        });
    }

    // Initial render of the question
    renderQuestion();

    previewDialog->exec();
    delete imageTaggingAltIndex; // Clean up our state variable
    delete previewDialog;
}

// live preview end.

void MainWindow::setupMainLayout()
{
    Ui::MainWindow ui;
    ui.setupUi(this);
    setCentralWidget(ui.centralwidget);
    QSplitter* mainSplitter = findChild<QSplitter*>("mainSplitter");
    if (mainSplitter) mainSplitter->setSizes({300, 900});
}

void MainWindow::showWelcomeMessage()
{
    clearEditorPanel();
    QLabel *welcomeLabel = new QLabel("💖 Welcome to the Wifey MOOC Editor! 💖\n\nLoad a JSON file to start editing,\n or create a new question with the 'Add' button!");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size: 18pt; color: #8B008B;");
    currentEditor.reset(welcomeLabel);
    if (mainEditorFrameLayout) {
        mainEditorFrameLayout->addWidget(currentEditor.get(), 1);
    }
}

void MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Question File"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (filePath.isEmpty()) return;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for reading.");
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull() || !doc.isArray()) {
        QMessageBox::warning(this, "Error", "Invalid JSON file. File must contain an array of questions.");
        return;
    }
    allQuestions.clear();
    for (const QJsonValue &value : doc.array()) {
        if (value.isObject()) allQuestions.append(value.toObject());
    }
    currentFilePath = filePath;
    setWindowTitle(QString("💖 %1 - Wifey MOOC Editor 💖").arg(QFileInfo(filePath).fileName()));
    refreshQuestionList();
    if (!allQuestions.isEmpty()) {
        questionListWidget->setCurrentRow(0);
    } else {
        showWelcomeMessage();
    }
}

void MainWindow::refreshQuestionList()
{
    if (!questionListWidget) return;
    questionListWidget->blockSignals(true);
    questionListWidget->clear();
    for (int i = 0; i < allQuestions.size(); ++i) {
        QJsonObject q = allQuestions[i];
        QString type = q["type"].toString("unknown");
        QString text = q["question"].toString("No question text.");
        if (text.length() > 30) text = text.left(30) + "...";
        questionListWidget->addItem(QString("%1. [%2] %3").arg(i + 1).arg(type).arg(text));
    }
    questionListWidget->blockSignals(false);
}

void MainWindow::onQuestionSelected(QListWidgetItem *item)
{
    // First, save any changes from the previously selected question.
    // This uses our class member 'currentQuestionIndex', which still holds the old index.
    saveCurrentQuestion();

    if (!item) {
        currentQuestionIndex = -1;
        showWelcomeMessage();
        return;
    }

    int newlySelectedRow = questionListWidget->row(item);
    if (newlySelectedRow < 0 || newlySelectedRow >= allQuestions.size()) {
        currentQuestionIndex = -1;
        showWelcomeMessage();
        return;
    }

    // NOW we update our state to point to the new question.
    currentQuestionIndex = newlySelectedRow;
    loadEditorForQuestion(allQuestions[currentQuestionIndex]);
}

void MainWindow::saveCurrentQuestion()
{
    if (currentQuestionIndex < 0 || currentQuestionIndex >= allQuestions.size() || !currentEditor || !questionListWidget) return;
    BaseQuestionEditor *editor = qobject_cast<BaseQuestionEditor *>(currentEditor.get());
    if (!editor) return;
    allQuestions[currentQuestionIndex] = editor->getJson();
    QString type = allQuestions[currentQuestionIndex]["type"].toString("unknown");
    QString text = allQuestions[currentQuestionIndex]["question"].toString("No question text.");
    if (text.length() > 30) text = text.left(30) + "...";
    questionListWidget->item(currentQuestionIndex)->setText(QString("%1. [%2] %3").arg(currentQuestionIndex + 1).arg(type).arg(text));
}

void MainWindow::loadEditorForQuestion(const QJsonObject &questionJson)
{
    if (!questionTypeSelector) return;
    QString type = questionJson["type"].toString();
    int index = questionTypeSelector->findData(type);
    if (index == -1) {
        QMessageBox::warning(this, "Unsupported Type", QString("Question type '%1' is not supported.").arg(type));
        showWelcomeMessage();
        return;
    }
    questionTypeSelector->blockSignals(true);
    questionTypeSelector->setCurrentIndex(index);
    questionTypeSelector->blockSignals(false);
    clearEditorPanel();
    if (type == "mcq_single") currentEditor.reset(new MCQSingleEditor(this));
    else if (type == "mcq_multiple") currentEditor.reset(new McqMultipleEditor(this));
    else if (type == "word_fill") currentEditor.reset(new WordFillEditor(this));
    else if (type == "order_phrase") currentEditor.reset(new OrderPhraseEditor(this));
    else if (type == "match_phrases") currentEditor.reset(new MatchPhrasesEditor(this));
    else if (type == "categorization_multiple") currentEditor.reset(new CategorizationEditor(this));
    else if (type == "list_pick") currentEditor.reset(new ListPickEditor(this));
    else if (type == "image_tagging") currentEditor.reset(new ImageTaggingEditor(this));
    else if (type == "match_sentence") currentEditor.reset(new MatchSentenceEditor(this));
    else if (type == "sequence_audio") currentEditor.reset(new SequenceAudioEditor(this));
    else if (type == "fill_blanks_dropdown") currentEditor.reset(new FillBlanksDropdownEditor(this));
    else if (type == "multi_questions") currentEditor.reset(new MultiQuestionsEditor(this));
    else {
        QLabel *placeholder = new QLabel(QString("Editor for '%1' coming soon! ✨").arg(type), this);
        placeholder->setAlignment(Qt::AlignCenter);
        currentEditor.reset(placeholder);
    }
    if (mainEditorFrameLayout) {
        mainEditorFrameLayout->addWidget(currentEditor.get(), 1);
    }
    if (auto editor = dynamic_cast<BaseQuestionEditor *>(currentEditor.get())) {
        editor->loadJson(questionJson);
    }
}

void MainWindow::clearEditorPanel()
{
    if (currentEditor && mainEditorFrameLayout) {
        mainEditorFrameLayout->removeWidget(currentEditor.get());
        currentEditor.reset();
    }
}

void MainWindow::onQuestionTypeChanged(int index)
{
    if (currentQuestionIndex < 0 || currentQuestionIndex >= allQuestions.size()) return;
    QString type = questionTypeSelector->itemData(index).toString();
    allQuestions[currentQuestionIndex]["type"] = type;
    loadEditorForQuestion(allQuestions[currentQuestionIndex]);
}

void MainWindow::onAddQuestion()
{
    saveCurrentQuestion();
    QJsonObject newQuestion;
    newQuestion["type"] = "mcq_single";
    newQuestion["question"] = "💖 New Question 💖";
    newQuestion["options"] = QJsonArray{"Option A", "Option B"};
    newQuestion["answer"] = QJsonArray{0};
    allQuestions.append(newQuestion);
    refreshQuestionList();
    if (questionListWidget) {
        questionListWidget->setCurrentRow(allQuestions.size() - 1);
    }
}

// TODO: Proper fix!
void MainWindow::onDeleteQuestion()
{
    // 1. Get the row the user wants to delete DIRECTLY from the visual list.
    const int rowToDelete = questionListWidget->currentRow();

    qDebug() << "--- Delete Button Clicked ---";
    qDebug() << "Visual row selected for deletion:" << rowToDelete;
    qDebug() << "Index of the question currently in the editor:" << currentQuestionIndex;

    // 2. Make sure a selection exists.
    if (rowToDelete < 0) {
        QMessageBox::information(this, "Oops!", "Please select a question to delete, sweetie.");
        return;
    }

    if (QMessageBox::question(this, "Are you sure?", "💖 Are you sure you want to delete this adorable question? It can't be undone! 💖", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }

    // 3. Save any pending changes from the currently OPEN editor.
    saveCurrentQuestion();

    // 4. Remove the question from our master data list using the DIRECT row index.
    allQuestions.removeAt(rowToDelete);

    // 5. If the deleted question was the one being edited, or one before it,
    // we must adjust the editor's index to prevent it from pointing to the wrong question later.
    if (rowToDelete < currentQuestionIndex) {
        currentQuestionIndex--;
    } else if (rowToDelete == currentQuestionIndex) {
        currentQuestionIndex = -1; // The edited question no longer exists.
    }

    // 6. Refresh the visual list from the now-correct data.
    refreshQuestionList();

    // 7. Select the next logical item in the list.
    if (!allQuestions.isEmpty()) {
        int newIndexToSelect = qMin(rowToDelete, allQuestions.size() - 1);
        questionListWidget->setCurrentRow(newIndexToSelect);
    } else {
        // If the list is now empty, just show the welcome message.
        showWelcomeMessage();
    }
}

bool MainWindow::saveFile()
{
    if (currentFilePath.isEmpty()) return saveFileAs();
    return saveToFile(currentFilePath);
}

bool MainWindow::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Question File"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (filePath.isEmpty()) return false;
    return saveToFile(filePath);
}

bool MainWindow::saveToFile(const QString &filePath)
{
    saveCurrentQuestion();
    QJsonArray questionArray;
    QFileInfo fileInfo(filePath);
    QString saveDirectory = fileInfo.dir().path();
    for (const QJsonObject &q : allQuestions) {
        questionArray.append(q);
    }
    QJsonDocument doc(questionArray);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for writing.");
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    currentFilePath = filePath;
    setWindowTitle(QString("💖 %1 - Wifey MOOC Editor 💖").arg(QFileInfo(filePath).fileName()));
    QMessageBox::information(this, "Success!", "File saved successfully! 💕");
    return true;
}

void MainWindow::newFile()
{
    if (!allQuestions.isEmpty()) {
        if (QMessageBox::question(this, "New File", "Are you sure you want to create a new file? All unsaved changes will be lost!", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            return;
    }
    allQuestions.clear();
    currentFilePath.clear();
    currentQuestionIndex = -1;
    refreshQuestionList();
    showWelcomeMessage();
    setWindowTitle("💖 New Question File - Wifey MOOC Editor 💖");
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setShortcuts(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
}

void MainWindow::applyStylesheet()
{
    QString style =R"(
        QMainWindow { background-color: #FFB6C1; }
        QWidget { font-family: 'Arial'; font-size: 12pt; color: #8B008B; }
        QMenuBar { background-color: #FFC0CB; }
        QMenu { background-color: #FFC0CB; }
        QPushButton { background-color: #FF1493; color: #FFFFFF; border-radius: 5px; padding: 8px 12px; font-weight: bold; }
        QPushButton:hover { background-color: #FF69B4; }
        QLabel { color: #8B008B; }
        QLineEdit, QTextEdit, QPlainTextEdit { background-color: #FFEFD5; border: 1px solid #FFC0CB; border-radius: 5px; padding: 5px; }
        QComboBox { background-color: #FFFFFF; border: 1px solid #FFC0CB; border-radius: 5px; padding: 5px; }
        QGroupBox { border: 1px solid #FF69B4; border-radius: 5px; margin-top: 10px; font-weight: bold; }
        QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; background-color: #FFB6C1; }
        QListWidget { background-color: #FFEFD5; border: 1px solid #FFC0CB; }
        QSplitter::handle { background-color: #FF69B4; }
    )";
    this->setStyleSheet(style);
}
