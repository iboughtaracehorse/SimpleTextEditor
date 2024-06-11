#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class TextEditor {

public:
    TextEditor(size_t initialSize)
        : initialSize(initialSize) {
        text = new char* [initialSize];

        for (size_t i = 0; i < initialSize; ++i) {
            text[i] = new char[initialSize]();
        }
        strcpy_s(text[0], initialSize, "Hello World");

        undoArray = new char** [maxStates];
        redoArray = new char** [maxStates];
    }

    ~TextEditor() {
        for (size_t i = 0; i < initialSize; ++i) {
            delete[] text[i];
        }
        delete[] text;

        while (undo >= 0) {
            for (size_t i = 0; i < initialSize; ++i) {
                delete[] undoArray[undo][i];
            }
            delete[] undoArray[undo];
            --undo;
        }
        delete[] undoArray;

        while (redo >= 0) {
            for (size_t i = 0; i < initialSize; ++i) {
                delete[] redoArray[redo][i];
            }
            delete[] redoArray[redo];
            --redo;
        }
        delete[] redoArray;
    
    }


    void main() {
        char userInput[20];

        while (true) {
            std::cout << "Enter a command or 'help' to see the list of available commands: ";
            std::cin >> userInput;

            int command = getCommand(userInput);

            switch (command) {
            case COMMAND_APPEND:
                append();
                break;
            case COMMAND_INSERT:
                insert();
                break;
            case COMMAND_NEWLINE:
                newLine();
                break;
            case COMMAND_SAVE:
                save();
                break;
            case COMMAND_LOAD:
                load();
                break;
            case COMMAND_SEARCH:
                search();
                break;
            case COMMAND_HELP:
                help();
                break;
            case COMMAND_PRINT:
                printAllText();
                break;
            case COMMAND_EXIT:
                std::cout << "Exiting...\n";
                return;
            default:
                std::cout << "Unknown command. Please try again.\n";
                break;
            }
        }
    }


