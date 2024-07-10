#include <iostream>
#include <fstream>
#include <sstream>

#include "Windows.h"
#include "DynamicLibrary.h"

class CaesarCipher {
public:
    CaesarCipher() {}

    char* encrypt(char* inputText) {
        int key = 1;
        /*std::cout << "Enter a key: ";
        std::cin >> key;*/

        return encryptT(inputText, key);
    }

    char* decrypt(char* inputText) {
        int key = 1;
        /*std::cout << "Enter a key: ";
        std::cin >> key;*/

        return decryptT(inputText, key);
    }


private:

    char* encryptT(const char* rawText, int key) {
        const char* dllPath = "C:\\Users\\dariy\\Documents\\GitHub\\ENCRYPT3\\encrypt.dll";
        HINSTANCE hDLL = LoadLibraryA(dllPath);

        if (hDLL == nullptr) {
            std::cerr << "DLL connection failed!\n";
            return nullptr;
        }

        typedef char* (__cdecl* ENCRYPT_FUNC)(const char*, int);
        ENCRYPT_FUNC encryptFunc = (ENCRYPT_FUNC)GetProcAddress(hDLL, "encrypt");

        if (encryptFunc == nullptr) {
            std::cerr << "Encryption function not found in the DLL.\n";
            FreeLibrary(hDLL);
            return nullptr;
        }

        char* encryptedText = encryptFunc(rawText, key);
        FreeLibrary(hDLL);
        return encryptedText;
    }

    char* decryptT(const char* rawText, int key) {
        const char* dllPath = "C:\\Users\\dariy\\Documents\\GitHub\\ENCRYPT3\\encrypt.dll";
        HINSTANCE hDLL = LoadLibraryA(dllPath);

        if (hDLL == nullptr) {
            std::cerr << "Dll connection failed!\n";
            return nullptr;
        }

        typedef char* (__cdecl* DECRYPT_FUNC)(const char*, int);
        DECRYPT_FUNC decryptFunc = (DECRYPT_FUNC)GetProcAddress(hDLL, "decrypt");

        if (decryptFunc == nullptr) {
            std::cerr << "Decryption function not found in the DLL.\n";
            FreeLibrary(hDLL);
            return nullptr;
        }

        char* decryptedText = decryptFunc(rawText, key);
        freeEncryptedText(decryptedText);
        FreeLibrary(hDLL);
        return decryptedText;
    }

    void freeEncryptedText(char* ptr) {
        const char* dllPath = "C:\\Users\\dariy\\Documents\\GitHub\\ENCRYPT3\\encrypt.dll";
        HINSTANCE hDLL = LoadLibraryA(dllPath);

        if (hDLL == nullptr) {
            std::cerr << "Dll connection failed!\n";
            return;
        }

        typedef void(__cdecl* FREE_FUNC)(char*);
        FREE_FUNC freeFunc = (FREE_FUNC)GetProcAddress(hDLL, "free_encrypted");

        if (freeFunc == nullptr) {
            std::cerr << "Free function not found in the DLL.\n";
        }
        else {
            freeFunc(ptr);
        }

        FreeLibrary(hDLL);
    }

};

class Text {
public:
    Text(size_t initialSize) : initialSize(initialSize) {
        text = new char* [initialSize];
        for (size_t i = 0; i < initialSize; ++i) {
            text[i] = new char[initialSize]();
        }
        undoArray = new char** [maxStates];
        redoArray = new char** [maxStates];
    }

    ~Text() {
        for (size_t i = 0; i < initialSize; ++i) {
            delete[] text[i];
        }
        delete[] text;

        clearArray(undoArray, undo);
        clearArray(redoArray, redo);
    }

    void clearArray(char*** array, int& index) {
        while (index >= 0) {
            for (size_t i = 0; i < initialSize; ++i) {
                delete[] array[index][i];
            }
            delete[] array[index];
            --index;
        }
        delete[] array;
    }

    char** getText() const {
        return text;
    }

    size_t getInitialSize() const {
        return initialSize;
    }

    char*** getUndoArray() {
        return undoArray;
    }

