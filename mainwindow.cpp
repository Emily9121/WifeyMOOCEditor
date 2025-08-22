#include "mainwindow.h"
#include "questiontemplate.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_splitter(nullptr)
    , m_leftFrame(nullptr)
    , m_rightFrame(nullptr)
    , m_questionsListWidget(nullptr)
    , m_editorScrollArea(nullptr)
    , m_editorContainer(nullptr)
    , m_currentQuestionIndex(-1)
    , m_currentEditor(nullptr)
    , m_settings(new QSettings(this))
{
    setWindowTitle("💖 Wifey MOOC JSON Editor 💖");
    setMinimumSize(1200, 800);

    // Load configuration
    m_wifeyMoocPath = m_settings->value("wifeymooc_path", "").toString();

    setupColors();
    setupUI();
    setupMenuBar();
    showWelcomeMessage();

    // Set initial window size and position
    resize(1400, 900);

    statusBar()->showMessage("Ready to create amazing questions! 💖", 2000);
}

MainWindow::~MainWindow()
{
    // Save window geometry
    m_settings->setValue("geometry", saveGeometry());
    m_settings->setValue("windowState", saveState());
}

void MainWindow::setupColors()
{
    // Apply pink color scheme to the application
    setStyleSheet(QString(
        "QMainWindow { background-color: %1; }"
        "QFrame { background-color: %2; border: 2px solid %3; border-radius: 5px; }"
        "QListWidget { background-color: %4; border: 1px solid %3; border-radius: 3px; }"
        "QPushButton { "
        "   background-color: %5; "
        "   color: black; "
        "   border: 2px solid %3; "
        "   border-radius: 5px; "
        "   padding: 8px 16px; "
        "   font-weight: bold; "
        "   min-width: 80px; "
        "}"
        "QPushButton:hover { background-color: %3; }"
        "QPushButton:pressed { background-color: %6; }"
        "QLineEdit, QTextEdit, QComboBox { "
        "   background-color: %7; "
        "   border: 1px solid %3; "
        "   border-radius: 3px; "
        "   padding: 5px; "
        "}"
        "QGroupBox { "
        "   font-weight: bold; "
        "   border: 2px solid %3; "
        "   border-radius: 5px; "
        "   margin-top: 1ex; "
        "   color: %6; "
        "}"
        "QGroupBox::title { "
        "   subcontrol-origin: margin; "
        "   left: 10px; "
        "   padding: 0 5px 0 5px; "
        "}"
        "QLabel { color: %6; }"
    ).arg(m_colors.bg, m_colors.secondary, m_colors.accent, m_colors.white,
          m_colors.button, m_colors.text, m_colors.entry));
}

