#!/usr/bin/env python3

"""
💖 Wifey MOOC JSON Editor v2.0 (Super AI External Prompts Edition!) 💖
A complete WYSIWYG editor for WifeyMOOC quiz questions
Now with a magical AI assistant and prompts that you can edit in prompts.json! ✨

Created with love and lots of pink! 💕

**AI SETUP NOTE:**
To enable the AI features, please install the necessary libraries by running this command in your terminal:
pip install requests

"""


import tkinter as tk
from tkinter import ttk, filedialog, messagebox, scrolledtext
import json
import os
import subprocess
from typing import Dict, List, Any
import threading
import random


class WifeyMOOCEditor:
    def __init__(self, root):
        self.root = root
        self.root.title("💖 Wifey MOOC JSON Editor v2.0 💖")
        self.root.geometry("1400x900")  # Bigger for new features!

        # ✨ Put your secret key here, babe! ✨
        self.GOOGLE_API_KEY = "put ur own key here"
        # Ultra-pink color scheme - even cuter with better readability! 💕
        self.colors = {
            'bg': '#FFB6C1',        # Light pink
            'secondary': '#FFC0CB', # Pink
            'accent': '#FF69B4',    # Hot pink
            'text': '#8B008B',      # Dark magenta
            'white': "#FFFFFF",     # White
            'button': '#FF1493',    # Deep pink
            'entry': '#FFEFD5',     # Papaya whip
            'success': '#98FB98',   # Pale green
            'warning': '#FFB347',   # Peach
            'button_text': '#000000'
        }

        self.wifeymooc_app_path = "/Applications/WifeyMOOC.app/Contents/MacOS/WifeyMOOC"

        # Data storage
        self.questions = []
        self.current_file = None
        self.current_question_index = None
        self.question_generator = None

        # ✨ Prompts are now loaded from an external file! ✨
        self.prompt_templates = {}
        self.load_prompts()

        # Updated question type templates - all the new cuteness! ✨
        self.question_templates = {
            'list_pick': {
                'type': 'list_pick',
                'question': 'Pick all the cute options you want! 💖',
                'options': ['Option 1', 'Option 2', 'Option 3'],
                'answer': [0]
            },
            'mcq_single': {
                'type': 'mcq_single',
                'question': 'Choose the best answer, babe! 💕',
                'options': [
                    {'image': 'image1.jpg', 'text': 'Option A'},
                    {'image': 'image2.jpg', 'text': 'Option B'},
                    {'image': 'image3.jpg', 'text': 'Option C'}
                ],
                'answer': [0],
                'media': None
            },
            'mcq_multiple': {
                'type': 'mcq_multiple',
                'question': 'Pick all the right answers, sweetie! ',
                'options': [
                    {'image': 'image1.jpg', 'text': 'Option A'},
                    {'image': 'image2.jpg', 'text': 'Option B'},
                    {'image': 'image3.jpg', 'text': 'Option C'}
                ],
                'answer': [0, 1],
                'media': None
            },
            'word_fill': {
                'type': 'word_fill',
                'question': 'Fill in the cute blanks! 💕',
                'sentence_parts': ['Fill this ', ' with the perfect word ', ' please!'],
                'answers': ['blank', 'darling'],
                'media': None
            },
            'match_sentence': {
                'type': 'match_sentence',
                'question': 'Match the adorable sentences with images! 💖',
                'pairs': [
                    {'sentence': 'Cute sentence 1', 'image_path': 'image1.jpg'},
                    {'sentence': 'Sweet sentence 2', 'image_path': 'image2.jpg'}
                ],
                'answer': {'image1.jpg': 'Cute sentence 1', 'image2.jpg': 'Sweet sentence 2'},
                'media': None
            },
            'sequence_audio': {
                'type': 'sequence_audio',
                'question': 'Put these sweet sounds in order! 🎵',
                'audio_options': [
                    {'option': 'First lovely sound'},
                    {'option': 'Second amazing sound'}
                ],
                'answer': [0, 1],
                'media': {'audio': 'audio.mp3'}
            },
            'order_phrase': {
                'type': 'order_phrase',
                'question': 'Put these phrases in the right order, honey! 💕',
                'phrase_shuffled': ['Second phrase', 'First phrase', 'Third phrase'],
                'answer': ['First phrase', 'Second phrase', 'Third phrase'],
                'media': None
            },
            'categorization_multiple': {
                'type': 'categorization_multiple',
                'question': 'Categorize these cute items! 📂',
                'stimuli': [
                    {'text': 'Adorable Item 1', 'image': None},
                    {'text': 'Sweet Item 2', 'image': None}
                ],
                'categories': [' ', 'Category A', 'Category B'],
                'answer': {'Adorable Item 1': 'Category A', 'Sweet Item 2': 'Category B'},
                'media': None
            },
            'fill_blanks_dropdown': {
                'type': 'fill_blanks_dropdown',
                'question': 'Choose from the dropdowns, sweetie! ⬇️',
                'sentence_parts': ['Choose ', ' and then ', ' from these cute dropdowns.'],
                'options_for_blanks': [
                    [' ', 'option1', 'option2'],
                    [' ', 'choice1', 'choice2']
                ],
                'answers': ['option1', 'choice1'],
                'media': None
            },
            'match_phrases': {
                'type': 'match_phrases',
                'question': 'Match the phrase beginnings with their perfect endings! 💖',
                'pairs': [{
                    'source': 'Beginning of cute phrase 1...',
                    'targets': [' ', 'ending A', 'ending B', 'ending C']
                }],
                'answer': {'Beginning of cute phrase 1...': 'ending A'},
                'media': None
            },
            'multi_questions': {
                'type': 'multi_questions',
                'questions': [{
                    'type': 'mcq_single',
                    'question': 'First cute question! 💖',
                    'options': [
                        {'image': 'image1.jpg', 'text': 'Option 1'},
                        {'image': 'image2.jpg', 'text': 'Option 2'}
                    ],
                    'answer': [0]
                }, {
                    'type': 'mcq_multiple',
                    'question': 'Second adorable question! 💕',
                    'options': [
                        {'image': 'image3.jpg', 'text': 'Choice A'},
                        {'image': 'image4.jpg', 'text': 'Choice B'}
                    ],
                    'answer': [0, 1]
                }]
            },
            'image_tagging': {
                'type': 'image_tagging',
                'question': 'Tag the cute image! 💖',
                'media': {'image': 'body.jpg'},
                'button_label': 'Alternative View',
                'tags': [
                    {'label': 'Tag 1', 'id': 'tag1'},
                    {'label': 'Tag 2', 'id': 'tag2'}
                ],
                'answer': {
                    'tag1': [100, 150],
                    'tag2': [200, 250]
                },
                'alternatives': []
            }
        }

        self.setup_ui()

    def get_default_prompts(self):
        """Returns the default prompts dictionary, just in case prompts.json is missing!"""
        return {
            'All-60': """Your task is to use the following text as INSPIRATION to generate a 60 self-contained questions in French. The questions MUST be understandable and answerable without having read the source text. The questions difficulty MUST be targetting an A2 level student, with a heavy bias toward conjugation while still generating some on other subjects, The generator should note that the student is a giant dyke, and in that aspect, the questions should have a sapphic twist.

Preferably the entire sets of question will tell a story

Return your answer as a single JSON array of objects. Each object represents a single question and MUST include a "q_type" key to identify the question format.

Here are the possible formats:

1.  **"q_type": "MCQ Single Choice"**: A general knowledge question inspired by a theme or word in the text.
    Keys: "question", "réponse" (the single correct string), "distracteurs" (an array of 3 incorrect strings).

2.  **"q_type": "MCQ Multiple Choice"**: A general knowledge question inspired by a theme or word in the text.
    Keys: "question", "réponses" (an array of all correct strings), "distracteurs" (an array of incorrect strings).

3.  **"q_type": "Fill in the Blanks"**: Create a NEW sentence using a key vocabulary word from the text as the blank.
    Keys: "sentence_parts" (array of strings), "answers" (array with the single correct word).

4.  **"q_type": "Fill in the Blanks (Dropdown)"**: Create a NEW sentence using a key vocabulary word from the text as dropdowns.
    Keys: "sentence_parts" (array of strings), "options_for_blanks": (an array of arrays, where each inner array contains the string options for the corresponding blank. Make sure the correct answer is one of the options.), "answers" (array with the single correct word).

5.  **"q_type": "Order the Phrase"**: Take a simple, self-contained sentence from the text.
    Keys: "question", "réponse" (correctly ordered array of strings), "phrase_shuffled" (shuffled array of strings).

6.  **"q_type": "Categorization"**: Create a categorization question based on a theme from the text.
    Keys: "question", "categories" (array of strings), "stimuli" (array of strings to categorize), "answer" (a dictionary mapping stimuli to categories).

7.  **"q_type": "List Pick"**: Create a question where multiple items from a list can be chosen, based on a theme from the text.
    Keys: "question", "options" (array of all possible strings), "réponses" (array of the correct strings).

Generate 20 MCQ Single Choice, 15 MCQ Multiple Choice, 10 Fill in the Blanks (Dropdown), 5 List Pick, 5 Categorization, 4 Order the Phrase and 1 Fill in the Blanks question. Do not include any other text or explanation outside of the JSON array. Randomize the order, except for the 1 Fill in the Blanks that should be at the very end.

Text for inspiration:
---
{text}
---""",
            'All': """Your task is to use the following text as INSPIRATION to generate a variety of self-contained questions in French. The questions MUST be understandable and answerable without having read the source text.

Return your answer as a single JSON array of objects. Each object represents a single question and MUST include a "q_type" key to identify the question format.

Here are the possible formats:

1.  **"q_type": "MCQ Single Choice"**: A general knowledge question inspired by a theme or word in the text.
    Keys: "question", "réponse" (the single correct string), "distracteurs" (an array of 3 incorrect strings).

2.  **"q_type": "MCQ Multiple Choice"**: A general knowledge question inspired by a theme or word in the text.
    Keys: "question", "réponses" (an array of all correct strings), "distracteurs" (an array of incorrect strings).

3.  **"q_type": "Fill in the Blanks"**: Create a NEW sentence using a key vocabulary word from the text as the blank.
    Keys: "sentence_parts" (array of strings), "answers" (array with the single correct word).

4.  **"q_type": "Fill in the Blanks (Dropdown)"**: Create a NEW sentence using a key vocabulary word from the text as dropdowns.
    Keys: "sentence_parts" (array of strings), "options_for_blanks": (an array of arrays, where each inner array contains the string options for the corresponding blank. Make sure the correct answer is one of the options.), "answers" (array with the single correct word).

5.  **"q_type": "Order the Phrase"**: Take a simple, self-contained sentence from the text.
    Keys: "question", "réponse" (correctly ordered array of strings), "phrase_shuffled" (shuffled array of strings).

6.  **"q_type": "Categorization"**: Create a categorization question based on a theme from the text.
    Keys: "question", "categories" (array of strings), "stimuli" (array of strings to categorize), "answer" (a dictionary mapping stimuli to categories).

7.  **"q_type": "List Pick"**: Create a question where multiple items from a list can be chosen, based on a theme from the text.
    Keys: "question", "options" (array of all possible strings), "réponses" (array of the correct strings).

Generate a mix of these question types. Do not include any other text or explanation outside of the JSON array.

Text for inspiration:
---
{text}
---""",
            'MCQ Single Choice': """Your task is to use the following text as INSPIRATION to generate a list of simple, clear, self-contained multiple-choice questions in French. The questions must be understandable and answerable without having read the source text.

For each question, provide:
1. A self-contained question ("question").
2. The single correct answer ("réponse").
3. A list of three plausible but incorrect answers ("distracteurs").

Return your answer as a single JSON array of objects. Each object must have three keys: "question", "réponse", and "distracteurs".

Text for inspiration:
---
{text}
---""",
            'MCQ Multiple Choice': """Your task is to use the following text as INSPIRATION to generate a list of simple, clear, self-contained multiple-choice questions in French where there can be multiple correct answers. The questions must be understandable and answerable without having read the source text.

For each question, provide:
1. A self-contained question ("question").
2. A list of all correct answers ("réponses").
3. A list of plausible but incorrect answers ("distracteurs").

Return your answer as a single JSON array of objects. Each object must have three keys: "question", "réponses", and "distracteurs".

Text for inspiration:
---
{text}
---""",
            'Fill in the Blanks': """Your task is to use the vocabulary from the following text to create NEW "fill-in-the-blank" questions in French. The new sentences you create should be general knowledge or easily understandable on their own.

For each question, choose an important word from the source text and create a new sentence where that word is the blank.

Return your answer as a single JSON array of objects. Each object must have two keys:
1. "sentence_parts": an array of strings for the new sentence.
2. "answers": an array containing the single correct word for the blank.

Example using "arc-en-ciel" from a text:
{
  "sentence_parts": ["Après la pluie, on peut parfois voir un ", " dans le ciel."],
  "answers": ["arc-en-ciel"]
}

Text for vocabulary inspiration:
---
{text}
---""",
            'Order the Phrase': """Your task is to take simple, self-contained sentences from the following text and turn them into "order the phrase" questions. Prefer sentences that are understandable on their own without needing the original text's context.

For each question, provide:
1. The question itself (e.g., "Mets les mots dans le bon ordre.").
2. The correctly ordered phrase as a list of strings ("réponse").
3. The shuffled phrase as a list of strings ("phrase_shuffled").

Return your answer as a single JSON array of objects. Each object must have three keys: "question", "réponse", and "phrase_shuffled".

Text:
---
{text}
---""",
            'Categorization': """Your task is to use the themes from the following text to generate self-contained categorization questions in French. The questions must be understandable and answerable without having read the source text.

For each question, provide:
1. The question itself.
2. A list of categories as strings.
3. A list of items (stimuli) to be categorized. Each item should be a simple string.
4. An answer mapping, where each key is an item and its value is the correct category.

Return your answer as a single JSON array of objects. Each object must have four keys: "question", "categories", "stimuli", and "answer".

Text for inspiration:
---
{text}
---""",
            'Fill in the Blanks (Dropdown)': """Your task is to use the vocabulary from the following text to create NEW "fill-in-the-blank" questions with dropdown menus in French. The new sentences you create should be general knowledge or easily understandable on their own.

For each question, provide:
1. "sentence_parts": an array of strings representing the new sentence with blanks.
2. "options_for_blanks": an array of arrays, where each inner array contains the string options for the corresponding blank. Make sure the correct answer is one of the options.
3. "answers": an array containing the single correct string for each blank.

Example using "arc-en-ciel" from a text:
{
  "sentence_parts": ["Après la pluie, on peut parfois voir un ", " dans le ciel."],
  "options_for_blanks": [["arc-en-ciel", "nuage", "soleil"]],
  "answers": ["arc-en-ciel"]
}

Text for vocabulary inspiration:
---
{text}
---""",
            'Match Phrases': """Your task is to take simple, self-contained sentences from the following text and create "match the phrases" questions in French. Split the sentences into a beginning ('source') and an ending ('target').

For each question, provide:
1. The question itself.
2. A list of 'pairs'. Each pair object should have a 'source' (the beginning of the phrase) and a list of possible 'targets' (endings), including the correct one.
3. The 'answer' mapping, where the key is the 'source' and the value is the correct 'target'.

Return your answer as a single JSON array of objects. Each object must have three keys: "question", "pairs", and "answer".

Text:
---
{text}
---""",
            'List Pick': """Your task is to use the themes from the following text to generate self-contained "list pick" questions in French, where the user can select multiple correct items from a list. The questions must be understandable and answerable without having read the source text.

For each question, provide:
1. The question itself.
2. A list of string options, including both correct and incorrect items.
3. A list of all correct answers ("réponses").

Return your answer as a single JSON array of objects. Each object must have three keys: "question", "options", and "réponses".

Text for inspiration:
---
{text}
---"""
        }

    def load_prompts(self):
        """Loads prompts from prompts.json, creating it if it doesn't exist."""
        try:
            with open('prompts.json', 'r', encoding='utf-8') as f:
                self.prompt_templates = json.load(f)
        except (FileNotFoundError, json.JSONDecodeError):
            print("💖 'prompts.json' not found or is invalid, creating a new one for you, babe! 💖")
            self.prompt_templates = self.get_default_prompts()
            try:
                with open('prompts.json', 'w', encoding='utf-8') as f:
                    json.dump(self.prompt_templates, f, indent=2, ensure_ascii=False)
            except Exception as e:
                print(f"😢 Could not write to prompts.json: {e}")
                messagebox.showerror("Error", "Could not create the prompts.json file, sweetie!")


    def setup_ui(self):
        """Setup the most gorgeous pink UI ever! 💖"""
        # Main frame
        main_frame = tk.Frame(self.root, bg=self.colors['bg'])
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        # Ultra-cute title
        title = tk.Label(main_frame, text="💖 Wifey MOOC Question Editor v2.0 💖",
                        font=('Comic Sans MS', 22, 'bold'),
                        bg=self.colors['bg'], fg=self.colors['text'])
        title.pack(pady=10)

        # Top buttons frame - even more functional!
        buttons_frame = tk.Frame(main_frame, bg=self.colors['bg'])
        buttons_frame.pack(fill=tk.X, pady=5)

        tk.Button(buttons_frame, text="✨ New JSON", command=self.new_json,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

        tk.Button(buttons_frame, text="💾 Load JSON", command=self.load_json,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

        tk.Button(buttons_frame, text="💖 Save JSON", command=self.save_json,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

        tk.Button(buttons_frame, text="💖 Save As", command=self.save_json_as,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

        tk.Button(buttons_frame, text="🚀 Save & Launch", command=self.save_and_launch,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

        tk.Button(buttons_frame, text="✨ New Question", command=self.show_add_question_dialog,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

        tk.Button(buttons_frame, text="✨ Generate with AI ✨", command=self.show_ai_generator_dialog,
                 bg='#77dd77', fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=10)



        # Main content area - two gorgeous panels
        content_frame = tk.Frame(main_frame, bg=self.colors['bg'])
        content_frame.pack(fill=tk.BOTH, expand=True, pady=10)

        # Left panel - questions list (beautifully organized!)
        left_frame = tk.Frame(content_frame, bg=self.colors['secondary'], relief=tk.RAISED, bd=2)
        left_frame.pack(side=tk.LEFT, fill=tk.Y, padx=(0, 5))

        tk.Label(left_frame, text="💕 Questions List 💕", font=('Arial', 14, 'bold'),
                bg=self.colors['secondary'], fg=self.colors['text']).pack(pady=5)

        # Questions listbox with scrollbar
        list_frame = tk.Frame(left_frame)
        list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        self.questions_listbox = tk.Listbox(list_frame, width=35, height=25,
                                          bg=self.colors['white'],
                                          selectbackground=self.colors['accent'],
                                          font=('Arial', 9))
        scrollbar_list = tk.Scrollbar(list_frame, orient=tk.VERTICAL)
        self.questions_listbox.config(yscrollcommand=scrollbar_list.set)
        scrollbar_list.config(command=self.questions_listbox.yview)

        self.questions_listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar_list.pack(side=tk.RIGHT, fill=tk.Y)

        self.questions_listbox.bind('<<ListboxSelect>>', self.on_question_select)

        # List management buttons - ultra-cute and functional!
        list_buttons_frame = tk.Frame(left_frame, bg=self.colors['secondary'])
        list_buttons_frame.pack(fill=tk.X, padx=5, pady=5)

        tk.Button(list_buttons_frame, text="🗑️ Delete", command=self.delete_question,
                 bg='#FF6347', fg=self.colors['button_text'], font=('Arial', 9, 'bold')).pack(side=tk.LEFT, padx=2)

        tk.Button(list_buttons_frame, text="⬆️ Up", command=self.move_question_up,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 9, 'bold')).pack(side=tk.LEFT, padx=2)

        tk.Button(list_buttons_frame, text="⬇️ Down", command=self.move_question_down,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 9, 'bold')).pack(side=tk.LEFT, padx=2)

        # Right panel - question editor (the main attraction!)
        self.right_frame = tk.Frame(content_frame, bg=self.colors['white'], relief=tk.RAISED, bd=2)
        self.right_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

        # Welcome message
        self.show_welcome_message()

    # --- ✨ AI GENERATION MAGIC! ✨ ---

    def show_ai_generator_dialog(self):
        """Shows the super cute dialog for generating questions with AI, now with offline mode!"""
        dialog = tk.Toplevel(self.root)
        dialog.title("✨ AI Question Generation (Super-Powered!) ✨")
        dialog.geometry("800x850")
        dialog.configure(bg=self.colors['bg'])
        dialog.transient(self.root)
        dialog.grab_set()

        # --- Main Frame ---
        main_frame = tk.Frame(dialog, bg=self.colors['bg'])
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        # --- Top Controls ---
        top_controls_frame = tk.Frame(main_frame, bg=self.colors['bg'])
        top_controls_frame.pack(fill=tk.X)

        # Question Type Selector
        type_frame = tk.Frame(top_controls_frame, bg=self.colors['bg'])
        type_frame.pack(side=tk.LEFT, pady=5)
        tk.Label(type_frame, text="💖 Question Type:", font=('Comic Sans MS', 12, 'bold'),
                 bg=self.colors['bg'], fg=self.colors['text']).pack(side=tk.LEFT, padx=5)

        self.question_type_var = tk.StringVar(value='All-60')
        question_types_for_ai = ['All-60', 'All', 'MCQ Single Choice', 'MCQ Multiple Choice', 'Fill in the Blanks',
                                 'Order the Phrase', 'Categorization', 'Fill in the Blanks (Dropdown)',
                                 'Match Phrases', 'List Pick']
        question_type_combo = ttk.Combobox(type_frame, textvariable=self.question_type_var,
                                           values=question_types_for_ai, state='readonly', font=('Arial', 11))
        question_type_combo.pack(side=tk.LEFT, padx=5)

        # --- Source Text Input ---
        tk.Label(main_frame, text="💖 Paste your French text for inspiration below, babe! 💖",
                 font=('Comic Sans MS', 14, 'bold'), bg=self.colors['bg'], fg=self.colors['text']).pack(pady=(10, 5))
        source_text = scrolledtext.ScrolledText(main_frame, height=6, width=80, wrap=tk.WORD, font=('Arial', 11))
        source_text.pack(pady=5, fill=tk.X)
        source_text.insert('1.0', "Lola et Inès sont allées à la Marche des Fiertés samedi dernier. C'était une belle journée ensoleillée à Paris. Lola portait une robe arc-en-ciel et Inès tenait une pancarte colorée.")

        # --- Offline Mode Checkbox ---
        offline_mode_var = tk.BooleanVar(value=False)
        offline_check = tk.Checkbutton(main_frame, text="📝 Offline Mode (Manual Copy/Paste)",
                                       variable=offline_mode_var, font=('Arial', 11, 'bold'),
                                       bg=self.colors['bg'], fg=self.colors['text'])
        offline_check.pack(pady=10)

        # --- Online Mode Frame & Button ---
        online_frame = tk.Frame(main_frame, bg=self.colors['bg'])
        online_frame.pack(fill=tk.BOTH, expand=True)

        generate_button = tk.Button(online_frame, text="✨ Generate Questions! ✨",
                                    command=lambda: self.run_ai_question_generation(source_text.get('1.0', tk.END), prompt_editor.get('1.0', tk.END), dialog),
                                    bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 12, 'bold'))
        generate_button.pack(pady=10)

        # --- Offline Mode Frame & Widgets ---
        offline_frame = tk.LabelFrame(main_frame, text="📝 Offline Generation Steps 📝",
                                      font=('Arial', 12, 'bold'), bg=self.colors['secondary'],
                                      fg=self.colors['text'])

        # Frame for the button at the bottom
        offline_button_frame = tk.Frame(offline_frame, bg=self.colors['secondary'])
        offline_button_frame.pack(side=tk.BOTTOM, fill=tk.X)

        # Frame for the content that will expand
        offline_content_frame = tk.Frame(offline_frame, bg=self.colors['secondary'])
        offline_content_frame.pack(fill=tk.BOTH, expand=True)

        tk.Label(offline_content_frame, text="Step 1: Copy this prompt and paste it into your AI tool 💕",
                 font=('Arial', 10, 'italic'), bg=self.colors['secondary']).pack(pady=5)
        prompt_output_text = scrolledtext.ScrolledText(offline_content_frame, height=8, width=80, wrap=tk.WORD, font=('Arial', 10))
        prompt_output_text.pack(pady=5, padx=10, fill=tk.BOTH, expand=True)
        prompt_output_text.config(state='disabled') # Read-only!

        tk.Label(offline_content_frame, text="Step 2: Paste the complete JSON response from your AI here ✨",
                 font=('Arial', 10, 'italic'), bg=self.colors['secondary']).pack(pady=5)
        response_input_text = scrolledtext.ScrolledText(offline_content_frame, height=8, width=80, wrap=tk.WORD, font=('Arial', 10))
        response_input_text.pack(pady=5, padx=10, fill=tk.BOTH, expand=True)

        process_button = tk.Button(offline_button_frame, text="💖 Process Pasted JSON 💖",
                                   command=lambda: self.process_pasted_json(response_input_text.get('1.0', tk.END), dialog),
                                   bg='#77dd77', fg=self.colors['button_text'], font=('Arial', 12, 'bold'))
        process_button.pack(pady=10)


        # --- Prompt Editor (shared logic) ---
        prompt_editor_frame = tk.LabelFrame(main_frame, text="💖 AI Prompt (Edit in prompts.json!) 💖",
                                            font=('Arial', 12, 'bold'), bg=self.colors['bg'],
                                            fg=self.colors['text'])
        prompt_editor_frame.pack(pady=10, fill=tk.BOTH, expand=True)
        prompt_editor = scrolledtext.ScrolledText(prompt_editor_frame, height=10, width=80, wrap=tk.WORD, font=('Arial', 10))
        prompt_editor.pack(pady=5, fill=tk.BOTH, expand=True)

        def update_prompt_text(event=None):
            """Updates the prompt editor and the offline prompt display. So clever!"""
            q_type = self.question_type_var.get()
            template = self.prompt_templates.get(q_type, "Something went wrong, babe! Check your prompts.json file!")
            prompt_editor.config(state='normal')
            prompt_editor.delete('1.0', tk.END)
            prompt_editor.insert('1.0', template)
            prompt_editor.config(state='disabled') # Read-only in the app now
            # Also update the offline view if it's active
            if offline_mode_var.get():
                toggle_offline_mode()

        question_type_combo.bind('<<ComboboxSelected>>', update_prompt_text)

        def toggle_offline_mode():
            """Shows or hides the UI elements based on the checkbox state."""
            if offline_mode_var.get():
                # --- OFFLINE MODE IS ON ---
                online_frame.pack_forget()
                offline_frame.pack(fill=tk.BOTH, expand=True, pady=10)

                # Generate and display the prompt to be copied
                prompt_template = self.prompt_templates.get(self.question_type_var.get(), "")
                text_for_prompt = source_text.get('1.0', tk.END)
                full_prompt = prompt_template.format(text=text_for_prompt)

                prompt_output_text.config(state='normal')
                prompt_output_text.delete('1.0', tk.END)
                prompt_output_text.insert('1.0', full_prompt)
                prompt_output_text.config(state='disabled')

            else:
                # --- OFFLINE MODE IS OFF ---
                offline_frame.pack_forget()
                online_frame.pack(fill=tk.BOTH, expand=True)

        offline_check.config(command=toggle_offline_mode)

        # Initialize UI state
        update_prompt_text()
        toggle_offline_mode()

    def process_pasted_json(self, pasted_text, dialog):
        """Processes the JSON text pasted by the user in offline mode."""
        question_type = self.question_type_var.get()
        generated_items = []

        if not pasted_text.strip():
            messagebox.showwarning("Oopsie!", "The text box is empty, sweetie! Please paste the AI's JSON response. 💕")
            return

        try:
            # The logic here is similar to parsing the response in the _ai_worker
            cleaned_text = pasted_text.strip()
            if "```json" in cleaned_text:
                cleaned_text = cleaned_text.split("```json")[1].split("```")[0].strip()

            loaded_json = json.loads(cleaned_text)
            item_list = loaded_json if isinstance(loaded_json, list) else [loaded_json]

            for item in item_list:
                if question_type not in ['All', 'All-60']:
                    item['q_type'] = question_type
                generated_items.append(item)

            self._show_ai_results(generated_items, dialog)

        except json.JSONDecodeError:
            messagebox.showerror("JSON Error 😢", "I couldn't understand the pasted text, babe. Please make sure it's valid JSON!")
        except Exception as e:
            messagebox.showerror("Error 😢", f"Something went wrong while processing the JSON:\n{e}")

    def run_ai_question_generation(self, text, prompt_template, dialog):
        """The core function that runs the AI model in the background."""
        api_key = self.GOOGLE_API_KEY
        if "PASTE_YOUR_KEY_HERE" in api_key or "it's a secret" in api_key:
            messagebox.showwarning("Oopsie!", "Please paste your Google API key into the code, sweetie! 💕")
            return

        question_type = self.question_type_var.get()

        dialog.config(cursor="watch")
        dialog.update()

        thread = threading.Thread(target=self._ai_worker, args=(text, api_key, question_type, prompt_template, dialog))
        thread.start()

    def _ai_worker(self, text, api_key, question_type, prompt_template, dialog):
        """This is where the AI does its thinking!"""
        try:
            import requests
            import json
            import traceback

            # The prompt is now built from your editable template!
            prompt = prompt_template.format(text=text)

            url = f"[https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash-preview-05-20:generateContent?key=](https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash-preview-05-20:generateContent?key=){api_key}"

            payload = {
                "contents": [{"parts": [{"text": prompt}]}],
                "generationConfig": {"response_mime_type": "application/json"}
            }
            headers = {'Content-Type': 'application/json'}

            response = requests.post(url, json=payload, headers=headers)
            response.raise_for_status()

            response_json = response.json()
            generated_items = []
            new_text = ""

            if (response_json and "candidates" in response_json and response_json["candidates"] and
                "content" in response_json["candidates"][0] and "parts" in response_json["candidates"][0]["content"]):
                new_text = response_json["candidates"][0]["content"]["parts"][0]["text"].strip()
            else:
                print(f"Gemini response was empty or malformed. Full response: {response_json}")

            if new_text:
                print(f"--- AI Raw Output ---\n{new_text}\n---------------------")
                try:
                    # It's better to handle potential JSON arrays at the root
                    loaded_json = json.loads(new_text)
                    if isinstance(loaded_json, list):
                        item_list = loaded_json
                    else: # If it's a single object
                        item_list = [loaded_json]

                    for item in item_list:
                        if question_type != 'All' and question_type != 'All-60':
                            item['q_type'] = question_type
                        # If it is 'All', we trust the AI has provided the 'q_type' key as requested in the prompt.
                        generated_items.append(item)
                except json.JSONDecodeError:
                    print("AI did not return valid JSON. Sad face.")
                    # Let's try to be clever and fix it!
                    try:
                        # Sometimes the AI wraps the JSON in markdown ```json ... ```
                        if "```json" in new_text:
                            cleaned_text = new_text.split("```json")[1].split("```")[0].strip()
                            loaded_json = json.loads(cleaned_text)
                            if isinstance(loaded_json, list):
                                item_list = loaded_json
                            else:
                                item_list = [loaded_json]

                            for item in item_list:
                                if question_type != 'All' and question_type != 'All-60':
                                    item['q_type'] = question_type
                                generated_items.append(item)
                    except Exception as e:
                        print(f"Still couldn't parse the JSON, even after cleaning. Error: {e}")


            self.root.after(0, self._show_ai_results, generated_items, dialog)

        except requests.exceptions.RequestException as e:
            print(f"--- DETAILED NETWORK ERROR ---\n{e}\n------------------------------")
            self.root.after(0, messagebox.showerror, "AI Network Error 😢", f"Could not connect to the AI service:\n{e}")
        except Exception as e:
            print("--- DETAILED AI ERROR TRACEBACK ---")
            traceback.print_exc()
            print("------------------------------------")
            self.root.after(0, messagebox.showerror, "AI Error 😢", f"Something went wrong with the AI magic:\n{e}")
        finally:
            self.root.after(0, dialog.config, {"cursor": ""})

    def _show_ai_results(self, generated_items, parent_dialog):
        """Displays the generated questions for you to choose from."""
        if not generated_items:
            messagebox.showinfo("AI Result", "The AI couldn't think of any questions this time, sweetie. Try a different text! 💕")
            parent_dialog.config(cursor="")
            return

        results_dialog = tk.Toplevel(self.root)
        results_dialog.title("✨ AI Suggestions! ✨")
        results_dialog.geometry("700x600")
        results_dialog.configure(bg=self.colors['bg'])
        results_dialog.transient(parent_dialog)
        results_dialog.grab_set()

        tk.Label(results_dialog, text="💖 Pick the questions you love! 💖",
                font=('Comic Sans MS', 16, 'bold'),
                bg=self.colors['bg'], fg=self.colors['text']).pack(pady=10)

        container = tk.Frame(results_dialog)
        container.pack(fill=tk.BOTH, expand=True, padx=10)

        canvas = tk.Canvas(container, bg=self.colors['white'])
        scrollbar = ttk.Scrollbar(container, orient="vertical", command=canvas.yview)
        results_frame = tk.Frame(canvas, bg=self.colors['white'])

        canvas.configure(yscrollcommand=scrollbar.set)
        canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        canvas.create_window((0, 0), window=results_frame, anchor="nw")

        def on_frame_configure(event):
            canvas.configure(scrollregion=canvas.bbox("all"))

        results_frame.bind("<Configure>", on_frame_configure)

        selected_items = []

        for item in generated_items:
            var = tk.BooleanVar(value=True)
            display_text = ""
            q_type = item.get('q_type', '')

            if q_type == 'MCQ Single Choice':
                display_text = f"Q: {item.get('question', 'N/A')}\nA: {item.get('réponse', 'N/A')}"
            elif q_type == 'MCQ Multiple Choice':
                 display_text = f"Q: {item.get('question', 'N/A')}\nA: {', '.join(item.get('réponses', ['N/A']))}"
            elif q_type == 'Fill in the Blanks':
                blank_sentence = " [___] ".join(item.get('sentence_parts', []))
                display_text = f"Fill in the blank:\n{blank_sentence}\nAnswer: {item.get('answers', ['N/A'])[0]}"
            elif q_type == 'Order the Phrase':
                correct_phrase = " ".join(item.get('réponse', []))
                display_text = f"Order the phrase:\n{correct_phrase}"
            elif q_type == 'Categorization':
                cats = ', '.join(item.get('categories', []))
                stims = ', '.join(item.get('stimuli', []))
                display_text = f"Q: {item.get('question', 'N/A')}\nCategories: {cats}\nItems: {stims}"
            elif q_type == 'Fill in the Blanks (Dropdown)':
                blank_sentence = " [___] ".join(item.get('sentence_parts', []))
                answers = ', '.join(item.get('answers', []))
                display_text = f"Fill with dropdown:\n{blank_sentence}\nAnswers: {answers}"
            elif q_type == 'Match Phrases':
                num_pairs = len(item.get('pairs', []))
                display_text = f"Q: {item.get('question', 'N/A')}\n({num_pairs} pairs to match)"
            elif q_type == 'List Pick':
                answers = ', '.join(item.get('réponses', []))
                display_text = f"Q: {item.get('question', 'N/A')}\nCorrect answers: {answers}"
            else:
                display_text = f"Unsupported question type: {q_type}"

            chk = tk.Checkbutton(results_frame, text=display_text, variable=var,
                                bg=self.colors['white'], wraplength=650, justify=tk.LEFT, anchor='w')
            chk.pack(anchor=tk.W, padx=5, pady=4)
            selected_items.append((var, item))

        def add_selected():
            for var, item in selected_items:
                if var.get():
                    q_type = item.get('q_type')
                    if q_type == 'MCQ Single Choice':
                        correct_answer = item['réponse']
                        options = [correct_answer] + item.get('distracteurs', ['A', 'B', 'C'])
                        random.shuffle(options)
                        answer_index = options.index(correct_answer)
                        formatted_options = [{'image': None, 'text': opt} for opt in options]
                        new_question = {
                            'type': 'mcq_single', 'question': item['question'],
                            'hint': "Basé sur le texte que tu as fourni, ma chérie!",
                            'options': formatted_options, 'answer': [answer_index], 'media': None}
                        self.questions.append(new_question)
                    elif q_type == 'MCQ Multiple Choice':
                        correct_answers = item['réponses']
                        all_options = correct_answers + item.get('distracteurs', ['D', 'E'])
                        random.shuffle(all_options)
                        answer_indices = [i for i, opt in enumerate(all_options) if opt in correct_answers]
                        formatted_options = [{'image': None, 'text': opt} for opt in all_options]
                        new_question = {
                            'type': 'mcq_multiple', 'question': item['question'],
                            'hint': "Basé sur le texte que tu as fourni, ma chérie!",
                            'options': formatted_options, 'answer': answer_indices, 'media': None}
                        self.questions.append(new_question)
                    elif q_type == 'Fill in the Blanks':
                        new_question = {
                            'type': 'word_fill', 'question': "Remplis les blancs, ma chérie!",
                            'sentence_parts': item['sentence_parts'], 'answers': item['answers'], 'media': None}
                        self.questions.append(new_question)
                    elif q_type == 'Order the Phrase':
                        new_question = {
                            'type': 'order_phrase', 'question': item.get('question', "Mets les mots dans le bon ordre."),
                            'phrase_shuffled': item['phrase_shuffled'], 'answer': item['réponse'], 'media': None}
                        self.questions.append(new_question)
                    elif q_type == 'Categorization':
                        stimuli_formatted = [{'text': s, 'image': None} for s in item.get('stimuli', [])]
                        new_question = {
                            'type': 'categorization_multiple', 'question': item.get('question', "Classe ces éléments dans les bonnes catégories."),
                            'stimuli': stimuli_formatted, 'categories': [' '] + item.get('categories', []),
                            'answer': item.get('answer', {}), 'media': None}
                        self.questions.append(new_question)
                    elif q_type == 'Fill in the Blanks (Dropdown)':
                        new_question = {
                            'type': 'fill_blanks_dropdown', 'question': "Choisis la bonne option dans les menus déroulants.",
                            'sentence_parts': item.get('sentence_parts', []), 'options_for_blanks': item.get('options_for_blanks', []),
                            'answers': item.get('answers', []), 'media': None}
                        self.questions.append(new_question)
                    elif q_type == 'Match Phrases':
                        new_question = {
                            'type': 'match_phrases', 'question': item.get('question', "Associe le début de chaque phrase avec la fin correcte."),
                            'pairs': item.get('pairs', []), 'answer': item.get('answer', {}), 'media': None}
                        self.questions.append(new_question)
                    elif q_type == 'List Pick':
                        options = item.get('options', [])
                        correct_answers = item.get('réponses', [])
                        answer_indices = [i for i, opt in enumerate(options) if opt in correct_answers]
                        new_question = {
                            'type': 'list_pick', 'question': item.get('question', "Sélectionne toutes les bonnes réponses."),
                            'options': options, 'answer': answer_indices, 'media': None}
                        self.questions.append(new_question)

            self.refresh_questions_list()
            results_dialog.destroy()
            parent_dialog.destroy()
            messagebox.showinfo("Success!", "Added the selected questions to your quiz, babe!")

        tk.Button(results_dialog, text="💖 Add Selected to Quiz 💖", command=add_selected,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 12, 'bold')).pack(pady=10)

    def show_welcome_message(self):
        """Show the most adorable welcome message ever! 💖"""
        for widget in self.right_frame.winfo_children():
            widget.destroy()
        welcome_label = tk.Label(self.right_frame,
                               text="💖 Welcome to Wifey MOOC Editor v2.0! 💖\n\n" +
                                    "✨ Prompts are now in prompts.json for easy editing! ✨\n\n" +
                                    "💕 Let's make the cutest quizzes for Sierra! 💕",
                               font=('Comic Sans MS', 16, 'bold'),
                               bg=self.colors['white'], fg=self.colors['text'],
                               justify=tk.CENTER)
        welcome_label.pack(expand=True)

    def new_json(self):
        if self.questions and not messagebox.askyesno("Create New File?", "All unsaved changes will be lost. Continue?"):
            return
        self.questions = []
        self.current_file = None
        self.current_question_index = None
        self.refresh_questions_list()
        self.show_welcome_message()
        messagebox.showinfo("New File", "New JSON file created! Start adding questions, babe!")

    def load_json(self):
        """Load questions from JSON file - pure magic! ✨"""
        file_path = filedialog.askopenfilename(
            title="💖 Select Your Cute Wifey MOOC JSON File 💖",
            filetypes=[("JSON files", "*.json"), ("All files", "*.*")])
        if file_path:
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    self.questions = json.load(f)
                self.current_file = file_path
                self.refresh_questions_list()
                messagebox.showinfo("Success! 💖", f"Loaded {len(self.questions)} adorable questions successfully!")
            except Exception as e:
                messagebox.showerror("Oopsie! 😢", f"Failed to load file, babe:\n{str(e)}")

    def save_json(self):
        """Save questions to JSON file - preserve all the cuteness! 💾"""
        if not self.questions:
            messagebox.showwarning("Warning! 💕", "No questions to save, sweetie!")
            return
        if not self.current_file:
            self.save_json_as()
            return
        try:
            with open(self.current_file, 'w', encoding='utf-8') as f:
                json.dump(self.questions, f, indent=2, ensure_ascii=False)
            messagebox.showinfo("Success! 💖", "Questions saved successfully, darling!")
        except Exception as e:
            messagebox.showerror("Oopsie! 😢", f"Failed to save file, honey:\n{str(e)}")

    def save_json_as(self):
        """Save questions to a new JSON file - spread the cuteness! 💕"""
        file_path = filedialog.asksaveasfilename(
            title="💖 Save Your Beautiful Wifey MOOC JSON File 💖",
            defaultextension=".json",
            filetypes=[("JSON files", "*.json"), ("All files", "*.*")])
        if file_path:
            self.current_file = file_path
            self.save_json()

    def refresh_questions_list(self):
        """Refresh the questions listbox - keep it super fresh! 💫"""
        self.questions_listbox.delete(0, tk.END)
        for i, question in enumerate(self.questions):
            qtype = question.get('type', 'unknown')
            if qtype == 'multi_questions':
                sub_count = len(question.get('questions', []))
                qtext = f"Multi-Block ({sub_count} questions)"
            else:
                qtext = question.get('question', 'No question text')
            if len(qtext) > 35:
                qtext = qtext[:35] + "... 💖"
            self.questions_listbox.insert(tk.END, f"{i+1}. [{qtype}] {qtext}")

    def save_and_launch(self):
        if not self.questions:
            messagebox.showwarning("Warning! 💕", "No questions to save, sweetie!")
            return
        if not self.current_file:
            self.save_json_as()
            if not self.current_file:
                return
        else:
            self.save_json()
        try:
            cmd = [self.wifeymooc_app_path, "-q", self.current_file]
            subprocess.Popen(cmd)
            messagebox.showinfo("Launched! 💖", f"Launched WifeyMOOC with\n{self.current_file}")
        except Exception as e:
            messagebox.showerror("Oopsie! 😢", f"Failed to launch app, babe:\n{str(e)}")

    def on_question_select(self, event):
        """Handle question selection from list - ultra-responsive! 💕"""
        selection = self.questions_listbox.curselection()
        if selection:
            index = selection[0]
            self.current_question_index = index
            self.edit_question(index)

    def show_add_question_dialog(self):
        """Show the most adorable dialog to add new questions! ✨"""
        dialog = tk.Toplevel(self.root)
        dialog.title("✨ Add New Adorable Question ✨")
        dialog.geometry("500x700")
        dialog.configure(bg=self.colors['bg'])
        dialog.transient(self.root)
        dialog.grab_set()
        dialog.geometry("+%d+%d" % (self.root.winfo_rootx() + 50, self.root.winfo_rooty() + 50))
        tk.Label(dialog, text="💖 Choose Your Question Type, Princess! 💖",
                font=('Comic Sans MS', 16, 'bold'),
                bg=self.colors['bg'], fg=self.colors['text']).pack(pady=10)
        button_frame = tk.Frame(dialog, bg=self.colors['bg'])
        button_frame.pack(fill=tk.BOTH, expand=True, padx=20, pady=10)
        question_types = [
            ('📝 List Pick (Select Multiple)', 'list_pick'),
            ('🔘 Single Choice MCQ (Enhanced!)', 'mcq_single'),
            ('☑️ Multiple Choice MCQ (Enhanced!)', 'mcq_multiple'),
            ('📝 Word Fill (Fill Blanks)', 'word_fill'),
            ('🔗 Match Sentences to Images', 'match_sentence'),
            ('🎵 Audio Sequence Order', 'sequence_audio'),
            ('📋 Order Phrases Correctly', 'order_phrase'),
            ('📂 Categorization Multiple', 'categorization_multiple'),
            ('⬇️ Fill Blanks with Dropdowns', 'fill_blanks_dropdown'),
            ('🔗 Match Phrase Beginnings to Endings', 'match_phrases'),
            ('🎭 Multi-Questions Block (Super Cool!)', 'multi_questions'),
            ('🏷️ Image Tagging (Drag & Drop!)', 'image_tagging')]
        for i, (display_name, qtype) in enumerate(question_types):
            color = self.colors['accent'] if qtype in ['multi_questions', 'image_tagging'] else self.colors['button']
            btn = tk.Button(button_frame, text=display_name,
                           command=lambda t=qtype: self.add_question(t, dialog),
                           bg=color, fg=self.colors['button_text'],
                           font=('Arial', 10, 'bold'), width=45)
            btn.pack(pady=4, padx=5, fill=tk.X)
        tk.Button(button_frame, text="❌ Cancel", command=dialog.destroy,
                 bg='#FF6347', fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(pady=15)

    def add_question(self, question_type, dialog):
        """Add a new question of specified type - ultra-exciting! 💖"""
        if question_type in self.question_templates:
            new_question = self.question_templates[question_type].copy()
            if 'pairs' in new_question: new_question['pairs'] = [p.copy() for p in new_question['pairs']]
            if 'stimuli' in new_question: new_question['stimuli'] = [s.copy() for s in new_question['stimuli']]
            if 'audio_options' in new_question: new_question['audio_options'] = [o.copy() for o in new_question['audio_options']]
            if 'options_for_blanks' in new_question: new_question['options_for_blanks'] = [o.copy() for o in new_question['options_for_blanks']]
            if 'questions' in new_question:
                new_question['questions'] = []
                for subq in self.question_templates[question_type]['questions']:
                    subq_copy = subq.copy()
                    if 'options' in subq_copy:
                        subq_copy['options'] = [opt.copy() if isinstance(opt, dict) else opt for opt in subq_copy['options']]
                    new_question['questions'].append(subq_copy)
            if 'tags' in new_question:
                new_question['tags'] = [t.copy() for t in new_question['tags']]
                new_question['answer'] = new_question['answer'].copy()
                new_question['alternatives'] = []
            self.questions.append(new_question)
            self.refresh_questions_list()
            new_index = len(self.questions) - 1
            self.questions_listbox.selection_set(new_index)
            self.current_question_index = new_index
            self.edit_question(new_index)
            dialog.destroy()
            messagebox.showinfo("Success! 💖", f"Added new {question_type.replace('_', ' ')} question, babe!")

    def delete_question(self):
        """Delete selected question - sometimes we need to let go 💔"""
        selection = self.questions_listbox.curselection()
        if not selection:
            messagebox.showwarning("Warning! 💕", "Please select a question to delete, sweetie!")
            return
        index = selection[0]
        question = self.questions[index]
        qtext = f"Multi-questions block with {len(question.get('questions', []))} sub-questions" if question.get('type') == 'multi_questions' else question.get('question', 'No question text')[:50]
        if messagebox.askyesno("Confirm Delete 🗑️", f"Are you sure you want to delete this question, honey?\n\n{qtext}..."):
            del self.questions[index]
            self.refresh_questions_list()
            self.show_welcome_message()
            messagebox.showinfo("Deleted! 💔", "Question deleted successfully, darling!")

    def move_question_up(self):
        """Move selected question up in the list - rearrange the cuteness! ⬆️"""
        selection = self.questions_listbox.curselection()
        if not selection or selection[0] == 0: return
        index = selection[0]
        self.questions[index], self.questions[index-1] = self.questions[index-1], self.questions[index]
        self.refresh_questions_list()
        self.questions_listbox.selection_set(index-1)

    def move_question_down(self):
        """Move selected question down in the list - rearrange the cuteness! ⬇️"""
        selection = self.questions_listbox.curselection()
        if not selection or selection[0] == len(self.questions) - 1: return
        index = selection[0]
        self.questions[index], self.questions[index+1] = self.questions[index+1], self.questions[index]
        self.refresh_questions_list()
        self.questions_listbox.selection_set(index+1)

    def browse_media_file(self, var, media_type):
        """Browse for media file - find the perfect files! 📁"""
        filetypes = {
            'video': [("Video files", "*.mp4 *.avi *.mov *.wmv *.flv *.mkv"), ("All files", "*.*")],
            'audio': [("Audio files", "*.mp3 *.wav *.ogg *.m4a *.aac *.flac"), ("All files", "*.*")],
            'image': [("Image files", "*.jpg *.jpeg *.png *.gif *.bmp *.svg *.webp"), ("All files", "*.*")]}
        file_path = filedialog.askopenfilename(
            title=f"💖 Select Your Perfect {media_type.title()} File 💖",
            filetypes=filetypes.get(media_type, [("All files", "*.*")]))
        if file_path:
            var.set(file_path)

    def create_media_section(self, question, index):
        """Create the gorgeous media management section! 🎬"""
        media_frame = tk.LabelFrame(self.editor_frame, text="🎬 Media Files (Video, Audio, Images) 🎬",
                                   font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        media_frame.pack(fill=tk.X, padx=10, pady=10)
        media = question.get('media', {}) or {}
        video_frame = tk.Frame(media_frame, bg=self.colors['white'])
        video_frame.pack(fill=tk.X, padx=5, pady=2)
        tk.Label(video_frame, text="📹 Video File:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
        video_var = tk.StringVar(value=media.get('video', ''))
        tk.Entry(video_frame, textvariable=video_var, width=50, bg=self.colors['entry']).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
        tk.Button(video_frame, text="Browse 📁", bg=self.colors['button'], fg=self.colors['button_text'],
                 command=lambda: self.browse_media_file(video_var, "video")).pack(side=tk.RIGHT, padx=5)
        audio_frame = tk.Frame(media_frame, bg=self.colors['white'])
        audio_frame.pack(fill=tk.X, padx=5, pady=2)
        tk.Label(audio_frame, text="🎵 Audio File:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
        audio_var = tk.StringVar(value=media.get('audio', ''))
        tk.Entry(audio_frame, textvariable=audio_var, width=50, bg=self.colors['entry']).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
        tk.Button(audio_frame, text="Browse 📁", bg=self.colors['button'], fg=self.colors['button_text'],
                 command=lambda: self.browse_media_file(audio_var, "audio")).pack(side=tk.RIGHT, padx=5)
        image_frame = tk.Frame(media_frame, bg=self.colors['white'])
        image_frame.pack(fill=tk.X, padx=5, pady=2)
        tk.Label(image_frame, text="🖼️ Image File:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
        image_var = tk.StringVar(value=media.get('image', ''))
        tk.Entry(image_frame, textvariable=image_var, width=50, bg=self.colors['entry']).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
        tk.Button(image_frame, text="Browse 📁", bg=self.colors['button'], fg=self.colors['button_text'],
                 command=lambda: self.browse_media_file(image_var, "image")).pack(side=tk.RIGHT, padx=5)
        def save_media():
            new_media = {}
            if video_var.get().strip(): new_media['video'] = video_var.get().strip()
            if audio_var.get().strip(): new_media['audio'] = audio_var.get().strip()
            if image_var.get().strip(): new_media['image'] = image_var.get().strip()
            question['media'] = new_media if new_media else None
            messagebox.showinfo("Success! 💖", "Media files updated successfully, babe!")
        tk.Button(media_frame, text="💾 Save Media Files", command=save_media,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(pady=8)

    def create_question_text_section(self, question, index, label_text="📝 Question Text & Hint 📝"):
        """Create the question text and hint editing section! 💖"""
        text_frame = tk.LabelFrame(self.editor_frame, text=label_text,
                                font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        text_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(text_frame, text="💡 Write your question here, sweetie! You can use Enter for newlines! ✨",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        question_text_widget = tk.Text(text_frame, height=4, font=('Arial', 11),
                            bg=self.colors['entry'], width=80, wrap=tk.WORD)
        question_text_widget.pack(padx=10, pady=(0, 10), fill=tk.X)
        question_text_widget.insert('1.0', question.get('question', ''))
        tk.Label(text_frame, text="💡 Add a cute little hint for Sierra here (optional)! ✨",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        hint_text_widget = tk.Text(text_frame, height=3, font=('Arial', 11),
                                 bg=self.colors['entry'], width=80, wrap=tk.WORD)
        hint_text_widget.pack(padx=10, pady=(0, 10), fill=tk.X)
        hint_text_widget.insert('1.0', question.get('hint', ''))
        def save_question_and_hint():
            question['question'] = question_text_widget.get('1.0', tk.END).strip()
            hint_text = hint_text_widget.get('1.0', tk.END).strip()
            if hint_text:
                question['hint'] = hint_text
            else:
                question.pop('hint', None)
            self.refresh_questions_list()
            messagebox.showinfo("Success! 💖", "Question and hint updated beautifully!")
        tk.Button(text_frame, text="💾 Save Text & Hint", command=save_question_and_hint,
                bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(pady=5)

    def edit_question(self, index):
        """Edit question at specified index - the main magic! ✨"""
        if index < 0 or index >= len(self.questions): return
        question = self.questions[index]
        qtype = question.get('type')
        for widget in self.right_frame.winfo_children(): widget.destroy()
        canvas = tk.Canvas(self.right_frame, bg=self.colors['white'])
        scrollbar = ttk.Scrollbar(self.right_frame, orient="vertical", command=canvas.yview)
        self.editor_frame = tk.Frame(canvas, bg=self.colors['white'])
        canvas.configure(yscrollcommand=scrollbar.set)
        canvas.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")
        canvas_frame = canvas.create_window((0, 0), window=self.editor_frame, anchor="nw")
        def on_frame_configure(event): canvas.configure(scrollregion=canvas.bbox("all"))
        def on_canvas_configure(event): canvas.itemconfig(canvas_frame, width=event.width)
        self.editor_frame.bind("<Configure>", on_frame_configure)
        canvas.bind("<Configure>", on_canvas_configure)
        def on_mousewheel(event): canvas.yview_scroll(int(-1*(event.delta/120)), "units")
        canvas.bind_all("<MouseWheel>", on_mousewheel)
        title_text = f"💖 Editing Question {index + 1}: {qtype.replace('_', ' ').title()} 💖"
        tk.Label(self.editor_frame, text=title_text, font=('Comic Sans MS', 18, 'bold'),
                bg=self.colors['white'], fg=self.colors['text']).pack(pady=10)
        self.create_lesson_section(question) # ✨ ADDED: Call our new lesson PDF editor! ✨
        editor_method = getattr(self, f'edit_{qtype}', None)
        if editor_method:
            editor_method(question, index)
        else:
            tk.Label(self.editor_frame, text=f"❌ Oopsie! Unsupported question type: {qtype}",
                    font=('Arial', 12), bg=self.colors['white'], fg='red').pack(pady=20)
            
    def create_lesson_section(self, question):
        """✨ ADDED: A new function to create the adorable lesson PDF editor section! ✨"""
        lesson_frame = tk.LabelFrame(self.editor_frame, text="📚 Lesson PDF (Optional) 📚",
                                     font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        lesson_frame.pack(fill=tk.X, padx=10, pady=10)

        lesson_obj = question.get('lesson', {}) or {}
        lesson_pdf_var = tk.StringVar(value=lesson_obj.get('pdf', ''))

        lesson_row = tk.Frame(lesson_frame, bg=self.colors['white'])
        lesson_row.pack(fill=tk.X, padx=5, pady=5)
        tk.Label(lesson_row, text="PDF File:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
        tk.Entry(lesson_row, textvariable=lesson_pdf_var, width=50, bg=self.colors['entry']).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
        tk.Button(lesson_row, text="Browse 📁", bg=self.colors['button'], fg=self.colors['button_text'],
                  command=lambda: self.browse_media_file(lesson_pdf_var, 'pdf')).pack(side=tk.RIGHT, padx=5)

        def save_lesson():
            pdf_path = lesson_pdf_var.get().strip()
            if pdf_path:
                question['lesson'] = {'pdf': pdf_path}
            else:
                question.pop('lesson', None) # Remove the key if the path is empty
            messagebox.showinfo("Success! 💖", "Lesson PDF path updated successfully, darling!")

        tk.Button(lesson_frame, text="💾 Save Lesson PDF", command=save_lesson,
                  bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(pady=8)

    def edit_image_tagging(self, question, index):
        """Enhanced editor for image_tagging questions - now with proper alternative support! 🏷️"""
        self.create_question_text_section(question, index)
        main_frame = tk.LabelFrame(self.editor_frame, text="🖼️ Main Image Configuration 🖼️",
                                  font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        main_frame.pack(fill=tk.X, padx=10, pady=10)
        media = question.get('media', {}) or {}
        main_image_frame = tk.Frame(main_frame, bg=self.colors['white'])
        main_image_frame.pack(fill=tk.X, padx=5, pady=5)
        tk.Label(main_image_frame, text="🖼️ Main Image:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
        main_image_var = tk.StringVar(value=media.get('image', ''))
        tk.Entry(main_image_frame, textvariable=main_image_var, width=50, bg=self.colors['entry']).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
        tk.Button(main_image_frame, text="Browse 📁", bg=self.colors['button'], fg=self.colors['button_text'],
                 command=lambda: self.browse_media_file(main_image_var, "image")).pack(side=tk.RIGHT, padx=5)
        button_frame = tk.Frame(main_frame, bg=self.colors['white'])
        button_frame.pack(fill=tk.X, padx=5, pady=5)
        tk.Label(button_frame, text="🔘 Main Button Label:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
        button_label_var = tk.StringVar(value=question.get('button_label', 'Alternative View'))
        tk.Entry(button_frame, textvariable=button_label_var, width=30, bg=self.colors['entry']).pack(side=tk.LEFT, padx=5)
        def save_main_config():
            question['media'] = {'image': main_image_var.get().strip()} if main_image_var.get().strip() else {}
            question['button_label'] = button_label_var.get().strip()
            messagebox.showinfo("Success! 💖", "Main image configuration saved!")
        tk.Button(main_frame, text="💾 Save Main Config", command=save_main_config,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(pady=5)
        tags_frame = tk.LabelFrame(self.editor_frame, text="🏷️ Draggable Tags Configuration 🏷️",
                                  font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        tags_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(tags_frame, text="💡 Configure tags and their coordinates for the main image!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        tags_list_frame = tk.Frame(tags_frame, bg=self.colors['white'])
        tags_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.image_tagging_tags_data = []
        def refresh_main_tags():
            for widget in tags_list_frame.winfo_children(): widget.destroy()
            self.image_tagging_tags_data.clear()
            for i, tag in enumerate(question.get('tags', [])):
                row_frame = tk.Frame(tags_list_frame, bg=self.colors['white'])
                row_frame.pack(fill=tk.X, pady=2)
                tk.Label(row_frame, text=f"🏷️ {i+1} ID:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                id_var = tk.StringVar(value=tag.get('id', ''))
                tk.Entry(row_frame, textvariable=id_var, bg=self.colors['entry'], width=15, font=('Arial', 10)).pack(side=tk.LEFT, padx=2)
                tk.Label(row_frame, text="📝 Label:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=(10,2))
                label_var = tk.StringVar(value=tag.get('label', ''))
                tk.Entry(row_frame, textvariable=label_var, bg=self.colors['entry'], width=25, font=('Arial', 10)).pack(side=tk.LEFT, padx=2)
                tk.Label(row_frame, text="📍 Main X:", bg=self.colors['white']).pack(side=tk.LEFT, padx=(10,2))
                coords = question.get('answer', {}).get(tag.get('id', ''), [0, 0])
                x_var = tk.StringVar(value=str(coords[0] if len(coords) > 0 else 0))
                tk.Entry(row_frame, textvariable=x_var, bg=self.colors['entry'], width=8, font=('Arial', 10)).pack(side=tk.LEFT, padx=2)
                tk.Label(row_frame, text="Y:", bg=self.colors['white']).pack(side=tk.LEFT, padx=(5,2))
                y_var = tk.StringVar(value=str(coords[1] if len(coords) > 1 else 0))
                tk.Entry(row_frame, textvariable=y_var, bg=self.colors['entry'], width=8, font=('Arial', 10)).pack(side=tk.LEFT, padx=2)
                if len(question.get('tags', [])) > 1:
                    tk.Button(row_frame, text="🗑️", bg='#FF6347', fg=self.colors['button_text'],
                             command=lambda idx=i: delete_main_tag(idx)).pack(side=tk.RIGHT, padx=5)
                self.image_tagging_tags_data.append((id_var, label_var, x_var, y_var))
        def add_main_tag():
            new_tag = {'id': f'tag{len(question.get("tags", [])) + 1}', 'label': 'New Tag'}
            question.setdefault('tags', []).append(new_tag)
            question.setdefault('answer', {})[new_tag['id']] = [100, 100]
            refresh_main_tags()
        def delete_main_tag(idx):
            if len(question.get('tags', [])) > 1:
                tag_id = question['tags'][idx].get('id')
                del question['tags'][idx]
                if tag_id in question.get('answer', {}): del question['answer'][tag_id]
                refresh_main_tags()
        def save_main_tags():
            tags, answer = [], {}
            for id_var, label_var, x_var, y_var in self.image_tagging_tags_data:
                tag_id = id_var.get().strip()
                if tag_id:
                    tags.append({'id': tag_id, 'label': label_var.get().strip()})
                    try: answer[tag_id] = [float(x_var.get()), float(y_var.get())]
                    except ValueError: answer[tag_id] = [0, 0]
            question['tags'], question['answer'] = tags, answer
            messagebox.showinfo("Success! 💖", "Main tags saved beautifully!")
        refresh_main_tags()
        tags_btn_frame = tk.Frame(tags_frame, bg=self.colors['white'])
        tags_btn_frame.pack(fill=tk.X, pady=5)
        tk.Button(tags_btn_frame, text="➕ Add Tag", command=add_main_tag,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=2)
        tk.Button(tags_btn_frame, text="💾 Save Main Tags", command=save_main_tags,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=2)
        alt_frame = tk.LabelFrame(self.editor_frame, text="🔄 Alternative Images (Enhanced!) 🔄",
                                 font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        alt_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(alt_frame, text="💡 Different images with same tags but different coordinates! Now fully editable!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        alt_management_frame = tk.Frame(alt_frame, bg=self.colors['white'])
        alt_management_frame.pack(fill=tk.X, padx=5, pady=5)
        tk.Label(alt_management_frame, text="🔢 Number of alternatives:",
                bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
        alt_count_var = tk.StringVar(value=str(len(question.get('alternatives', []))))
        tk.Entry(alt_management_frame, textvariable=alt_count_var, width=5, bg=self.colors['entry']).pack(side=tk.LEFT, padx=5)
        def update_alternatives_structure():
            try:
                count = int(alt_count_var.get())
                current_alts = question.get('alternatives', [])
                while len(current_alts) < count:
                    current_alts.append({
                        'media': {'image': 'alternative.jpg'},
                        'button_label': f'Alt View {len(current_alts) + 1}',
                        'tags': [t.copy() for t in question.get('tags', [])],
                        'answer': {t.get('id', ''): [150, 150] for t in question.get('tags', [])}})
                while len(current_alts) > count: current_alts.pop()
                question['alternatives'] = current_alts
                refresh_alternatives_editor()
                messagebox.showinfo("Success! 💖", f"Updated to {count} alternatives!")
            except ValueError: messagebox.showerror("Error! 😢", "Please enter a valid number!")
        tk.Button(alt_management_frame, text="🔄 Update Count", command=update_alternatives_structure,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)
        alt_details_frame = tk.Frame(alt_frame, bg=self.colors['white'])
        alt_details_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=10)
        def refresh_alternatives_editor():
            for widget in alt_details_frame.winfo_children(): widget.destroy()
            for alt_idx, alt in enumerate(question.get('alternatives', [])):
                alt_container = tk.LabelFrame(alt_details_frame, text=f"🌟 Alternative {alt_idx + 1} 🌟",
                                             bg=self.colors['secondary'], font=('Arial', 11, 'bold'))
                alt_container.pack(fill=tk.X, pady=5, padx=2)
                alt_img_frame = tk.Frame(alt_container, bg=self.colors['secondary'])
                alt_img_frame.pack(fill=tk.X, padx=5, pady=3)
                tk.Label(alt_img_frame, text="🖼️ Image:", bg=self.colors['secondary'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                alt_img_var = tk.StringVar(value=alt.get('media', {}).get('image', ''))
                tk.Entry(alt_img_frame, textvariable=alt_img_var, bg=self.colors['entry'], width=40, font=('Arial', 10)).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                tk.Button(alt_img_frame, text="Browse", bg=self.colors['button'], fg=self.colors['button_text'],
                         command=lambda v=alt_img_var: self.browse_media_file(v, "image")).pack(side=tk.RIGHT, padx=2)
                alt_btn_frame = tk.Frame(alt_container, bg=self.colors['secondary'])
                alt_btn_frame.pack(fill=tk.X, padx=5, pady=3)
                tk.Label(alt_btn_frame, text="🔘 Button Label:", bg=self.colors['secondary'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                alt_btn_var = tk.StringVar(value=alt.get('button_label', f'Alt View {alt_idx + 1}'))
                tk.Entry(alt_btn_frame, textvariable=alt_btn_var, bg=self.colors['entry'], width=25, font=('Arial', 10)).pack(side=tk.LEFT, padx=5)
                tags_coord_frame = tk.LabelFrame(alt_container, text="🏷️ Tag Coordinates for This Alternative",
                                                bg=self.colors['secondary'], font=('Arial', 10, 'bold'))
                tags_coord_frame.pack(fill=tk.X, padx=5, pady=5)
                alt_coords_data = []
                for tag in question.get('tags', []):
                    tag_id, tag_label = tag.get('id', ''), tag.get('label', '')
                    coord_row = tk.Frame(tags_coord_frame, bg=self.colors['secondary'])
                    coord_row.pack(fill=tk.X, pady=1)
                    tk.Label(coord_row, text=f"{tag_label}:", bg=self.colors['secondary'], width=15, anchor='w').pack(side=tk.LEFT)
                    alt_coords = alt.get('answer', {}).get(tag_id, [0, 0])
                    tk.Label(coord_row, text="X:", bg=self.colors['secondary']).pack(side=tk.LEFT, padx=(5,2))
                    alt_x_var = tk.StringVar(value=str(alt_coords[0] if len(alt_coords) > 0 else 0))
                    tk.Entry(coord_row, textvariable=alt_x_var, bg=self.colors['entry'], width=8, font=('Arial', 9)).pack(side=tk.LEFT, padx=2)
                    tk.Label(coord_row, text="Y:", bg=self.colors['secondary']).pack(side=tk.LEFT, padx=(5,2))
                    alt_y_var = tk.StringVar(value=str(alt_coords[1] if len(alt_coords) > 1 else 0))
                    tk.Entry(coord_row, textvariable=alt_y_var, bg=self.colors['entry'], width=8, font=('Arial', 9)).pack(side=tk.LEFT, padx=2)
                    alt_coords_data.append((tag_id, alt_x_var, alt_y_var))
                def save_this_alternative(idx=alt_idx, img_var=alt_img_var, btn_var=alt_btn_var, coords_data=alt_coords_data):
                    if idx < len(question.get('alternatives', [])):
                        alt = question['alternatives'][idx]
                        alt['media'] = {'image': img_var.get().strip()}
                        alt['button_label'] = btn_var.get().strip()
                        alt_answer = {}
                        for tag_id, x_var, y_var in coords_data:
                            try: alt_answer[tag_id] = [float(x_var.get()), float(y_var.get())]
                            except ValueError: alt_answer[tag_id] = [0, 0]
                        alt['answer'] = alt_answer
                        messagebox.showinfo("Success! 💖", f"Alternative {idx + 1} saved perfectly!")
                tk.Button(alt_container, text=f"💾 Save Alternative {alt_idx + 1}", command=save_this_alternative,
                         bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(pady=5)
        refresh_alternatives_editor()
        def save_everything():
            save_main_config()
            save_main_tags()
            messagebox.showinfo("Success! 💖", "Complete image tagging configuration saved beautifully!")
        tk.Button(alt_frame, text="💖 Save Everything", command=save_everything,
                 bg=self.colors['success'], fg=self.colors['button_text'], font=('Arial', 12, 'bold')).pack(pady=10)

    def edit_multi_questions(self, question, index):
        """Enhanced editor for multi_questions - works perfectly with image tagging too! 🎭"""
        subq_frame = tk.LabelFrame(self.editor_frame, text="🎭 Multi-Questions Block (Super Enhanced!) 🎭",
                                  font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        subq_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(subq_frame, text="💡 Create multiple sub-questions! Each can be any question type including image tagging!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        subq_list_frame = tk.Frame(subq_frame, bg=self.colors['white'])
        subq_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.multi_questions_data = []
        def refresh_subquestions():
            for widget in subq_list_frame.winfo_children(): widget.destroy()
            self.multi_questions_data.clear()
            for i, subq in enumerate(question.get('questions', [])):
                subq_container = tk.LabelFrame(subq_list_frame, text=f"💖 Sub-Question {i+1}: {subq.get('type', 'unknown')} 💖",
                                              bg=self.colors['secondary'], font=('Arial', 11, 'bold'))
                subq_container.pack(fill=tk.X, pady=5, padx=2)
                type_frame = tk.Frame(subq_container, bg=self.colors['secondary'])
                type_frame.pack(fill=tk.X, padx=5, pady=3)
                tk.Label(type_frame, text="🎭 Type:", bg=self.colors['secondary'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                available_types = ['mcq_single', 'mcq_multiple', 'word_fill', 'list_pick',
                                 'match_sentence', 'sequence_audio', 'order_phrase',
                                 'categorization_multiple', 'fill_blanks_dropdown', 'match_phrases', 'image_tagging']
                type_var = tk.StringVar(value=subq.get('type', 'mcq_single'))
                ttk.Combobox(type_frame, textvariable=type_var, values=available_types, state='readonly', width=20).pack(side=tk.LEFT, padx=5)
                text_frame = tk.Frame(subq_container, bg=self.colors['secondary'])
                text_frame.pack(fill=tk.X, padx=5, pady=3)
                tk.Label(text_frame, text="📝 Question:", bg=self.colors['secondary'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                text_var = tk.StringVar(value=subq.get('question', ''))
                tk.Entry(text_frame, textvariable=text_var, bg=self.colors['entry'], width=70, font=('Arial', 10)).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                preview_frame = tk.Frame(subq_container, bg=self.colors['secondary'])
                preview_frame.pack(fill=tk.X, padx=5, pady=5)
                preview_text = tk.Text(preview_frame, height=3, width=80, bg=self.colors['entry'], font=('Arial', 8))
                preview_text.pack(padx=5, pady=2)
                preview_content = f"Type: {subq.get('type', 'unknown')}\n"
                if subq.get('type') == 'image_tagging':
                    preview_content += f"Main Image: {subq.get('media', {}).get('image', 'Not set')}\n"
                    preview_content += f"Tags: {len(subq.get('tags', []))} tags, {len(subq.get('alternatives', []))} alternatives"
                elif subq.get('type') in ['mcq_single', 'mcq_multiple']:
                    preview_content += f"Options: {len(subq.get('options', []))} options\nAnswers: {subq.get('answer', [])}"
                else:
                    preview_content += f"Answer: {str(subq.get('answer', 'Not set'))[:50]}"
                preview_text.insert('1.0', preview_content)
                preview_text.config(state='disabled')
                self.multi_questions_data.append({'type_var': type_var, 'text_var': text_var, 'preview_text': preview_text, 'subq_index': i})
                subq_btn_frame = tk.Frame(subq_container, bg=self.colors['secondary'])
                subq_btn_frame.pack(fill=tk.X, pady=5)
                tk.Button(subq_btn_frame, text="🔧 Edit Details", bg=self.colors['button'], fg=self.colors['button_text'],
                         command=lambda idx=i: edit_subquestion_details(idx), font=('Arial', 9, 'bold')).pack(side=tk.LEFT, padx=2)
                if len(question.get('questions', [])) > 1:
                    tk.Button(subq_btn_frame, text="🗑️ Delete", bg='#FF6347', fg=self.colors['button_text'],
                             command=lambda idx=i: delete_subquestion(idx), font=('Arial', 9, 'bold')).pack(side=tk.LEFT, padx=2)
        def add_subquestion():
            question.setdefault('questions', []).append({
                'type': 'mcq_single', 'question': 'New cute sub-question! 💖',
                'options': [{'image': 'image1.jpg', 'text': 'Option A'}, {'image': 'image2.jpg', 'text': 'Option B'}],
                'answer': [0]})
            refresh_subquestions()
        def delete_subquestion(idx):
            if len(question.get('questions', [])) > 1:
                del question['questions'][idx]
                refresh_subquestions()
        def edit_subquestion_details(idx):
            if idx >= len(question.get('questions', [])): return
            subq = question['questions'][idx]
            detail_dialog = tk.Toplevel(self.root)
            detail_dialog.title(f"✨ Edit Sub-Question {idx+1} ({subq.get('type', 'unknown')}) ✨")
            detail_dialog.geometry("1000x800")
            detail_dialog.configure(bg=self.colors['bg'])
            detail_dialog.transient(self.root)
            detail_dialog.grab_set()
            canvas = tk.Canvas(detail_dialog, bg=self.colors['white'])
            scrollbar = ttk.Scrollbar(detail_dialog, orient="vertical", command=canvas.yview)
            detail_frame = tk.Frame(canvas, bg=self.colors['white'])
            canvas.configure(yscrollcommand=scrollbar.set)
            canvas.pack(side="left", fill="both", expand=True, padx=10, pady=10)
            scrollbar.pack(side="right", fill="y")
            canvas_frame = canvas.create_window((0, 0), window=detail_frame, anchor="nw")
            def on_frame_configure(event): canvas.configure(scrollregion=canvas.bbox("all"))
            def on_canvas_configure(event): canvas.itemconfig(canvas_frame, width=event.width)
            detail_frame.bind("<Configure>", on_frame_configure)
            canvas.bind("<Configure>", on_canvas_configure)
            self.editor_frame = detail_frame
            qtype = subq.get('type')
            editor_method = getattr(self, f'edit_{qtype}', None)
            if editor_method: editor_method(subq, idx)
            tk.Button(detail_dialog, text="💖 Close & Refresh", command=lambda: [detail_dialog.destroy(), refresh_subquestions()],
                     bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 12, 'bold')).pack(pady=10)
        def save_all_subquestions():
            for data in self.multi_questions_data:
                idx = data['subq_index']
                if idx < len(question.get('questions', [])):
                    subq = question['questions'][idx]
                    subq['type'] = data['type_var'].get()
                    subq['question'] = data['text_var'].get()
            messagebox.showinfo("Success! 💖", "Multi-questions block saved beautifully!")
            refresh_subquestions()
        refresh_subquestions()
        btn_frame = tk.Frame(subq_frame, bg=self.colors['white'])
        btn_frame.pack(fill=tk.X, pady=8)
        tk.Button(btn_frame, text="➕ Add Sub-Question", command=add_subquestion,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)
        tk.Button(btn_frame, text="💾 Save All Sub-Questions", command=save_all_subquestions,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

    def edit_mcq_single(self, question, index):
        """Enhanced editor for mcq_single - supports text, image, or both! 💖"""
        self.create_question_text_section(question, index)
        self.create_media_section(question, index)
        options_frame = tk.LabelFrame(self.editor_frame, text="🔘 Enhanced Answer Options (Text + Image Support!) 🔘",
                                     font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        options_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(options_frame, text="💡 Options can be text-only, image-only, or text+image! Select the correct one!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        options_list_frame = tk.Frame(options_frame, bg=self.colors['white'])
        options_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.mcq_single_option_vars = []
        self.mcq_single_correct_var = tk.IntVar()
        def refresh_options():
            for widget in options_list_frame.winfo_children(): widget.destroy()
            self.mcq_single_option_vars.clear()
            correct_answer = question.get('answer', [0])[0] if question.get('answer') else 0
            self.mcq_single_correct_var.set(correct_answer)
            for i, option in enumerate(question.get('options', [])):
                opt_container = tk.LabelFrame(options_list_frame, text=f"💖 Option {i+1} 💖",
                                             bg=self.colors['secondary'], font=('Arial', 10, 'bold'))
                opt_container.pack(fill=tk.X, pady=3, padx=2)
                tk.Radiobutton(opt_container, text="✅ This is Correct!", variable=self.mcq_single_correct_var, value=i,
                              bg=self.colors['secondary'], font=('Arial', 9, 'bold')).pack(pady=2)
                if isinstance(option, dict):
                    img_frame = tk.Frame(opt_container, bg=self.colors['secondary'])
                    img_frame.pack(fill=tk.X, padx=5, pady=2)
                    tk.Label(img_frame, text="🖼️ Image:", bg=self.colors['secondary']).pack(side=tk.LEFT)
                    img_var = tk.StringVar(value=option.get('image', ''))
                    tk.Entry(img_frame, textvariable=img_var, bg=self.colors['entry'], width=40).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                    tk.Button(img_frame, text="Browse", bg=self.colors['button'], fg=self.colors['button_text'],
                             command=lambda v=img_var: self.browse_media_file(v, "image")).pack(side=tk.RIGHT, padx=2)
                    text_frame = tk.Frame(opt_container, bg=self.colors['secondary'])
                    text_frame.pack(fill=tk.X, padx=5, pady=2)
                    tk.Label(text_frame, text="📝 Text:", bg=self.colors['secondary']).pack(side=tk.LEFT)
                    text_var = tk.StringVar(value=option.get('text', ''))
                    tk.Entry(text_frame, textvariable=text_var, bg=self.colors['entry'], width=50).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                    self.mcq_single_option_vars.append((img_var, text_var))
                else:
                    text_frame = tk.Frame(opt_container, bg=self.colors['secondary'])
                    text_frame.pack(fill=tk.X, padx=5, pady=2)
                    tk.Label(text_frame, text="📝 Text:", bg=self.colors['secondary']).pack(side=tk.LEFT)
                    text_var = tk.StringVar(value=str(option))
                    tk.Entry(text_frame, textvariable=text_var, bg=self.colors['entry'], width=50).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                    self.mcq_single_option_vars.append((tk.StringVar(), text_var))
                if len(question.get('options', [])) > 2:
                    tk.Button(opt_container, text="🗑️ Delete Option", bg='#FF6347', fg=self.colors['button_text'],
                             command=lambda idx=i: delete_option(idx)).pack(pady=5)
        def add_option():
            question.setdefault('options', []).append({'image': '', 'text': 'New Amazing Option'})
            refresh_options()
        def delete_option(idx):
            if len(question.get('options', [])) > 2:
                del question['options'][idx]
                correct = self.mcq_single_correct_var.get()
                if correct == idx: self.mcq_single_correct_var.set(0)
                elif correct > idx: self.mcq_single_correct_var.set(correct - 1)
                refresh_options()
        def save_options():
            options = []
            for img_var, text_var in self.mcq_single_option_vars:
                img, text = img_var.get().strip(), text_var.get().strip()
                if img and text: options.append({'image': img, 'text': text})
                elif img: options.append({'image': img, 'text': ''})
                elif text: options.append({'image': '', 'text': text})
                else: options.append({'image': '', 'text': 'Empty Option'})
            question['options'] = options
            question['answer'] = [self.mcq_single_correct_var.get()]
            messagebox.showinfo("Success! 💖", "Enhanced MCQ options saved perfectly!")
        refresh_options()
        btn_frame = tk.Frame(options_frame, bg=self.colors['white'])
        btn_frame.pack(fill=tk.X, pady=8)
        tk.Button(btn_frame, text="➕ Add Option", command=add_option,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)
        tk.Button(btn_frame, text="💾 Save All Options", command=save_options,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

    def edit_mcq_multiple(self, question, index):
        """Enhanced editor for mcq_multiple - supports text, image, or both! 💖"""
        self.create_question_text_section(question, index)
        self.create_media_section(question, index)
        options_frame = tk.LabelFrame(self.editor_frame, text="☑️ Enhanced Multiple Choice (Text + Image Support!) ☑️",
                                     font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        options_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(options_frame, text="💡 Check all correct options! Mix text and images as you like!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        options_list_frame = tk.Frame(options_frame, bg=self.colors['white'])
        options_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.mcq_multiple_option_vars = []
        self.mcq_multiple_correct_vars = []
        def refresh_options():
            for widget in options_list_frame.winfo_children(): widget.destroy()
            self.mcq_multiple_option_vars.clear()
            self.mcq_multiple_correct_vars.clear()
            for i, option in enumerate(question.get('options', [])):
                opt_container = tk.LabelFrame(options_list_frame, text=f"💖 Option {i+1} 💖",
                                             bg=self.colors['secondary'], font=('Arial', 10, 'bold'))
                opt_container.pack(fill=tk.X, pady=3, padx=2)
                correct_var = tk.BooleanVar(value=i in question.get('answer', []))
                tk.Checkbutton(opt_container, text="✅ This is Correct!", variable=correct_var,
                              bg=self.colors['secondary'], font=('Arial', 9, 'bold')).pack(pady=2)
                self.mcq_multiple_correct_vars.append(correct_var)
                if isinstance(option, dict):
                    img_frame = tk.Frame(opt_container, bg=self.colors['secondary'])
                    img_frame.pack(fill=tk.X, padx=5, pady=2)
                    tk.Label(img_frame, text="🖼️ Image:", bg=self.colors['secondary']).pack(side=tk.LEFT)
                    img_var = tk.StringVar(value=option.get('image', ''))
                    tk.Entry(img_frame, textvariable=img_var, bg=self.colors['entry'], width=40).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                    tk.Button(img_frame, text="Browse", bg=self.colors['button'], fg=self.colors['button_text'],
                             command=lambda v=img_var: self.browse_media_file(v, "image")).pack(side=tk.RIGHT, padx=2)
                    text_frame = tk.Frame(opt_container, bg=self.colors['secondary'])
                    text_frame.pack(fill=tk.X, padx=5, pady=2)
                    tk.Label(text_frame, text="📝 Text:", bg=self.colors['secondary']).pack(side=tk.LEFT)
                    text_var = tk.StringVar(value=option.get('text', ''))
                    tk.Entry(text_frame, textvariable=text_var, bg=self.colors['entry'], width=50).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                    self.mcq_multiple_option_vars.append((img_var, text_var))
                else:
                    text_frame = tk.Frame(opt_container, bg=self.colors['secondary'])
                    text_frame.pack(fill=tk.X, padx=5, pady=2)
                    tk.Label(text_frame, text="📝 Text:", bg=self.colors['secondary']).pack(side=tk.LEFT)
                    text_var = tk.StringVar(value=str(option))
                    tk.Entry(text_frame, textvariable=text_var, bg=self.colors['entry'], width=50).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                    self.mcq_multiple_option_vars.append((tk.StringVar(), text_var))
                if len(question.get('options', [])) > 2:
                    tk.Button(opt_container, text="🗑️ Delete Option", bg='#FF6347', fg=self.colors['button_text'],
                             command=lambda idx=i: delete_option(idx)).pack(pady=5)
        def add_option():
            question.setdefault('options', []).append({'image': '', 'text': 'New Fantastic Option'})
            refresh_options()
        def delete_option(idx):
            if len(question.get('options', [])) > 2:
                del question['options'][idx]
                answer = question.get('answer', [])
                question['answer'] = [a - 1 if a > idx else a for a in answer if a != idx]
                refresh_options()
        def save_options():
            options = []
            for img_var, text_var in self.mcq_multiple_option_vars:
                img, text = img_var.get().strip(), text_var.get().strip()
                if img and text: options.append({'image': img, 'text': text})
                elif img: options.append({'image': img, 'text': ''})
                elif text: options.append({'image': '', 'text': text})
                else: options.append({'image': '', 'text': 'Empty Option'})
            question['options'] = options
            question['answer'] = [i for i, var in enumerate(self.mcq_multiple_correct_vars) if var.get()]
            messagebox.showinfo("Success! 💖", "Enhanced multiple choice options saved beautifully!")
        refresh_options()
        btn_frame = tk.Frame(options_frame, bg=self.colors['white'])
        btn_frame.pack(fill=tk.X, pady=8)
        tk.Button(btn_frame, text="➕ Add Option", command=add_option,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)
        tk.Button(btn_frame, text="💾 Save All Options", command=save_options,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

    def edit_list_pick(self, question, index):
        """Editor for list_pick questions - pick all the cute options! 💖"""
        self.create_question_text_section(question, index)
        self.create_media_section(question, index)
        options_frame = tk.LabelFrame(self.editor_frame, text="📋 Answer Options (Students can pick multiple) 📋",
                                     font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        options_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(options_frame, text="💡 Check the boxes next to correct answers!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        options_list_frame = tk.Frame(options_frame, bg=self.colors['white'])
        options_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.list_pick_option_vars = []
        self.list_pick_correct_vars = []
        def refresh_options():
            for widget in options_list_frame.winfo_children(): widget.destroy()
            self.list_pick_option_vars.clear()
            self.list_pick_correct_vars.clear()
            for i, option in enumerate(question.get('options', [])):
                row_frame = tk.Frame(options_list_frame, bg=self.colors['white'])
                row_frame.pack(fill=tk.X, pady=2)
                correct_var = tk.BooleanVar(value=i in question.get('answer', []))
                tk.Checkbutton(row_frame, text="✅ Correct", variable=correct_var,
                              bg=self.colors['white'], font=('Arial', 9, 'bold')).pack(side=tk.LEFT)
                self.list_pick_correct_vars.append(correct_var)
                tk.Label(row_frame, text=f"Option {i+1}:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=(10, 5))
                option_var = tk.StringVar(value=option)
                tk.Entry(row_frame, textvariable=option_var, bg=self.colors['entry'],
                        width=50, font=('Arial', 10)).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                self.list_pick_option_vars.append(option_var)
                if len(question.get('options', [])) > 2:
                    tk.Button(row_frame, text="🗑️", bg='#FF6347', fg=self.colors['button_text'],
                             command=lambda idx=i: delete_option(idx)).pack(side=tk.RIGHT, padx=2)
        def add_option():
            question.setdefault('options', []).append('New Cute Option')
            refresh_options()
        def delete_option(idx):
            if len(question.get('options', [])) > 2:
                del question['options'][idx]
                answer = question.get('answer', [])
                question['answer'] = [a - 1 if a > idx else a for a in answer if a != idx]
                refresh_options()
        def save_options():
            question['options'] = [var.get() for var in self.list_pick_option_vars if var.get().strip()]
            question['answer'] = [i for i, var in enumerate(self.list_pick_correct_vars) if var.get()]
            messagebox.showinfo("Success! 💖", "Options saved successfully, darling!")
        refresh_options()
        btn_frame = tk.Frame(options_frame, bg=self.colors['white'])
        btn_frame.pack(fill=tk.X, pady=8)
        tk.Button(btn_frame, text="➕ Add Option", command=add_option,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)
        tk.Button(btn_frame, text="💾 Save All Options", command=save_options,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

    def edit_word_fill(self, question, index):
        """Editor for word_fill questions - now with multi-line sentence parts! 💖"""
        self.create_question_text_section(question, index)
        self.create_media_section(question, index)
        parts_frame = tk.LabelFrame(self.editor_frame, text="📝 Sentence Parts (Now supports newlines!) 📝",
                                font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        parts_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(parts_frame, text="💡 Example: 'The cat is ' [BLANK] ' and very ' [BLANK] ' today.'\nCreate parts: ['The cat is ', ' and very ', ' today.']",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        parts_list_frame = tk.Frame(parts_frame, bg=self.colors['white'])
        parts_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.word_fill_parts_widgets = []
        def refresh_parts():
            for widget in parts_list_frame.winfo_children(): widget.destroy()
            self.word_fill_parts_widgets.clear()
            for i, part in enumerate(question.get('sentence_parts', [])):
                row_frame = tk.Frame(parts_list_frame, bg=self.colors['white'])
                row_frame.pack(fill=tk.X, pady=2)
                tk.Label(row_frame, text=f"Part {i+1}:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                part_widget = tk.Text(row_frame, height=2, bg=self.colors['entry'], width=60, font=('Arial', 10), wrap=tk.WORD)
                part_widget.pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                part_widget.insert('1.0', part)
                self.word_fill_parts_widgets.append(part_widget)
                if len(question.get('sentence_parts', [])) > 1:
                    tk.Button(row_frame, text="🗑️", bg='#FF6347', fg=self.colors['button_text'],
                            command=lambda idx=i: delete_part(idx)).pack(side=tk.RIGHT, padx=2)
        answers_frame = tk.LabelFrame(parts_frame, text="✅ Correct Answers for Blanks ✅",
                                    font=('Arial', 11, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        answers_frame.pack(fill=tk.X, padx=5, pady=10)
        tk.Label(answers_frame, text="💡 Enter the correct word/phrase for each blank!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        answers_list_frame = tk.Frame(answers_frame, bg=self.colors['white'])
        answers_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.word_fill_answers_vars = []
        def refresh_answers():
            for widget in answers_list_frame.winfo_children(): widget.destroy()
            self.word_fill_answers_vars.clear()
            for i, answer in enumerate(question.get('answers', [])):
                row_frame = tk.Frame(answers_list_frame, bg=self.colors['white'])
                row_frame.pack(fill=tk.X, pady=2)
                tk.Label(row_frame, text=f"Answer for Blank {i+1}:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                answer_var = tk.StringVar(value=answer)
                tk.Entry(row_frame, textvariable=answer_var, bg=self.colors['entry'],
                        width=40, font=('Arial', 10)).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                self.word_fill_answers_vars.append(answer_var)
                if len(question.get('answers', [])) > 0:
                    tk.Button(row_frame, text="🗑️", bg='#FF6347', fg=self.colors['button_text'],
                            command=lambda idx=i: delete_answer(idx)).pack(side=tk.RIGHT, padx=2)
        def add_part():
            question.setdefault('sentence_parts', []).append('')
            refresh_parts()
        def delete_part(idx):
            if len(question.get('sentence_parts', [])) > 1:
                del question['sentence_parts'][idx]
                refresh_parts()
        def add_answer():
            question.setdefault('answers', []).append('')
            refresh_answers()
        def delete_answer(idx):
            if idx < len(question.get('answers', [])):
                del question['answers'][idx]
                refresh_answers()
        def save_all():
            question['sentence_parts'] = [w.get('1.0', tk.END).rstrip('\n') for w in self.word_fill_parts_widgets]
            question['answers'] = [v.get() for v in self.word_fill_answers_vars if v.get().strip()]
            messagebox.showinfo("Success! 💖", "Word fill data saved perfectly, babe!")
        refresh_parts()
        refresh_answers()
        btn_frame = tk.Frame(parts_frame, bg=self.colors['white'])
        btn_frame.pack(fill=tk.X, pady=8)
        tk.Button(btn_frame, text="➕ Add Part", command=add_part,
                bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=2)
        tk.Button(btn_frame, text="➕ Add Answer", command=add_answer,
                bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=2)
        tk.Button(btn_frame, text="💾 Save Everything", command=save_all,
                bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=2)

    def edit_categorization_multiple(self, question, index):
        """Editor for categorization_multiple questions - organize everything cutely! 📂"""
        self.create_question_text_section(question, index)
        self.create_media_section(question, index)
        categories_frame = tk.LabelFrame(self.editor_frame, text="📂 Categories (The cute buckets!) 📂",
                                        font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        categories_frame.pack(fill=tk.X, padx=10, pady=5)
        tk.Label(categories_frame, text="💡 Create the categories that students will sort items into!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        categories_list_frame = tk.Frame(categories_frame, bg=self.colors['white'])
        categories_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.categorization_category_vars = []
        def refresh_categories():
            for widget in categories_list_frame.winfo_children(): widget.destroy()
            self.categorization_category_vars.clear()
            for i, category in enumerate(question.get('categories', [])):
                row_frame = tk.Frame(categories_list_frame, bg=self.colors['white'])
                row_frame.pack(fill=tk.X, pady=2)
                tk.Label(row_frame, text=f"Category {i+1}:", bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                category_var = tk.StringVar(value=category)
                tk.Entry(row_frame, textvariable=category_var, bg=self.colors['entry'],
                        width=40, font=('Arial', 10)).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                self.categorization_category_vars.append(category_var)
                if len(question.get('categories', [])) > 2 and category.strip():
                    tk.Button(row_frame, text="🗑️", bg='#FF6347', fg=self.colors['button_text'],
                             command=lambda idx=i: delete_category(idx)).pack(side=tk.RIGHT, padx=2)
        stimuli_frame = tk.LabelFrame(self.editor_frame, text="🎯 Items to Categorize (Text and/or Images!) 🎯",
                                     font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        stimuli_frame.pack(fill=tk.X, padx=10, pady=5)
        tk.Label(stimuli_frame, text="💡 Add items (text and/or images) that students will categorize!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        stimuli_list_frame = tk.Frame(stimuli_frame, bg=self.colors['white'])
        stimuli_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.categorization_stimuli_data = []
        def refresh_stimuli():
            for widget in stimuli_list_frame.winfo_children(): widget.destroy()
            self.categorization_stimuli_data.clear()
            for i, stimulus in enumerate(question.get('stimuli', [])):
                stim_container = tk.LabelFrame(stimuli_list_frame, text=f"💖 Item {i+1} 💖",
                                              bg=self.colors['secondary'], font=('Arial', 10, 'bold'))
                stim_container.pack(fill=tk.X, pady=3, padx=2)
                text_frame = tk.Frame(stim_container, bg=self.colors['secondary'])
                text_frame.pack(fill=tk.X, padx=5, pady=2)
                tk.Label(text_frame, text="📝 Text (optional):", bg=self.colors['secondary'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                text_var = tk.StringVar(value=stimulus.get('text', '') or '')
                tk.Entry(text_frame, textvariable=text_var, bg=self.colors['entry'],
                        width=50, font=('Arial', 10)).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                image_frame = tk.Frame(stim_container, bg=self.colors['secondary'])
                image_frame.pack(fill=tk.X, padx=5, pady=2)
                tk.Label(image_frame, text="🖼️ Image (optional):", bg=self.colors['secondary'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                image_var = tk.StringVar(value=stimulus.get('image', '') or '')
                tk.Entry(image_frame, textvariable=image_var, bg=self.colors['entry'],
                        width=40, font=('Arial', 10)).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                tk.Button(image_frame, text="Browse 📁", bg=self.colors['button'], fg=self.colors['button_text'],
                         command=lambda v=image_var: self.browse_media_file(v, "image")).pack(side=tk.RIGHT, padx=2)
                category_frame = tk.Frame(stim_container, bg=self.colors['secondary'])
                category_frame.pack(fill=tk.X, padx=5, pady=2)
                tk.Label(category_frame, text="📂 Correct Category:", bg=self.colors['secondary'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                stimulus_key = stimulus.get('text', '') or (os.path.basename(stimulus.get('image', '')) if stimulus.get('image') else '')
                current_answer = question.get('answer', {}).get(stimulus_key, '')
                category_var = tk.StringVar(value=current_answer)
                categories_list = [c for c in question.get('categories', []) if c.strip()]
                if categories_list:
                    ttk.Combobox(category_frame, textvariable=category_var, values=categories_list,
                                 state='readonly', width=25).pack(side=tk.LEFT, padx=5)
                else:
                    tk.Entry(category_frame, textvariable=category_var, bg=self.colors['entry'],
                            width=25, font=('Arial', 10)).pack(side=tk.LEFT, padx=5)
                if len(question.get('stimuli', [])) > 1:
                    tk.Button(stim_container, text="🗑️ Delete This Item", bg='#FF6347', fg=self.colors['button_text'],
                             command=lambda idx=i: delete_stimulus(idx)).pack(pady=5)
                self.categorization_stimuli_data.append((text_var, image_var, category_var))
        def add_category():
            question.setdefault('categories', []).append('New Cute Category')
            refresh_categories()
            refresh_stimuli()
        def delete_category(idx):
            if len(question.get('categories', [])) > 2:
                del question['categories'][idx]
                refresh_categories()
                refresh_stimuli()
        def add_stimulus():
            question.setdefault('stimuli', []).append({'text': 'New Adorable Item', 'image': None})
            refresh_stimuli()
        def delete_stimulus(idx):
            if len(question.get('stimuli', [])) > 1:
                stimulus = question['stimuli'][idx]
                answer = question.get('answer', {})
                text_key, image_key = stimulus.get('text', ''), stimulus.get('image', '')
                if text_key and text_key in answer: del answer[text_key]
                elif image_key and image_key in answer: del answer[image_key]
                elif image_key and os.path.basename(image_key) in answer: del answer[os.path.basename(image_key)]
                del question['stimuli'][idx]
                refresh_stimuli()
        def save_all():
            question['categories'] = [v.get() for v in self.categorization_category_vars if v.get().strip()]
            stimuli, answer = [], {}
            for text_var, image_var, category_var in self.categorization_stimuli_data:
                text, image, category = text_var.get().strip(), image_var.get().strip() or None, category_var.get().strip()
                if text or image:
                    stimuli.append({'text': text or None, 'image': image})
                    answer_key = text or (os.path.basename(image) if image else '')
                    if category and answer_key: answer[answer_key] = category
            question['stimuli'], question['answer'] = stimuli, answer
            messagebox.showinfo("Success! 💖", "All categorization data saved perfectly, darling!")
        refresh_categories()
        refresh_stimuli()
        btn_frame = tk.Frame(stimuli_frame, bg=self.colors['white'])
        btn_frame.pack(fill=tk.X, pady=8)
        tk.Button(btn_frame, text="➕ Add Category", command=add_category,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=2)
        tk.Button(btn_frame, text="➕ Add Item", command=add_stimulus,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=2)
        tk.Button(btn_frame, text="💾 Save Everything", command=save_all,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=2)

    def edit_match_sentence(self, question, index):
        """Editor for match_sentence questions - match cutely! 💖"""
        self.create_question_text_section(question, index)
        self.create_media_section(question, index)
        pairs_frame = tk.LabelFrame(self.editor_frame, text="🔗 Sentence-Image Pairs (So cute!) 🔗",
                                   font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        pairs_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(pairs_frame, text="💡 Students will match sentences to their corresponding images!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        pairs_list_frame = tk.Frame(pairs_frame, bg=self.colors['white'])
        pairs_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.match_sentence_pairs_data = []
        def refresh_pairs():
            for widget in pairs_list_frame.winfo_children(): widget.destroy()
            self.match_sentence_pairs_data.clear()
            for i, pair in enumerate(question.get('pairs', [])):
                pair_container = tk.LabelFrame(pairs_list_frame, text=f"💖 Pair {i+1} 💖",
                                              bg=self.colors['secondary'], font=('Arial', 10, 'bold'))
                pair_container.pack(fill=tk.X, pady=5, padx=2)
                sentence_frame = tk.Frame(pair_container, bg=self.colors['secondary'])
                sentence_frame.pack(fill=tk.X, padx=5, pady=2)
                tk.Label(sentence_frame, text="📝 Sentence:", bg=self.colors['secondary'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                sentence_var = tk.StringVar(value=pair.get('sentence', ''))
                tk.Entry(sentence_frame, textvariable=sentence_var, bg=self.colors['entry'],
                        width=60, font=('Arial', 10)).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                image_frame = tk.Frame(pair_container, bg=self.colors['secondary'])
                image_frame.pack(fill=tk.X, padx=5, pady=2)
                tk.Label(image_frame, text="🖼️ Image Path:", bg=self.colors['secondary'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT)
                image_var = tk.StringVar(value=pair.get('image_path', ''))
                tk.Entry(image_frame, textvariable=image_var, bg=self.colors['entry'],
                        width=50, font=('Arial', 10)).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                tk.Button(image_frame, text="Browse 📁", bg=self.colors['button'], fg=self.colors['button_text'],
                         command=lambda v=image_var: self.browse_media_file(v, "image")).pack(side=tk.RIGHT, padx=2)
                if len(question.get('pairs', [])) > 1:
                    tk.Button(pair_container, text="🗑️ Delete This Pair", bg='#FF6347', fg=self.colors['button_text'],
                             command=lambda idx=i: delete_pair(idx)).pack(pady=5)
                self.match_sentence_pairs_data.append((sentence_var, image_var))
        def add_pair():
            question.setdefault('pairs', []).append({'sentence': 'New Cute Sentence', 'image_path': 'new_image.jpg'})
            refresh_pairs()
        def delete_pair(idx):
            if len(question.get('pairs', [])) > 1:
                pair = question['pairs'][idx]
                answer = question.get('answer', {})
                if pair.get('image_path') in answer: del answer[pair.get('image_path')]
                del question['pairs'][idx]
                refresh_pairs()
        def save_pairs():
            pairs, answer = [], {}
            for sentence_var, image_var in self.match_sentence_pairs_data:
                sentence, image_path = sentence_var.get().strip(), image_var.get().strip()
                if sentence and image_path:
                    pairs.append({'sentence': sentence, 'image_path': image_path})
                    answer[image_path] = sentence
            question['pairs'], question['answer'] = pairs, answer
            messagebox.showinfo("Success! 💖", "Sentence-image pairs saved perfectly, darling!")
        refresh_pairs()
        btn_frame = tk.Frame(pairs_frame, bg=self.colors['white'])
        btn_frame.pack(fill=tk.X, pady=8)
        tk.Button(btn_frame, text="➕ Add New Pair", command=add_pair,
                 bg=self.colors['button'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)
        tk.Button(btn_frame, text="💾 Save All Pairs", command=save_pairs,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

    def edit_sequence_audio(self, question, index):
        """Editor for sequence_audio questions - audio sequencing magic! 🎵"""
        self.create_question_text_section(question, index)
        self.create_media_section(question, index)
        options_frame = tk.LabelFrame(self.editor_frame, text="🎵 Audio Options (Students put in order) 🎵",
                                     font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        options_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(options_frame, text="💡 Describe each audio clip - students will put them in correct sequence!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        sequence_var = tk.StringVar(value=str(question.get('answer', [])))
        tk.Label(options_frame, text="✅ Correct sequence order (indices):",
                bg=self.colors['white'], font=('Arial', 10, 'bold')).pack(pady=5)
        tk.Entry(options_frame, textvariable=sequence_var, bg=self.colors['entry'], width=40).pack(pady=5)
        def save_sequence():
            try:
                seq_text = sequence_var.get().strip()
                question['answer'] = eval(seq_text) if seq_text.startswith('[') and seq_text.endswith(']') else [int(x.strip()) for x in seq_text.split(',')]
                messagebox.showinfo("Success! 💖", "Audio sequence saved beautifully!")
            except:
                messagebox.showerror("Oopsie! 😢", "Please enter a valid sequence format: [0, 1, 2] or 0, 1, 2")
        tk.Button(options_frame, text="💾 Save Sequence", command=save_sequence,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(pady=10)

    def edit_order_phrase(self, question, index):
        """Editor for order_phrase questions - get those phrases in order! 📋"""
        self.create_question_text_section(question, index)
        self.create_media_section(question, index)
        phrases_frame = tk.LabelFrame(self.editor_frame, text="📋 Phrase Ordering (So organized!) 📋",
                                     font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        phrases_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(phrases_frame, text="💡 Enter shuffled phrases (one per line) and correct order:",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 9, 'italic')).pack(pady=5)
        shuffled_text = tk.Text(phrases_frame, height=6, width=70, bg=self.colors['entry'])
        shuffled_text.pack(pady=5)
        shuffled_text.insert('1.0', '\n'.join(question.get('phrase_shuffled', [])))
        correct_text = tk.Text(phrases_frame, height=6, width=70, bg=self.colors['entry'])
        correct_text.pack(pady=5)
        correct_text.insert('1.0', '\n'.join(question.get('answer', [])))
        def save_phrases():
            question['phrase_shuffled'] = [line.strip() for line in shuffled_text.get('1.0', tk.END).strip().split('\n') if line.strip()]
            question['answer'] = [line.strip() for line in correct_text.get('1.0', tk.END).strip().split('\n') if line.strip()]
            messagebox.showinfo("Success! 💖", "Phrases saved in perfect order!")
        tk.Button(phrases_frame, text="💾 Save Phrases", command=save_phrases,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(pady=10)

    def edit_fill_blanks_dropdown(self, question, index):
        """A complete and beautiful editor for fill_blanks_dropdown questions! ⬇️💖"""
        self.create_question_text_section(question, index)
        self.create_media_section(question, index)
        main_editor_frame = tk.LabelFrame(self.editor_frame, text="⬇️ Dropdown Blanks Editor ⬇️",
                                         font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        main_editor_frame.pack(fill=tk.X, padx=10, pady=10)
        parts_frame = tk.LabelFrame(main_editor_frame, text="📝 Sentence Parts (with newlines!)",
                                   font=('Arial', 11, 'bold'), bg=self.colors['white'])
        parts_frame.pack(fill=tk.X, padx=5, pady=5)
        parts_list_frame = tk.Frame(parts_frame, bg=self.colors['white'])
        parts_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.fbd_parts_widgets = []
        def refresh_parts():
            for widget in parts_list_frame.winfo_children(): widget.destroy()
            self.fbd_parts_widgets.clear()
            for i, part in enumerate(question.get('sentence_parts', [])):
                row_frame = tk.Frame(parts_list_frame, bg=self.colors['white'])
                row_frame.pack(fill=tk.X, pady=2)
                tk.Label(row_frame, text=f"Part {i+1}:", bg=self.colors['white']).pack(side=tk.LEFT)
                part_widget = tk.Text(row_frame, height=2, bg=self.colors['entry'], wrap=tk.WORD)
                part_widget.pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                part_widget.insert('1.0', part)
                self.fbd_parts_widgets.append(part_widget)
                if len(question.get('sentence_parts', [])) > 1:
                    tk.Button(row_frame, text="🗑️", bg='#FF6347', fg=self.colors['button_text'],
                             command=lambda idx=i: delete_part(idx)).pack(side=tk.RIGHT)
        blanks_frame = tk.LabelFrame(main_editor_frame, text="✅ Blanks, Options & Answers",
                                    font=('Arial', 11, 'bold'), bg=self.colors['white'])
        blanks_frame.pack(fill=tk.X, padx=5, pady=10)
        blanks_list_frame = tk.Frame(blanks_frame, bg=self.colors['white'])
        blanks_list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.fbd_blanks_data = []
        def refresh_blanks():
            for widget in blanks_list_frame.winfo_children(): widget.destroy()
            self.fbd_blanks_data.clear()
            options_for_blanks = question.get('options_for_blanks', [])
            answers = question.get('answers', [])
            for i in range(len(options_for_blanks)):
                blank_container = tk.LabelFrame(blanks_list_frame, text=f"💖 Dropdown Blank {i+1} 💖",
                                               bg=self.colors['secondary'])
                blank_container.pack(fill=tk.X, pady=4, padx=2)
                opts_frame = tk.Frame(blank_container, bg=self.colors['secondary'])
                opts_frame.pack(fill=tk.X, padx=5, pady=2)
                tk.Label(opts_frame, text="Options (separate with ';'):", bg=self.colors['secondary']).pack(side=tk.LEFT)
                options_str = '; '.join(options_for_blanks[i])
                options_var = tk.StringVar(value=options_str)
                tk.Entry(opts_frame, textvariable=options_var, bg=self.colors['entry'], font=('Arial', 10)).pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)
                ans_frame = tk.Frame(blank_container, bg=self.colors['secondary'])
                ans_frame.pack(fill=tk.X, padx=5, pady=2)
                tk.Label(ans_frame, text="✅ Correct Answer:", bg=self.colors['secondary']).pack(side=tk.LEFT)
                answer_var = tk.StringVar(value=answers[i] if i < len(answers) else '')
                ttk.Combobox(ans_frame, textvariable=answer_var,
                             values=options_for_blanks[i], state='readonly', width=30).pack(side=tk.LEFT, padx=5)
                if len(options_for_blanks) > 1:
                    tk.Button(blank_container, text="🗑️ Delete This Blank", bg='#FF6347', fg=self.colors['button_text'],
                             command=lambda idx=i: delete_blank(idx)).pack(pady=3)
                self.fbd_blanks_data.append({'options_var': options_var, 'answer_var': answer_var})
        def add_part():
            question.setdefault('sentence_parts', []).append('')
            refresh_parts()
        def delete_part(idx):
            if len(question.get('sentence_parts', [])) > 1:
                del question['sentence_parts'][idx]
                refresh_parts()
        def add_blank():
            question.setdefault('options_for_blanks', []).append([' ', 'Option A', 'Option B'])
            question.setdefault('answers', []).append('Option A')
            refresh_blanks()
        def delete_blank(idx):
            if len(question.get('options_for_blanks', [])) > 1:
                del question['options_for_blanks'][idx]
                if idx < len(question.get('answers', [])): del question['answers'][idx]
                refresh_blanks()
        def save_all():
            question['sentence_parts'] = [w.get('1.0', tk.END).rstrip('\n') for w in self.fbd_parts_widgets]
            new_options, new_answers = [], []
            for data in self.fbd_blanks_data:
                opts = [opt.strip() for opt in data['options_var'].get().split(';') if opt.strip()]
                new_options.append(opts)
                new_answers.append(data['answer_var'].get())
            question['options_for_blanks'], question['answers'] = new_options, new_answers
            messagebox.showinfo("Success! 💖", "Dropdown question saved perfectly, darling!")
            refresh_blanks()
        refresh_parts()
        refresh_blanks()
        btn_frame = tk.Frame(main_editor_frame, bg=self.colors['white'])
        btn_frame.pack(fill=tk.X, pady=10)
        tk.Button(btn_frame, text="➕ Add Part", command=add_part,
                 bg=self.colors['button'], fg=self.colors['button_text']).pack(side=tk.LEFT, padx=5)
        tk.Button(btn_frame, text="➕ Add Blank", command=add_blank,
                 bg=self.colors['button'], fg=self.colors['button_text']).pack(side=tk.LEFT, padx=5)
        tk.Button(btn_frame, text="💾 Save Everything", command=save_all,
                 bg=self.colors['accent'], fg=self.colors['button_text'], font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=5)

    def edit_match_phrases(self, question, index):
        """Editor for match_phrases questions - match those cute phrases! 🔗"""
        self.create_question_text_section(question, index)
        self.create_media_section(question, index)
        match_frame = tk.LabelFrame(self.editor_frame, text="🔗 Match Phrase Beginnings to Endings 🔗",
                                   font=('Arial', 12, 'bold'), bg=self.colors['white'], fg=self.colors['text'])
        match_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(match_frame, text="💡 This is a complex editor - use JSON view for detailed editing!",
                bg=self.colors['white'], fg=self.colors['text'], font=('Arial', 10, 'italic')).pack(pady=20)

def main():
    """Main function to run the most adorable editor ever! 💖"""
    root = tk.Tk()
    try:
        root.state('zoomed')
    except tk.TclError:
        try: root.attributes('-zoomed', True)
        except tk.TclError: pass
    root.minsize(1200, 800)
    app = WifeyMOOCEditor(root)
    def on_closing():
        if messagebox.askyesno("Leaving? 💔", "Are you sure you want to close the editor, princess?"):
            messagebox.showinfo("Goodbye! 💖", "Thanks for using Wifey MOOC Editor v2.0, babe! 💕")
            root.destroy()
    root.protocol("WM_DELETE_WINDOW", on_closing)
    root.mainloop()

if __name__ == "__main__":
    print("💖 Starting the most adorable MOOC editor v3.4 ever! 💖")
    print("✨ New feature: Prompts are now in an external file! ✨")
    print("💕 Made with extra love for all the girlies! 💕")
    main()