    char*** getRedoArray() {
        return redoArray;
    }

    int& getUndo() {
        return undo;
    }

    int& getRedo() {
        return redo;
    }

    int getMaxStates() const {
        return maxStates;
    }

private:
    char** text;
    size_t initialSize;
    char*** undoArray;
    char*** redoArray;
    int undo = 0;
    int redo = 0;
    const int maxStates = 3;

};

class TextEditor {
public:
    TextEditor(size_t initialSize) : text(initialSize) {
        currentText = text.getText();
        strcpy_s(currentText[0], text.getInitialSize(), "Hello World");
    }

    void run() {
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
                copyText();
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
                copyText();

                int row, position, numChars;
                std::cout << "Enter row number: ";
                std::cin >> row;
                std::cout << "Enter position: ";
                std::cin >> position;
                std::cout << "Enter number of characters to delete: ";
                break;
            case COMMAND_UNDO:
                undoCommand();
                break;
            case COMMAND_REDO:
                redoCommand();
                break;
            case COMMAND_COPY:
                int rowCopy, positionCopy, numCharsCopy;
                std::cout << "Enter a row: \n";
                std::cin >> rowCopy;
                std::cout << "Enter a position: \n";
                std::cin >> positionCopy;
                std::cout << "Enter an amount of chars to copy (no more than 256): \n";
                std::cin >> numCharsCopy;
                copy(rowCopy, positionCopy, numCharsCopy);
                break;
            case COMMAND_PASTE:
                copyText();
                paste();
                break;
            case COMMAND_CUT:
                copyText();
                cut();
                break;
            case COMMAND_ENCRYPT:
                copyText();
                encryptText();
                
                break;
            case COMMAND_DECRYPT:
                copyText();
                decryptText();

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
        COMMAND_CUT,
        COMMAND_ENCRYPT,
        COMMAND_DECRYPT,
        COMMAND_UNKNOWN
    };

    char copied[256];
    Text text;
    char** currentText;
    CaesarCipher caesar;

    const char* commandsToStrings[17] = { "append", "insert", "newline", "save", "load", "search", "help", "exit",
                                          "print", "delete", "undo", "redo", "copy", "paste", "cut", "encrypt", "decrypt"};

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