void MainWindow::setupUI()
{
    // Create central widget
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(m_centralWidget);

    // Create title label
    QLabel *titleLabel = new QLabel("💖 Wifey MOOC Question Editor 💖", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    // Make title nice and petite, not a space hog!
    titleLabel->setFixedHeight(36); // Try 30-40 for cuteness
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; margin: 0px 5px 0px 5px; color: " + m_colors.text + ";");
    mainLayout->addWidget(titleLabel);

    // Create button toolbar
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    m_newButton = new QPushButton("✨ New JSON", this);
    m_loadButton = new QPushButton("💾 Load JSON", this);
    m_saveButton = new QPushButton("💖 Save JSON", this);
    m_saveAsButton = new QPushButton("💖 Save As", this);
    m_launchButton = new QPushButton("🚀 Save & Launch", this);
    m_addQuestionButton = new QPushButton("✨ New Question", this);
    m_configButton = new QPushButton("⚙️ Settings", this);

    buttonLayout->addWidget(m_newButton);
    buttonLayout->addWidget(m_loadButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_saveAsButton);
    buttonLayout->addWidget(m_launchButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_addQuestionButton);
    buttonLayout->addWidget(m_configButton);

    mainLayout->addLayout(buttonLayout);

    // Create main splitter
    m_splitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(m_splitter, 1);

    // Create left frame (questions list)
    m_leftFrame = new QFrame(this);
    m_leftFrame->setFixedWidth(350);
    QVBoxLayout *leftLayout = new QVBoxLayout(m_leftFrame);

    QLabel *listLabel = new QLabel("💕 Questions List 💕", this);
    listLabel->setAlignment(Qt::AlignCenter);
    listLabel->setStyleSheet("font-weight: bold; font-size: 12px; margin: 5px;");
    leftLayout->addWidget(listLabel);

    m_questionsListWidget = new QListWidget(this);
    leftLayout->addWidget(m_questionsListWidget);

    // Create list management buttons
    QHBoxLayout *listButtonLayout = new QHBoxLayout();
    m_deleteButton = new QPushButton("🗑️ Delete", this);
    m_moveUpButton = new QPushButton("⬆️ Up", this);
    m_moveDownButton = new QPushButton("⬇️ Down", this);

    m_deleteButton->setStyleSheet("QPushButton { background-color: #FF6347; }");

    listButtonLayout->addWidget(m_deleteButton);
    listButtonLayout->addWidget(m_moveUpButton);
    listButtonLayout->addWidget(m_moveDownButton);
    leftLayout->addLayout(listButtonLayout);

    m_splitter->addWidget(m_leftFrame);

    // Create right frame (question editor)
    m_rightFrame = new QFrame(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(m_rightFrame);

    m_editorScrollArea = new QScrollArea(this);
    m_editorScrollArea->setWidgetResizable(true);
    m_editorScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_editorScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    rightLayout->addWidget(m_editorScrollArea);
    m_splitter->addWidget(m_rightFrame);

    // Set splitter proportions
    m_splitter->setStretchFactor(0, 0);
    m_splitter->setStretchFactor(1, 1);

    // Connect signals
    connect(m_newButton, &QPushButton::clicked, this, &MainWindow::newJson);
    connect(m_loadButton, &QPushButton::clicked, this, &MainWindow::loadJson);
    connect(m_saveButton, &QPushButton::clicked, this, &MainWindow::saveJson);
    connect(m_saveAsButton, &QPushButton::clicked, this, &MainWindow::saveJsonAs);
    connect(m_launchButton, &QPushButton::clicked, this, &MainWindow::saveAndLaunch);
    connect(m_addQuestionButton, &QPushButton::clicked, this, &MainWindow::showAddQuestionDialog);
    connect(m_configButton, &QPushButton::clicked, this, &MainWindow::showConfiguration);

    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::deleteQuestion);
    connect(m_moveUpButton, &QPushButton::clicked, this, &MainWindow::moveQuestionUp);
    connect(m_moveDownButton, &QPushButton::clicked, this, &MainWindow::moveQuestionDown);

    connect(m_questionsListWidget, &QListWidget::currentRowChanged, this, &MainWindow::onQuestionSelected);

    // Restore window geometry
    restoreGeometry(m_settings->value("geometry").toByteArray());
    restoreState(m_settings->value("windowState").toByteArray());
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();

    // File menu
    QMenu *fileMenu = menuBar->addMenu("📁 File");

    QAction *newAction = fileMenu->addAction("✨ New");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::newJson);

    QAction *openAction = fileMenu->addAction("💾 Open");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::loadJson);

    QAction *saveAction = fileMenu->addAction("💖 Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveJson);

    QAction *saveAsAction = fileMenu->addAction("💖 Save As");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveJsonAs);

    fileMenu->addSeparator();

    QAction *launchAction = fileMenu->addAction("🚀 Save & Launch");
    launchAction->setShortcut(QKeySequence("Ctrl+L"));
    connect(launchAction, &QAction::triggered, this, &MainWindow::saveAndLaunch);

    fileMenu->addSeparator();

    QAction *exitAction = fileMenu->addAction("❌ Exit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Edit menu
    QMenu *editMenu = menuBar->addMenu("✏️ Edit");

    QAction *addQuestionAction = editMenu->addAction("✨ Add Question");
    addQuestionAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(addQuestionAction, &QAction::triggered, this, &MainWindow::showAddQuestionDialog);

    QAction *deleteQuestionAction = editMenu->addAction("🗑️ Delete Question");
    deleteQuestionAction->setShortcut(QKeySequence::Delete);
    connect(deleteQuestionAction, &QAction::triggered, this, &MainWindow::deleteQuestion);

    // Tools menu
    QMenu *toolsMenu = menuBar->addMenu("🔧 Tools");

    QAction *configAction = toolsMenu->addAction("⚙️ Settings");
    connect(configAction, &QAction::triggered, this, &MainWindow::showConfiguration);

    // Help menu
    QMenu *helpMenu = menuBar->addMenu("❓ Help");

    QAction *aboutAction = helpMenu->addAction("💖 About");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "About Wifey MOOC Editor",
            "💖 Wifey MOOC JSON Editor 💖\n\n"
            "A complete WYSIWYG editor for WifeyMOOC quiz questions\n\n"
            "Created with love and lots of pink! 💕\n\n"
            "Version 1.0.0\n"
            "Built with Qt6 and C++");
    });
}

