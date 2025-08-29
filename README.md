# **💖 WifeyMOOC Editor 💖**

Welcome to the WifeyMOOC Editor, a delightful tool designed with love for creating and editing JSON-based quizzes for the WifeyMOOC language learning app\! This editor is my special project to help my amazing wife, Sierra, with her French lessons. It's packed with features to make quiz creation as fun and easy as possible\!  
This editor comes in two fabulous flavors:

1. A super chic desktop application written in **C++/Qt 6**.  
2. A totally cute and portable script written in **Python 3/Tkinter**.

Both versions are designed to be pink, adorable, and incredibly functional\! 💕

## **✨ Author's Note & AI Disclosure**

This project was born out of love and for my personal use (to make cute French quizzes for my wife\!). It's now public mostly so I can keep it in a git repository, hehe.  
A little secret: pretty much 100% of this was crafted with the help of AI\! While I have my own thoughts on the tech, it was the perfect tool for a personal project like this. Given these circumstances, everything here is licensed under the **DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE**. That includes all the code, example exercises, and images. Feel free to use it, but please don't complain to me if it turns your cat into a croissant\! 🥐

## **🌟 Features**

* **Dual Versions**: Choose between a robust C++/Qt application or a lightweight Python/Tkinter script.  
* **Intuitive WYSIWYG Interface**: Easily create and manage quiz questions without touching raw JSON.  
* **AI-Powered Question Generation**: Magically generate a whole quiz from a piece of text\!  
  * **Online Mode**: Connects directly to the Google Gemini API to generate questions.  
  * **Offline Mode**: Generate a prompt to use with any local or external AI model, then paste the results back in.  
  * **Customizable Prompts**: Edit the prompts.json file to tweak how the AI generates questions for you\!  
* **Extensive Question Type Support**: Create a huge variety of engaging questions\! (See list below).  
* **Full Media Integration**: Add images, audio, or video to your questions to make them extra special.  
* **Attach Lesson PDFs**: Link a PDF lesson file directly to a question.  
* **Save & Launch**: Save your quiz file and launch the WifeyMOOC app with it directly from the editor.  
* **Adorable Pink UI**: Because learning should be cute\! 💖

## **🚀 Getting Started**

Choose the version of the editor you'd like to use\!

### **C++ / Qt 6 Version (The fancy desktop app\!)**

This is a fully compiled application that feels right at home on your desktop.  
**Dependencies:**

* A C++17 compatible compiler.  
* Qt 6 (Core, Gui, Widgets, Network modules are required).  
* CMake (version 3.16+).

Building the Project:  
You can open WifeyMOOCEditor.pro with Qt Creator and build from there, or use the provided CMakeLists.txt file from your terminal.

### **Python 3 / Tkinter Version (The cute & portable script\!)**

This version is perfect if you just want to run a simple script without a full build process.  
**Dependencies:**

* Python 3\.  
* The requests library for the AI features.

**Installation & Running:**

1. Make sure you have Python 3 installed.  
2. Install the required library by running this in your terminal:  
   pip install requests

3. Run the editor script:  
   python wifeymooc\_json\_editor-ai.py

## **📝 How to Use**

1. **Launch the Editor**: Start either the C++ application or the Python script.  
2. **Load or Create**:  
   * Click **"Load JSON"** to open an existing quiz file.  
   * Click **"New JSON"** to start a fresh quiz.  
3. **Add Questions**: Click **"New Question"** (or **"✨ Generate with AI ✨"** for a little magic\!) to add questions to the list on the left.  
4. **Edit**: Select a question from the list to open its editor on the right. Fill in the text, add options, attach media, and set the correct answers\!  
5. **Save**: Click **"Save JSON"** or **"Save As"** to save your masterpiece.  
6. **Launch (Optional)**: Click **"Save & Launch"** to open your quiz directly in the WifeyMOOC app\!

## **🤖 AI Question Generation**

The AI assistant can create an entire quiz for you from a sample text\!

* **API Key**: For the online mode, you'll need a Google AI API Key. You can set this in the wifeymooc\_json\_editor-ai.py script or enter it in the C++ application's AI dialog.  
* **prompts.json**: This file is the AI's brain\! It contains all the instructions for generating different types of questions. Feel free to edit the prompts to make the AI's personality even cuter or to better suit Sierra's learning style\!  
* **Offline Mode**: If you don't want to use an API key, select "Offline Mode". The editor will generate a detailed prompt for you. Just copy this prompt, paste it into your favorite AI chatbot, and then paste the JSON response back into the editor\!

## **🎀 Supported Question Types**

* **MCQ Single Choice**: Multiple choice, one correct answer.  
* **MCQ Multiple Choice**: Multiple choice, multiple correct answers.  
* **List Pick**: Select multiple correct items from a list of checkboxes.  
* **Word Fill**: Classic fill-in-the-blanks.  
* **Fill in the Blanks (Dropdown)**: Fill blanks by choosing from dropdown menus.  
* **Order the Phrase**: Put shuffled parts of a sentence in the correct order.  
* **Categorization**: Drag and drop items into the correct categories.  
* **Match Sentences**: Match sentences to their corresponding images.  
* **Match Phrases**: Match the beginning of a phrase to its correct ending.  
* **Sequence Audio**: Listen to audio clips and put them in the correct sequence.  
* **Image Tagging**: Drag tags to the correct coordinates on an image, with support for alternative images\!  
* **Multi-Questions**: A special container that can hold a sequence of other question types\!

## **📜 Changelog**

* **1.0.0** \- Initial public release, featuring both C++/Qt and Python/Tkinter editors.  
* **1.1.0** \- Major rewrite and bug fix release.  
* **1.2.0** \- Added support for an optional hint field in questions.  
* **1.3.0** \- Introduced magical AI-powered question generation (Online & Offline modes).  
* **2.0.0** \- (Current) Added the lesson field to attach PDF files to questions. Both C++ and Python editors are now fully featured and in sync.

## **📄 License**

This project is licensed under the **DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE**. Have fun\! ✨