        strcat_s(currentText[findEndOfText()], bufferSize, newText);
        std::cout << "Your text was successfully appended!\n";
    }

    void newLine() {
        strcat_s(currentText[findEndOfText()], text.getInitialSize(), "\n");
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
            for (size_t i = 0; i < text.getInitialSize(); ++i) {
                if (currentText[i][0] != '\0') {
                    fprintf(file, "%s\n", currentText[i]);
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

            while (lineIndex < text.getInitialSize() && fgets(line, bufferSize, file) != nullptr) {
                size_t len = strlen(line);
                if (len > 0 && line[len - 1] == '\n') {
                    line[len - 1] = '\0';
                }
                strncpy_s(currentText[lineIndex], text.getInitialSize(), line, _TRUNCATE);
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
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            if (strstr(currentText[i], substring) != nullptr) {
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
        std::cout << "  copy     -- Copies text\n";
        std::cout << "  paste    -- Pastes text\n";
        std::cout << "  cut      -- Cuts text\n";
        std::cout << "  help     -- Display all available commands\n";
        std::cout << "  exit     -- Exit editor\n\n";
    }

    void printAllText() {
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            if (currentText[i][0] != '\0') {
                std::cout << currentText[i] << "\n";
            }
        }
    }

    size_t findEndOfText() const {
        size_t endline = text.getInitialSize() - 1;
        while (endline > 0 && currentText[endline][0] == '\0') {
            --endline;
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

        if (row < 0 || row >= text.getInitialSize() || index < 0 || index >= text.getInitialSize()) {
            std::cout << "Insertion cancelled: Invalid row or index\n";
            return;
        }

        size_t newTextLength = strlen(newText);
        size_t textLength = strlen(currentText[row]);

        if (mode == 1) {


            if (textLength + newTextLength >= text.getInitialSize()) {
                std::cout << "Insertion cancelled. No space.\n";
                return;

            }

            char temp[bufferSize];

            size_t i = 0;
            for (; i < index; ++i) {
                if (i < textLength) {
                    temp[i] = currentText[row][i];
                }
                else {
                    temp[i] = ' ';
                }
            }

            for (size_t j = 0; j < newTextLength; ++j, ++i) {
                temp[i] = newText[j];
            }

            for (size_t j = index; j < textLength; ++j, ++i) {
                temp[i] = currentText[row][j];
            }

            temp[i] = '\0';

            strcpy_s(currentText[row], bufferSize, temp);

            std::cout << "Text inserted successfully.\n";
        }
        
        else if (mode == 2) {

            size_t maxLength = text.getInitialSize() - index;

            if (newTextLength > maxLength) {
                std::cout << "Too long to replace!\n";
                return;
            }

            for (size_t i = 0; i < newTextLength && (index + 1) < text.getInitialSize(); i++) {
                currentText[row][index + i] = newText[i];
            }

            std::cout << "Successfully replaced!\n";
        }

        else {
            std::cout << "Unknown mode!\n";
        }
        
    }

    void deleteText(int row, int position, int numChars) {
        const size_t initialSize = 256;

        if (row < 0 || row >= initialSize || position < 0 || position >= initialSize || numChars < 0) {
            std::cout << "Deletion cancelled: Invalid row, index, or length\n";
            return;
        }

        size_t textLength = strlen(currentText[row]);
        if (position + numChars > textLength) {
            std::cout << "Deletion cancelled: An amount of symbols to delete exceeds text length\n";
            return;
        }

        for (size_t i = position; i < textLength - numChars; ++i) {
            currentText[row][i] = currentText[row][i + numChars];
        }

        currentText[row][textLength - numChars] = '\0';

        std::cout << "Text deleted successfully.\n";
    }

    void undoCommand() {
        if (text.getUndo() < 0) {
            std::cout << "You cannot undo further.\n";
            return;
        }

        if (text.getRedo() + 1 >= text.getMaxStates()) {
            shiftRedoArray();
        }
        else {
            text.getRedo()++;
            allocateRedoArray();
        }

        copyCurrentToUndo();
        copyUndoToCurrent();
        text.getUndo()--;

        std::cout << "Undo was successful.\n";
    }

    void redoCommand() {
        if (text.getRedo() < 0) {
            std::cout << "You cannot redo further.\n";
            return;
        }

        if (text.getUndo() + 1 >= text.getMaxStates()) {
            shiftUndoArray();
        }
        else {
            text.getUndo()++;
            allocateUndoArray();
        }

        copyCurrentToRedo();
        copyRedoToCurrent();
        text.getRedo()--;

        std::cout << "Redo was successful.\n";
    }

    void copyText() {
        if (text.getUndo() + 1 >= text.getMaxStates()) {
            for (size_t i = 0; i < text.getInitialSize(); i++) {
                delete[] text.getUndoArray()[0][i];
            }
            delete[] text.getUndoArray()[0];

            for (int i = 1; i < text.getMaxStates(); i++) {
                text.getUndoArray()[i - 1] = text.getUndoArray()[i];
            }

            text.getUndo()--; 
        }

        text.getUndo()++; 

        text.getUndoArray()[text.getUndo()] = new char* [text.getInitialSize()];

        for (size_t i = 0; i < text.getInitialSize(); i++) {
            text.getUndoArray()[text.getUndo()][i] = new char[text.getInitialSize()];
            strcpy_s(text.getUndoArray()[text.getUndo()][i], text.getInitialSize(), currentText[i]);
        }
    }

    void copy(int row, int position, int numChars) {
        const size_t initialSize = 256;


        if (row < 0 || row >= initialSize) {
            std::cout << "Invalid row!!\n";
            return;
        }

        strncpy_s(copied, initialSize, currentText[row] + position, numChars);
        copied[numChars] = '\0';

        std::cout << "Copied: " << copied << "\n";
    }

    void paste() {
        if (isEmpty(copied)) {
            std::cout << "Oops. Nothing to paste!!\n";
            return;
        }

        const size_t initialSize = 256;
        int row, position;
        std::cout << "Enter a row: \n";
        std::cin >> row;
        std::cout << "Enter a position: \n";
        std::cin >> position;

        size_t length = strlen(currentText[row]);
        size_t copiedLength = strlen(copied);
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

        memmove(currentText[row] + position + copiedLength, currentText[row] + position, length - position + 1);
        memcpy(currentText[row] + position, copied, copiedLength);

        std::cout << "Pasted successfully!!\n";
        copied[0] = '\0';
    }

    bool isEmpty(const char* array) {
        return (array[0] == '\0');
    }

    void cut() {
        int row, position, numChars;

        std::cout << "Enter a row: ";
        std::cin >> row;
        std::cout << "Enter a position: ";
        std::cin >> position;
        std::cout << "Enter an amount of chars to cut (no more than 256): ";
        std::cin >> numChars;

        copy(row, position, numChars);
        deleteText(row, position, numChars);
    }

    void shiftUndoArray() {
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            delete[] text.getUndoArray()[0][i];
        }
        delete[] text.getUndoArray()[0];

        for (int i = 1; i < text.getMaxStates(); ++i) {
            text.getUndoArray()[i - 1] = text.getUndoArray()[i];
        }

        text.getUndoArray()[text.getMaxStates() - 1] = new char* [text.getInitialSize()];
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            text.getUndoArray()[text.getMaxStates() - 1][i] = new char[text.getInitialSize()];
        }
    }

    void clearText() {
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            currentText[i][0] = '\0';
        }
        std::cout << "Text cleared.\n";
    }

    void allocateUndoArray() {
        text.getUndoArray()[text.getUndo()] = new char* [text.getInitialSize()];
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            text.getUndoArray()[text.getUndo()][i] = new char[text.getInitialSize()];
        }
    }

    void copyCurrentToUndo() {
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            strcpy_s(text.getUndoArray()[text.getUndo()][i], text.getInitialSize(), currentText[i]);
        }
    }

    void copyUndoToCurrent() {
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            strcpy_s(currentText[i], text.getInitialSize(), text.getUndoArray()[text.getUndo()][i]);
        }
    }

    void shiftRedoArray() {
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            delete[] text.getRedoArray()[0][i];
        }
        delete[] text.getRedoArray()[0];

        for (int i = 1; i < text.getMaxStates(); ++i) {
            text.getRedoArray()[i - 1] = text.getRedoArray()[i];
        }

        text.getRedoArray()[text.getMaxStates() - 1] = new char* [text.getInitialSize()];
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            text.getRedoArray()[text.getMaxStates() - 1][i] = new char[text.getInitialSize()];
        }
    }

    void allocateRedoArray() {
        text.getRedoArray()[text.getRedo()] = new char* [text.getInitialSize()];
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            text.getRedoArray()[text.getRedo()][i] = new char[text.getInitialSize()];
        }
    }

    void copyCurrentToRedo() {
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            strcpy_s(text.getRedoArray()[text.getRedo()][i], text.getInitialSize(), currentText[i]);
        }
    }

    void copyRedoToCurrent() {
        for (size_t i = 0; i < text.getInitialSize(); ++i) {
            strcpy_s(currentText[i], text.getInitialSize(), text.getRedoArray()[text.getRedo()][i]);
        }
    }

    void encryptText() {

        size_t initialSize = 256;
        for (size_t i = 0; i < initialSize; i++) {

            if (currentText[i][0] != NULL) {
                currentText[i] = caesar.encrypt(currentText[i]);
            }
        }
    }

    void decryptText() {

        size_t initialSize = 256;
        for (size_t i = 0; i < initialSize; i++) {
            if (currentText[i][0] != NULL) {
                currentText[i] = caesar.decrypt(currentText[i]);
            }
        }
    }
};


int main() {
    const size_t initialSize = 256;
    TextEditor editor(initialSize);
    CaesarCipher caesar;

    editor.run();
    return 0;
}
 