void MainWindow::showWelcomeMessage()
{
    // Safely remove and delete editor container if currently displayed
    QWidget *currentWidget = m_editorScrollArea->widget();
    if (currentWidget)
    {
        if (currentWidget == m_editorContainer)
        {
            m_editorScrollArea->takeWidget();
            m_editorContainer->deleteLater();
            m_editorContainer = nullptr;
        }
        else
        {
            // If some other widget is there, do nothing or delete accordingly
        }
    }

    // Create new welcome container fresh and set it in the scroll area
    m_editorContainer = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(m_editorContainer);
    QLabel *welcomeLabel = new QLabel(
        "💖 Welcome to Wifey MOOC Editor! 💖\n\n"
        "✨ Load a JSON file or create new questions to get started ✨\n\n"
        "💕 Made with love for all the girlies! 💕",
        m_editorContainer);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet(
        "font-size: 16px; font-weight: bold; color: " + m_colors.text + "; "
        "margin: 50px; padding: 20px;");
    layout->addWidget(welcomeLabel);
    m_editorScrollArea->setWidget(m_editorContainer);
}

void MainWindow::refreshQuestionsList()
{
    m_questionsListWidget->clear();

    for (int i = 0; i < m_questions.size(); ++i) {
        QJsonObject question = m_questions[i].toObject();
        QString type = question["type"].toString();
        QString questionText = question["question"].toString();

        // Truncate long questions
        if (questionText.length() > 40) {
            questionText = questionText.left(40) + "... 💖";
        }

        QString displayText = QString("%1. [%2] %3")
            .arg(i + 1)
            .arg(type)
            .arg(questionText);

        m_questionsListWidget->addItem(displayText);
    }
}

// Continue in next part...

// MainWindow implementation continued...

void MainWindow::newJson()
{
    if (!promptUnsavedChanges()) {
        return;
    }

    m_questions = QJsonArray();
    m_currentFile.clear();
    m_currentQuestionIndex = -1;

    refreshQuestionsList();
    showWelcomeMessage();

    QMessageBox::information(this, "New File", "New JSON file created! Start adding questions, babe! 💖");
}

