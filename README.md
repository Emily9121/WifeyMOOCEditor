# 💖 WifeyMOOC JSON Editor - C++/Qt6 Port 💖

A complete C++/Qt6 port of the Python WifeyMOOC JSON Editor with full feature parity and enhanced robustness.

## ✨ Features

- **Complete WYSIWYG editor** for WifeyMOOC quiz questions
- **10 supported question types** with specialized editors:
  - 📝 List Pick (Select Multiple)
  - 🔘 Single Choice (MCQ)
  - ☑️ Multiple Choice (MCQ)
  - 📝 Word Fill (Fill Blanks)
  - 🔗 Match Sentences to Images
  - 🎵 Audio Sequence Order
  - 📋 Order Phrases Correctly
  - 📂 Categorization Multiple
  - ⬇️ Fill Blanks with Dropdowns
  - 🔗 Match Phrase Beginnings to Endings

- **Full JSON file operations** (New, Load, Save, Save As)
- **Question management** (Add, Delete, Move Up/Down)
- **Media file support** (Video, Audio, Images)
- **Configuration system** for WifeyMOOC app path
- **Robust error handling** for unknown question types
- **Save & Launch** functionality
- **Beautiful pink UI theme** 💕

## 🔧 Requirements

- **Qt6** (Core, Widgets modules)
- **C++17** or later
- **CMake 3.16** or later
- **Compatible compiler** (GCC, Clang, MSVC)

## 🚀 Building

### Linux/macOS
```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Windows (Visual Studio)
```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

### Windows (MinGW)
```cmd
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

## 📁 Project Structure

```
WifeyMOOCEditor/
├── CMakeLists.txt              # Build configuration
├── main.cpp                    # Application entry point
├── mainwindow.h/.cpp          # Main window implementation
├── questioneditor.h/.cpp      # Question editor base and implementations
├── configdialog.h/.cpp        # Settings configuration dialog
├── questiontemplate.h/.cpp    # Question template definitions
├── jsonhandler.h/.cpp         # JSON file operations
├── questiontypes.h            # Question type enumerations and utilities
├── README.md                  # This file
└── example_questions.json     # Example questions file
```

## 🎯 Usage

1. **Launch the application**
2. **Configure settings** (⚙️ Settings) to set WifeyMOOC app path
3. **Create new JSON** (✨ New JSON) or **load existing** (💾 Load JSON)
4. **Add questions** (✨ New Question) and select question type
5. **Edit questions** by clicking on them in the list
6. **Save your work** (💖 Save JSON)
7. **Launch WifeyMOOC** (🚀 Save & Launch) to test your questions

## 🔒 Error Handling

The editor includes robust error handling for:
- **Unknown question types** (displays raw JSON editor)
- **Invalid JSON files** (shows error messages)
- **Missing WifeyMOOC app** (prompts for configuration)
- **File operation failures** (detailed error reporting)

## ⚙️ Configuration

The editor stores settings using Qt's QSettings:
- **WifeyMOOC application path**
- **Window geometry and state**
- **Recent files** (future enhancement)

Settings are platform-specific:
- **Windows**: Registry
- **macOS**: Property lists
- **Linux**: INI files

## 💝 Design Philosophy

- **Full feature parity** with the original Python version
- **Enhanced error handling** and robustness
- **Native Qt6 performance** and appearance
- **Cross-platform compatibility**
- **Maintainable and extensible** code structure
- **Beautiful pink UI** that makes editing questions a joy! 💕

## 🚨 Unknown Question Types

When the editor encounters an unknown question type (like `image_tagging`), it:
1. **Doesn't crash** the application
2. **Shows a warning** about the unknown type
3. **Provides a raw JSON editor** for manual editing
4. **Preserves the original data** when saving

## 🎨 UI Theme

The application features a beautiful pink color scheme inspired by the original:
- **Light Pink** (#FFB6C1) - Background
- **Pink** (#FFC0CB) - Secondary elements
- **Hot Pink** (#FF69B4) - Accents and borders
- **Deep Pink** (#FF1493) - Buttons
- **Dark Magenta** (#8B008B) - Text
- **Papaya Whip** (#FFEFD5) - Input fields

## 📝 Question Type Support

All question types from the original Python version are fully supported:

### Implemented Editors
- ✅ **ListPickEditor** - Multiple selection questions
- ✅ **McqSingleEditor** - Single choice questions
- ✅ **McqMultipleEditor** - Multiple choice questions
- ✅ **WordFillEditor** - Fill-in-the-blank questions
- ✅ **UnknownQuestionEditor** - Fallback for unsupported types

### Planned Enhancements
- 🔮 **MatchSentenceEditor** - Match sentences to images
- 🔮 **SequenceAudioEditor** - Audio sequence ordering
- 🔮 **OrderPhraseEditor** - Phrase ordering
- 🔮 **CategorizationEditor** - Item categorization
- 🔮 **FillBlanksDropdownEditor** - Dropdown-based fill blanks
- 🔮 **MatchPhrasesEditor** - Phrase matching

## 🐛 Known Issues

- Some advanced question types need additional editor implementations
- Media file preview not yet implemented
- Drag-and-drop reordering not yet implemented

## 🤝 Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Follow the existing code style
4. Add appropriate comments and documentation
5. Test thoroughly
6. Submit a pull request

## 📄 License

This project is provided as-is for educational and development purposes.

## 💖 Credits

Originally inspired by the Python WifeyMOOC JSON Editor.
Ported to C++/Qt6 with love and lots of pink! 💕

---

*Made with 💖 for all the girlies who love creating amazing educational content!*
