#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class TextEditor {
public:
    TextEditor(size_t initialSize, size_t bufferSize)
        : initialSize(initialSize), bufferSize(bufferSize) {
        text.resize(initialSize, std::string());
        text[0] = "Hello World"; // default text
    }

    void run() {
        std::string userInput;

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
                print();
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
        COMMAND_APPEND = 0,
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

    std::vector<std::string> text;
    const size_t initialSize;
    const size_t bufferSize;
    const std::vector<std::string> commandsToStrings = { "append", "insert", "newline", "save", "load", "search", "help", "exit", "print" };

    int getCommand(const std::string& userInput) {
        for (size_t i = 0; i < commandsToStrings.size(); i++) {
            if (userInput == commandsToStrings[i]) {
                return static_cast<int>(i);
            }
        }
        return COMMAND_UNKNOWN;
    }

    void append() {
        std::string newText;
        std::cout << "Enter text to append: ";
        std::cin.ignore();
        std::getline(std::cin, newText);
        text[findEndOfText()] += newText;
    }

    void insert() {
        std::string newText, buffer;
        int row, index;

        std::cout << "Enter text to insert: ";
        std::cin.ignore();
        std::getline(std::cin, newText);
        std::cout << "Enter placement in row_index format (e.g., 8_12): ";
        std::getline(std::cin, buffer);

        std::stringstream ss(buffer);
        char sep;
        if (ss >> row >> sep >> index && sep == '_') {
            if (row >= 0 && static_cast<size_t>(row) < initialSize && index >= 0 && static_cast<size_t>(index) < bufferSize) {
                insertText(row, index, newText);
            }
            else {
                std::cout << "Insertion cancelled: Invalid row or index\n";
            }
        }
        else {
            std::cout << "Insertion cancelled: Invalid format\n";
        }
    }

    void newLine() {
        text[findEndOfText()] += "\n";
    }

    void save() {
        std::string filePath;
        std::cout << "Input full path to file: ";
        std::cin.ignore();
        std::getline(std::cin, filePath);

        std::ofstream file(filePath, std::ofstream::out | std::ofstream::trunc);
        if (file.is_open()) {
            for (const auto& line : text) {
                if (!line.empty()) {
                    file << line << "\n";
                }
            }
            file.close();
            std::cout << "Your text was successfully saved!\n";
        }
        else {
            std::cout << "Failed to open the file for writing.\n";
        }
    }

    void load() {
        std::string filePath;
        std::cout << "Input full path to file: ";
        std::cin.ignore();
        std::getline(std::cin, filePath);

        std::ifstream file(filePath);
        if (file.is_open()) {
            std::string line;
            size_t lineIndex = 0;

            while (lineIndex < initialSize && std::getline(file, line)) {
                text[lineIndex] = line;
                lineIndex++;
            }
            file.close();
            std::cout << "Text was successfully loaded!\n";
        }
        else {
            std::cout << "Failed to open the file for reading.\n";
        }
    }

    void search() {
        std::string substring;
        std::cout << "Enter the substring to search: ";
        std::cin.ignore();
        std::getline(std::cin, substring);

        bool found = false;
        for (size_t i = 0; i < text.size(); i++) {
            size_t pos = text[i].find(substring);
            if (pos != std::string::npos) {
                std::cout << "Found substring at row " << i << ", index " << pos << "\n";
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

    void print() {
        for (const auto& line : text) {
            if (!line.empty()) {
                std::cout << line << "\n";
            }
        }
    }

    size_t findEndOfText() {
        size_t endline = initialSize - 1;
        while (endline > 0 && text[endline].empty()) {
            endline--;
        }
        return endline;
    }

    void insertText(int row, int index, const std::string& newText) {
        std::string& line = text[row];
        if (index <= static_cast<int>(line.size())) {
            line.insert(index, newText);
            std::cout << "Text inserted successfully.\n";
        }
        else {
            std::cout << "Invalid character position. Please enter a number between 0 and " << line.size() << ".\n";
        }
    }
};

int main() {
    const size_t initialSize = 256;
    const size_t bufferSize = 50;
    TextEditor editor(initialSize, bufferSize);
    editor.run();
    return 0;
}