void MainWindow::loadJson()
{
    if (!promptUnsavedChanges())
        return;

    QString fileName = QFileDialog::getOpenFileName(
        this,
        "💖 Select Your Cute Wifey MOOC JSON File 💖",
        QString(),
        "JSON files (*.json);;All files (*.*)"
    );
    if (fileName.isEmpty())
        return;

    QString errorMessage;
    QJsonArray questions;
    if (JsonHandler::loadQuestionsFromFile(fileName, questions, errorMessage))
    {
        m_questions = questions;
        m_currentFile = fileName;
        refreshQuestionsList();

        // Safely delete current editor widget before showWelcomeMessage
        if (m_currentEditor)
        {
            disconnect(m_currentEditor, &QuestionEditor::questionChanged, this, nullptr);
            QWidget *currentWidget = m_editorScrollArea->widget();
            if (currentWidget == m_currentEditor)
            {
                m_editorScrollArea->takeWidget();
            }
            m_currentEditor->deleteLater();
            m_currentEditor = nullptr;
        }
        m_currentQuestionIndex = -1;

        showWelcomeMessage();

        QMessageBox::information(this, "Success! 💖",
            QString("Loaded %1 adorable questions successfully!").arg(questions.size()));

        statusBar()->showMessage(QString("Loaded: %1").arg(fileName), 3000);
    }
    else
    {
        QMessageBox::critical(this, "Oopsie! 😢",
            QString("Failed to load file, babe:\n%1").arg(errorMessage));
    }
}

void MainWindow::saveJson()
{
    if (m_questions.isEmpty()) {
        QMessageBox::warning(this, "Warning! 💕", "No questions to save, sweetie!");
        return;
    }

    if (m_currentFile.isEmpty()) {
        saveJsonAs();
        return;
    }

    QString errorMessage;
    if (JsonHandler::saveQuestionsToFile(m_currentFile, m_questions, errorMessage)) {
        QMessageBox::information(this, "Success! 💖", "Questions saved successfully, darling!");
        statusBar()->showMessage("File saved! 💖", 2000);
    } else {
        QMessageBox::critical(this, "Oopsie! 😢", 
            QString("Failed to save file, honey:\n%1").arg(errorMessage));
    }
}

