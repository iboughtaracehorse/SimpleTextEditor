#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

class TextEditor {

public:
    TextEditor(size_t initialSize)
        : initialSize(initialSize), undo(-1), redo(-1) {
        text = new char* [initialSize];

        for (size_t i = 0; i < initialSize; ++i) {
            text[i] = new char[initialSize]();
            memset(text[i], 0, initialSize);
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
                copyText();
                append();
                break;
            case COMMAND_INSERT:
                copyText();
                insert();
                break;
            case COMMAND_NEWLINE:
                newLine();
                break;
            case COMMAND_SAVE:
                saveToFile();
                break;
            case COMMAND_LOAD:
                loadFromFile();
                break;
            case COMMAND_SEARCH:
                searchSubstring();
                break;
            case COMMAND_HELP:
                help();
                break;
            case COMMAND_PRINT:
                printAllText();
                break;
            case COMMAND_DELETE:
                deleteText();
                break;
            case COMMAND_UNDO:
                undoCommand();
                break;
            case COMMAND_REDO:
                redoCommand();
                break;
            case COMMAND_COPY:
                copy();
                break;
            case COMMAND_PASTE:
                paste(copied);
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
        COMMAND_DELETE,
        COMMAND_UNDO,
        COMMAND_REDO,
        COMMAND_COPY,
        COMMAND_PASTE,
        COMMAND_UNKNOWN
    };

    char** text;
    const size_t initialSize;
    char*** undoArray;
    char*** redoArray;
    int undo = 0;
    int redo = 0;
    const int maxStates = 3;
    char copied[256];
    const char* commandsToStrings[14] = { "append", "insert", "newline", "save", "load", "search", "help", "exit",
                                          "print", "delete", "undo", "redo", "copy", "paste"};


    int getCommand(const char* userInput) {
        for (size_t i = 0; i < sizeof(commandsToStrings) / sizeof(commandsToStrings[0]); ++i) {
            if (strcmp(userInput, commandsToStrings[i]) == 0) {
                return i;
            }
        }
        return COMMAND_UNKNOWN;
    }

    void append() {
        const size_t bufferSize = 256;
        char newText[bufferSize];
        std::cout << "Enter text to append: ";
        std::cin.ignore();
        std::cin.getline(newText, bufferSize);

        strcat_s(text[findEndOfText()], bufferSize, newText);
        std::cout << "Your text was successfully appended!\n";
    }

    void newLine() {
        strcat_s(text[findEndOfText()], initialSize, "\n");
    }

    void saveToFile() {
        const size_t bufferSize = 256;
        char filePath[bufferSize];

        std::cout << "Input full path to file: ";
        std::cin.ignore();
        std::cin.getline(filePath, bufferSize);

        FILE* file;
        errno_t err = fopen_s(&file, filePath, "w");
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

    void loadFromFile() {
        const size_t bufferSize = 256;
        char filePath[bufferSize];
        std::cout << "Input full path to file: ";
        std::cin.ignore();
        std::cin.getline(filePath, bufferSize);

        FILE* file;
        errno_t err = fopen_s(&file, filePath, "r");
        if (file != nullptr) {
            char line[bufferSize];
            size_t lineIndex = 0;

            while (lineIndex < initialSize && fgets(line, bufferSize, file) != nullptr) {
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

    void searchSubstring() {
        const size_t bufferSize = 256;

        char substring[bufferSize];
        std::cout << "Enter the substring to search: ";
        std::cin.ignore();
        std::cin.getline(substring, bufferSize);

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
        std::cout << "  insert   -- Insert text at a specific position. Has two modes(insert and replace)\n";
        std::cout << "  newline  -- Start a new line\n";
        std::cout << "  save     -- Save into the file\n";
        std::cout << "  load     -- Load an external file\n";
        std::cout << "  print    -- Print all text to the console\n";
        std::cout << "  search   -- Search for a specific word in the text\n";
        std::cout << "  delete   -- Delete a specified number of characters from a specific position\n";
        std::cout << "  undo     -- Undoes changes\n";
        std::cout << "  redo     -- Redoes changes\n";
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
        const size_t bufferSize = 256;
        char newText[bufferSize];
        int row, index;
        int mode;

        std::cout << "Choose mode (1 for insert, 2 for replace): ";
        std::cin >> mode;

        std::cout << "Enter text to insert: ";
        std::cin.ignore();
        std::cin.getline(newText, bufferSize);

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

        if (mode == 1) {


            if (textLength + newTextLength >= initialSize) {
                std::cout << "Insertion cancelled. No space.\n";
                return;

            }

            char temp[bufferSize];

            size_t i = 0;
            for (; i < index; ++i) {
                if (i < textLength) {
                    temp[i] = text[row][i];
                }
                else {
                    temp[i] = ' ';
                }
            }

            for (size_t j = 0; j < newTextLength; ++j, ++i) {
                temp[i] = newText[j];
            }

            for (size_t j = index; j < textLength; ++j, ++i) {
                temp[i] = text[row][j];
            }

            temp[i] = '\0';

            strcpy_s(text[row], bufferSize, temp);

            std::cout << "Text inserted successfully.\n";
        }
        
        else if (mode == 2) {

            size_t maxLength = initialSize - index;

            if (newTextLength > maxLength) {
                std::cout << "Too long to replace!\n";
                return;
            }

            for (size_t i = 0; i < newTextLength && (index + 1) < initialSize; i++) {
                text[row][i] = newText[i];
            }

            std::cout << "Successfully replaced!\n";
        }

        else {
            std::cout << "Unknown mode!\n";
        }
        
    }

    void deleteText() {

        const size_t bufferSize = 256;
        int row, index, length;

        std::cout << "Enter row number: ";
        std::cin >> row;

        std::cout << "Enter position: ";
        std::cin >> index;

        std::cout << "Enter number of characters to delete: ";
        std::cin >> length;

        if (row < 0 || row >= initialSize || index < 0 || index >= initialSize || length < 0) {
            std::cout << "Deletion cancelled: Invalid row, index, or length\n";
            return;
        }

        size_t textLength = strlen(text[row]);
        if (index + length > textLength) {
            std::cout << "Deletion cancelled: An amount of symbols to delete exceeds text length\n";
            return;
        }

        for (size_t i = index; i < textLength - length; ++i) {
            text[row][i] = text[row][i + length];
        }

        text[row][textLength - length] = '\0';

        std::cout << "Text deleted successfully.\n";
    }

    void copyText() {

        if (undo + 1 >= maxStates) {

            for (size_t i = 0; i < maxStates; i++) {
                delete[] undoArray[0][i];
            }
            
            delete[] undoArray[0];

            for (int i = 1; i < maxStates; i++) {
                undoArray[i - 1] = undoArray[i];
            }

            undo--;

            }

        undo++;

        undoArray[undo] = new char* [initialSize];

        for (size_t i = 0; i < initialSize; i++) {
            undoArray[undo][i] = new char[initialSize];
            strcpy_s(undoArray[undo][i], initialSize, text[i]);

        }
    }

    void undoCommand() {

        if (undo < 0) {
            std::cout << "You cannot undo further.\n";
            return;
        }

        if (redo + 1 >= maxStates) {
            for (size_t i = 0; i < initialSize; i++) {
                delete[] redoArray[0][i];
            }
            delete[] redoArray[0];

            for (int i = 1; i < maxStates; i++) {
                redoArray[i - 1] = redoArray[i];
            }

            redoArray[maxStates - 1] = new char* [initialSize];

            for (size_t i = 0; i < initialSize; i++) {
                redoArray[maxStates - 1][i] = new char[initialSize];
                strcpy_s(text[i], initialSize, redoArray[maxStates - 1][i]);

            }
        }

        else {
            redo++;
            redoArray[redo] = new char* [initialSize];

            for (size_t i = 0; i < initialSize; i++) {
                redoArray[redo][i] = new char[initialSize];
                strcpy_s(redoArray[redo][i], initialSize, text[i]);
            }
        }

#
        for (size_t i = 0; i < initialSize; i++) {
            strcpy_s(text[i], initialSize, undoArray[undo][i]);
            delete[] undoArray[undo][i];
        }
        delete[] undoArray[undo];
        undo--;

        std::cout << "Undo was successful.\n";

    }void redoCommand() {

        if (redo < 0) {
            std::cout << "You cannot redo further.\n";
            return;
        }

        if (undo + 1 >= maxStates) {
            for (size_t i = 0; i < initialSize; i++) {
                delete[] undoArray[0][i];
            }
            delete[] undoArray[0];

            for (int i = 1; i < maxStates; i++) {
                undoArray[i - 1] = undoArray[i];
            }

            undoArray[maxStates - 1] = new char* [initialSize];

            for (size_t i = 0; i < initialSize; i++) {
                undoArray[maxStates - 1][i] = new char[initialSize];
                strcpy_s(text[i], initialSize, undoArray[maxStates - 1][i]);

            }
        }

        else {
            undo++;
            undoArray[undo] = new char* [initialSize];

            for (size_t i = 0; i < initialSize; i++) {
                undoArray[undo][i] = new char[initialSize];
                strcpy_s(undoArray[undo][i], initialSize, text[i]);
            }
        }


        for (size_t i = 0; i < initialSize; i++) {
            strcpy_s(text[i], initialSize, redoArray[redo][i]);
            delete[] redoArray[redo][i];
        }
        delete[] redoArray[redo];
        redo--;

        std::cout << "Redo was successful.\n";
    }

    void copy() {
        const size_t bufferSize = 256;
        int row, position, numChars;

        std::cout << "Enter a row: \n";
        std::cin >> row;
        std::cout << "Enter a position: \n";
        std::cin >> position;
        std::cout << "Enter an amount of chars to copy (no more than 256): \n";
        std::cin >> numChars;

        if (row < 0 || row >= initialSize) {
            std::cout << "Invalid row!!\n";
            return;
        }

        strncpy_s(copied, bufferSize, text[row] + position, numChars);
        copied[numChars] = '\0';

        std::cout << "Copied: " << copied << "\n";
    }

    void paste(const char * copiedText) {
        const size_t initialSize = 256;
        int row, position;
        std::cout << "Enter a row: \n";
        std::cin >> row;
        std::cout << "Enter a position: \n";
        std::cin >> position;
       
        size_t length = strlen(text[row]);
        size_t copiedLength = strlen(copiedText);
        size_t requiredLength = length + copiedLength;

        if (row < 0 || row >= initialSize) {
            std::cout << "Invalid row!!\n";
            return;
        }

        if (position < 0 || position > length) {
            std::cout << "Invalid position!!\n";
            return;
        }

        if (copiedLength >= initialSize) {
            std::cout << "Not enough place to paste text!!\n";
            return;
        }

        for (size_t i = length; i >= position; i--) {
            text[row][position + i] = text[row][i];
        }

        for (size_t i = 0; i < copiedLength; i++) {
            text[row][i] = copied[i];
        }

 
        std::cout << "Pasted successfully!!\n";

    }

};

int main() {
    TextEditor editor(256);
    editor.main();
    return 0;
}