private:

    enum Commands {
        COMMAND_APPEND,
        COMMAND_INSERT,
        COMMAND_NEWLINE,
        COMMAND_SAVE,
        COMMAND_LOAD,
        COMMAND_SEARCH,
        COMMAND_HELP,
        COMMAND_EXIT,
        COMMAND_PRINT,
        COMMAND_UNKNOWN
    };

    char** text;
    const size_t initialSize;
    char*** undoArray;
    char*** redoArray;
    int undo;
    int redo;
    const int maxStates = 3;
    const char* commandsToStrings[9] = {"append", "insert", "newline", "save", "load", "search", "help", "exit", "print"};


    int getCommand(const char* userInput) {
        for (size_t i = 0; i < sizeof(commandsToStrings) / sizeof(commandsToStrings[0]); ++i) {
            if (strcmp(userInput, commandsToStrings[i]) == 0) {
                return i;
            }
        }
        return COMMAND_UNKNOWN;
    }

    void append() {
        const size_t initialSize = 256;
        char newText[initialSize];
        std::cout << "Enter text to append: ";
        std::cin.ignore();
        std::cin.getline(newText, initialSize);
        strcat_s(text[findEndOfText()], initialSize, newText);
        saveState();
    }

    void newLine() {
        strcat_s(text[findEndOfText()], initialSize, "\n");
    }

    void save() {
        const size_t bufferSize = 256;
        char filePath[bufferSize];

        std::cout << "Input full path to file: ";
        std::cin.ignore();
        std::cin.getline(filePath, bufferSize);

        FILE* file;
        errno_t err = fopen_s(&file, filePath, "a"); 
        if (err == 0 && file != nullptr) {
            for (size_t i = 0; i < initialSize; ++i) {
                if (text[i][0] != '\0') {
                    fprintf(file, "%s\n", text[i]);
                }
            }
            fclose(file);
            std::cout << "Your text was successfully saved!\n";
        }
        else {
            std::cout << "Failed to create or open the file for writing. Error code: " << err << "\n";
        }
    }


    void load() {
        const size_t initialSize = 256;
        char filePath[initialSize];
        std::cout << "Input full path to file: ";
        std::cin.ignore();
        std::cin.getline(filePath, initialSize);

        FILE* file;
        errno_t err = fopen_s(&file, filePath, "r");
        if (file != nullptr) {
            char line[initialSize];
            size_t lineIndex = 0;

            while (lineIndex < initialSize && fgets(line, initialSize, file) != nullptr) {
                size_t len = strlen(line);
                if (len > 0 && line[len - 1] == '\n') {
                    line[len - 1] = '\0';
                }
                strncpy_s(text[lineIndex], initialSize, line, _TRUNCATE);
                lineIndex++;
            }
            fclose(file);
            std::cout << "Text was successfully loaded!\n";
        }
        else {
            std::cout << "Failed to open the file.\n";
        }
    }


    void search() {
        const size_t initialSize = 256;

        char substring[initialSize];
        std::cout << "Enter the substring to search: ";
        std::cin.ignore();
        std::cin.getline(substring, initialSize);

        bool found = false;
        for (size_t i = 0; i < initialSize; ++i) {
            if (strstr(text[i], substring) != nullptr) {
                std::cout << "Found substring at row " << i << "\n";
                found = true;
            }
        }
        if (!found) {
            std::cout << "No such substring in your text!\n";
        }
    }

    void help() {
        std::cout << "\nAvailable commands:\n";
        std::cout << "  append   -- Append text to the end of the text\n";
        std::cout << "  insert   -- Insert text at a specific position\n";
        std::cout << "  newline  -- Start a new line\n";
        std::cout << "  save     -- Save into the file\n";
        std::cout << "  load     -- Load an external file\n";
        std::cout << "  print    -- Print all text to the console\n";
        std::cout << "  search   -- Search for a specific word in the text\n";
        std::cout << "  help     -- Display all available commands\n";
        std::cout << "  exit     -- Exit editor\n\n";
    }

    void printAllText() {
        for (size_t i = 0; i < initialSize; ++i) {
            if (text[i][0] != '\0') {
                std::cout << text[i] << "\n";
            }
        }
    }

    size_t findEndOfText() {
        size_t endline = initialSize - 1;
        while (endline > 0 && text[endline][0] == '\0') {
            endline--;
        }
        return endline;
    }

    void insert() {
        const size_t initialSize = 256;
        char newText[initialSize];
        int row, index;

        std::cout << "Enter text to insert: ";
        std::cin.ignore();
        std::cin.getline(newText, initialSize);

        std::cout << "Enter row number: ";
        std::cin >> row;

        std::cout << "Enter position: ";
        std::cin >> index;

        if (row < 0 || row >= initialSize || index < 0 || index >= initialSize) {
            std::cout << "Insertion cancelled: Invalid row or index\n";
            return;
        }

        size_t newTextLength = strlen(newText);
        size_t textLength = strlen(text[row]);
        if (textLength + newTextLength >= initialSize) {
            std::cout << "Insertion cancelled. No space.\n";
            return;
        }

        char temp[initialSize];
        for (size_t i = 0; i < index && i < textLength; ++i) {
            temp[i] = text[row][i];
        }

        for (size_t i = 0; i < newTextLength; ++i) {
            temp[index + i] = newText[i];
        }

        for (size_t i = index; i < textLength; ++i) {
            temp[newTextLength + i] = text[row][i];
        }

        temp[textLength + newTextLength] = '\0';
        strcpy_s(text[row], initialSize, temp);

        std::cout << "Text inserted successfully.\n";
        saveState();
    }

    char** copyText(){
        char** copy = new char* [initialSize];
        for (size_t i = 0; i < initialSize; ++i) {
            copy[i] = new char[initialSize];
            strcpy_s(copy[i], initialSize, text[i]);
        }
        return copy;
    }

    void saveState() {
        if (undo < maxStates - 1) {
            ++undo;
            undoArray[undo] = copyText();
        }
        if (redo >= 0) {
            while (redo >= 0) {
                for (size_t i = 0; i < initialSize; ++i) {
                    delete[] redoArray[redo][i];
                }
                delete[] redoArray[redo];
                --redo;
            }
        }
    }

};

int main() {
    TextEditor editor(256);
    editor.main();
    return 0;
}