void MainWindow::saveJsonAs()
{
    if (m_questions.isEmpty()) {
        QMessageBox::warning(this, "Warning! 💕", "No questions to save, sweetie!");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "💖 Save Your Beautiful Wifey MOOC JSON File 💖",
        QString(),
        "JSON files (*.json);;All files (*.*)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    m_currentFile = fileName;
    saveJson();
}

void MainWindow::saveAndLaunch()
{
    if (m_questions.isEmpty()) {
        QMessageBox::warning(this, "Warning! 💕", "No questions to save, sweetie!");
        return;
    }

    if (m_currentFile.isEmpty()) {
        saveJsonAs();
        if (m_currentFile.isEmpty()) {
            return;
        }
    } else {
        saveJson();
    }

    if (m_wifeyMoocPath.isEmpty()) {
        QMessageBox::warning(this, "Configuration Needed! 💕", 
            "Please configure the WifeyMOOC application path in Settings first, sweetie!");
        showConfiguration();
        return;
    }

    QStringList arguments;
    arguments << "-q" << m_currentFile;

    if (QProcess::startDetached(m_wifeyMoocPath, arguments)) {
        QMessageBox::information(this, "Launched! 💖", 
            QString("Launched WifeyMOOC with\n%1").arg(m_currentFile));
        statusBar()->showMessage("WifeyMOOC launched! 🚀", 3000);
    } else {
        QMessageBox::critical(this, "Oopsie! 😢", 
            "Failed to launch app, babe! Please check the application path in Settings.");
    }
}

void MainWindow::showAddQuestionDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("✨ Add New Adorable Question ✨");
    dialog.setModal(true);
    dialog.resize(450, 600);

    // Apply pink theme to dialog
    dialog.setStyleSheet(QString("QDialog { background-color: %1; }").arg(m_colors.bg));

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *titleLabel = new QLabel("💖 Choose Your Question Type, Babe! 💖", &dialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold; margin: 10px; color: " + m_colors.text + ";");
    layout->addWidget(titleLabel);

    // Question type buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout();

    struct QuestionTypeInfo {
        QuestionType type;
        QString displayName;
    };

    QList<QuestionTypeInfo> questionTypes = {
        {QuestionType::ListPick, questionTypeDisplayName(QuestionType::ListPick)},
        {QuestionType::McqSingle, questionTypeDisplayName(QuestionType::McqSingle)},
        {QuestionType::McqMultiple, questionTypeDisplayName(QuestionType::McqMultiple)},
        {QuestionType::WordFill, questionTypeDisplayName(QuestionType::WordFill)},
        {QuestionType::MatchSentence, questionTypeDisplayName(QuestionType::MatchSentence)},
        {QuestionType::SequenceAudio, questionTypeDisplayName(QuestionType::SequenceAudio)},
        {QuestionType::OrderPhrase, questionTypeDisplayName(QuestionType::OrderPhrase)},
        {QuestionType::CategorizationMultiple, questionTypeDisplayName(QuestionType::CategorizationMultiple)},
        {QuestionType::FillBlanksDropdown, questionTypeDisplayName(QuestionType::FillBlanksDropdown)},
        {QuestionType::MatchPhrases, questionTypeDisplayName(QuestionType::MatchPhrases)}
    };

    for (const auto &info : questionTypes) {
        QPushButton *button = new QPushButton(info.displayName, &dialog);
        button->setStyleSheet(QString("QPushButton { background-color: %1; }").arg(m_colors.accent));
        connect(button, &QPushButton::clicked, [this, info, &dialog]() {
            addQuestion(info.type);
            dialog.accept();
        });
        buttonLayout->addWidget(button);
    }

    layout->addLayout(buttonLayout);

    QPushButton *cancelButton = new QPushButton("❌ Cancel", &dialog);
    cancelButton->setStyleSheet("QPushButton { background-color: #FF6347; }");
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    layout->addWidget(cancelButton);

    dialog.exec();
}

void MainWindow::addQuestion(QuestionType type)
{
    QJsonObject newQuestion = QuestionTemplate::createTemplate(type);
    m_questions.append(newQuestion);

    refreshQuestionsList();

    // Select the new question automatically
    int newIndex = m_questions.size() - 1;
    m_questionsListWidget->setCurrentRow(newIndex);
    m_currentQuestionIndex = newIndex;
    editQuestion(newIndex);

    QString typeName = questionTypeDisplayName(type);
    QMessageBox::information(this, "Success! 💖", 
        QString("Added new %1 question, babe!").arg(typeName));
}

void MainWindow::onQuestionSelected()
{
    int currentRow = m_questionsListWidget->currentRow();
    if (currentRow >= 0 && currentRow < m_questions.size()) {
        m_currentQuestionIndex = currentRow;
        editQuestion(currentRow);
    }
}

void MainWindow::editQuestion(int index)
{
    if (index < 0 || index >= m_questions.size()) {
        return;
    }

    QJsonObject question = m_questions[index].toObject();
    QString typeStr = question["type"].toString();
    QuestionType type = stringToQuestionType(typeStr);

    // Safely delete old editor and clear scroll area
    if (m_currentEditor) {
        disconnect(m_currentEditor, &QuestionEditor::questionChanged, this, nullptr);
        m_editorScrollArea->takeWidget()->deleteLater();
        m_currentEditor = nullptr;
    }

    // Create the new editor for the question type
    switch (type) {
    case QuestionType::ListPick:
        m_currentEditor = new ListPickEditor();
        break;
    case QuestionType::McqSingle:
        m_currentEditor = new McqSingleEditor();
        break;
    case QuestionType::McqMultiple:
        m_currentEditor = new McqMultipleEditor();
        break;
    case QuestionType::WordFill:
        m_currentEditor = new WordFillEditor();
        break;
    case QuestionType::SequenceAudio:
        m_currentEditor = new SequenceAudioEditor();
        break;
    case QuestionType::MatchSentence:
        m_currentEditor = new MatchSentenceEditor();
        break;
    case QuestionType::FillBlanksDropdown:
        m_currentEditor = new FillBlanksDropdownEditor();
        break;
    case QuestionType::OrderPhrase:
        m_currentEditor = new OrderPhraseEditor();
        break;
    case QuestionType::CategorizationMultiple:
        m_currentEditor = new CategorizationMultipleEditor();
        break;
    case QuestionType::MatchPhrases:
        m_currentEditor = new MatchPhrasesEditor();
        break;
    case QuestionType::Unknown:
    default:
        m_currentEditor = new UnknownQuestionEditor(typeStr);
        break;
    }

    if (!m_currentEditor) {
        return;
    }

    m_currentEditor->setQuestion(question);

    // Connect signal to update data on changes
    connect(m_currentEditor, &QuestionEditor::questionChanged, this, [this]() {
        if (m_currentEditor && m_currentQuestionIndex >= 0) {
            m_questions[m_currentQuestionIndex] = m_currentEditor->getQuestion();
            refreshQuestionsList();
            m_questionsListWidget->setCurrentRow(m_currentQuestionIndex);
        }
    });

    // Set new editor into the scroll area safely
    m_editorScrollArea->setWidget(m_currentEditor);

    m_currentQuestionIndex = index;
}


void MainWindow::deleteQuestion()
{
    int currentRow = m_questionsListWidget->currentRow();
    if (currentRow < 0 || currentRow >= m_questions.size()) {
        QMessageBox::warning(this, "Warning! 💕", "Please select a question to delete, sweetie!");
        return;
    }

    QJsonObject question = m_questions[currentRow].toObject();
    QString questionText = question["question"].toString();
    if (questionText.length() > 50) {
        questionText = questionText.left(50) + "...";
    }

    int ret = QMessageBox::question(this, "Confirm Delete 🗑️", 
        QString("Are you sure you want to delete this question, honey?\n\n%1").arg(questionText),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        m_questions.removeAt(currentRow);
        refreshQuestionsList();
        showWelcomeMessage();
        QMessageBox::information(this, "Deleted! 💔", "Question deleted successfully, darling!");
    }
}

void MainWindow::moveQuestionUp()
{
    int currentRow = m_questionsListWidget->currentRow();
    if (currentRow <= 0 || currentRow >= m_questions.size()) {
        return;
    }

    QJsonValue temp = m_questions[currentRow];
    m_questions[currentRow] = m_questions[currentRow - 1];
    m_questions[currentRow - 1] = temp;

    refreshQuestionsList();
    m_questionsListWidget->setCurrentRow(currentRow - 1);
    m_currentQuestionIndex = currentRow - 1;
}

void MainWindow::moveQuestionDown()
{
    int currentRow = m_questionsListWidget->currentRow();
    if (currentRow < 0 || currentRow >= m_questions.size() - 1) {
        return;
    }

    QJsonValue temp = m_questions[currentRow];
    m_questions[currentRow] = m_questions[currentRow + 1];
    m_questions[currentRow + 1] = temp;

    refreshQuestionsList();
    m_questionsListWidget->setCurrentRow(currentRow + 1);
    m_currentQuestionIndex = currentRow + 1;
}

void MainWindow::showConfiguration()
{
    ConfigDialog dialog(this);
    dialog.setWifeyMoocPath(m_wifeyMoocPath);

    if (dialog.exec() == QDialog::Accepted) {
        m_wifeyMoocPath = dialog.getWifeyMoocPath();
        m_settings->setValue("wifeymooc_path", m_wifeyMoocPath);
    }
}

bool MainWindow::promptUnsavedChanges()
{
    if (m_questions.isEmpty()) {
        return true;
    }

    int ret = QMessageBox::question(this, "Unsaved Changes", 
        "You have unsaved changes. Do you want to continue?\n\nAll unsaved changes will be lost.",
        QMessageBox::Yes | QMessageBox::No);

    return ret == QMessageBox::Yes;
}
