#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Commands
{
    COMMAND_APPEND,
    COMMAND_INSERT,
    COMMAND_NEWLINE,
    COMMAND_SAVE,
    COMMAND_LOAD,
    COMMAND_SEARCH,
    COMMAND_HELP,
    COMMAND_EXIT,
    COMMAND_UNKNOWN,
    COMMAND_PRINT
};

void appendText(char* text, size_t bufferSize, const char* newText);
void input(char** text, size_t size);
void help();
int getCommand(const char* userInput);
void printAllText(char** text, size_t numLines);
size_t findTheEndOfTheText(char** text, size_t size);
void insertText(char** text, char* row, size_t size, int index, const char* newText);
void searchSubstring(char** text, size_t numLines, const char* substring);
void saveToFile(char** text, size_t numLines);
void loadFromFile(char** text, size_t initialSize);
void search(char** text, size_t initialSize);

int main() {

    char userInput[20];
    const size_t initialSize = 256;
    const size_t bufferSize = 50;
    char** text = (char**)malloc(initialSize * sizeof(char*));
    for (size_t i = 0; i < initialSize; i++) {
        text[i] = (char*)malloc(initialSize * sizeof(char));
        text[i][0] = '\0';
    }

    strcpy_s(text[0], initialSize, "Hello World"); // default text

    /* C:\Users\dariy\Documents\GitHub\ProgrammingParadigms1\file.txt */

    while (true)
    {
        printf("Enter a command or 'help' to see the list of available commands: ");
        scanf_s("%s", userInput, sizeof(userInput)); 

        int command = getCommand(userInput);

        switch (command)
        {
        case COMMAND_APPEND: {
            input(text, initialSize);
            break;
        }
        case COMMAND_INSERT: {
            char buffer[initialSize];
            char newText[initialSize];

            printf("Enter text to insert: ");
            scanf_s("%s", newText, sizeof(newText));

            printf("Enter placement in row_index format(e.g., 8_12): ");
            int row, index;

            if (scanf_s("%s", buffer, sizeof(buffer)) != NULL)
            {
                if (sscanf_s(buffer, "%d_%d", &row, &index) == 2)
                {

                    if (row >= 0 && index < initialSize && index >= 0 && index < initialSize)
                    {
                        insertText(text, text[row], initialSize, index, newText);
                    }
                    else {
                        printf("Insertion cancelled");
                    }
                }
            }
            break;
        }
        case COMMAND_NEWLINE: {
            size_t endline = findTheEndOfTheText(text, initialSize);
            appendText(text[endline], bufferSize, "\n");
            break;
        }
        case COMMAND_SAVE: {
            saveToFile(text, initialSize);
            break;
        }
        case COMMAND_LOAD: {
            loadFromFile(text, initialSize);
            break;
        }
        case COMMAND_SEARCH: {
           
            search(text, initialSize);
            break;
        }
        case COMMAND_HELP: {
            help();
            break;
        }
        case COMMAND_EXIT: {
            printf("Exiting...\n");
            goto exit;
        }
        case COMMAND_PRINT: {
            printAllText(text, initialSize);
            break;
        }
        default:
            printf("Unknown command. Please try again.\n");
            break;
        }
    }

exit:
    // Free the allocated memory
    for (size_t i = 0; i < initialSize; i++)
        free(text[i]);
    free(text);

    return 0;
}

void help() {
    printf("\nAvailable commands:\n");
    printf("  append   -- Append text to the end of the text\n");
    printf("  insert   -- Insert text at a specific position\n");
    printf("  newline  -- Start a new line\n\n");

    printf("  save     -- Save into the file\n");
    printf("  load     -- Load an external file\n\n");
    printf("  print    -- Print all text to the console\n\n");

    printf("  search   -- Search for a specific word in the text\n\n");

    printf("  help     -- Display all available commands\n");
    printf("  exit     -- Exit editor\n\n");
}

int getCommand(const char* userInput)
{
    if (strcmp(userInput, "append") == 0) {
        return COMMAND_APPEND;
    }
    else if (strcmp(userInput, "insert") == 0) {
        return COMMAND_INSERT;
    }
    else if (strcmp(userInput, "newline") == 0) {
        return COMMAND_NEWLINE;
    }
    else if (strcmp(userInput, "save") == 0) {
        return COMMAND_SAVE;
    }
    else if (strcmp(userInput, "load") == 0) {
        return COMMAND_LOAD;
    }
    else if (strcmp(userInput, "search") == 0) {
        return COMMAND_SEARCH;
    }
    else if (strcmp(userInput, "help") == 0) {
        return COMMAND_HELP;
    }
    else if (strcmp(userInput, "exit") == 0) {
        return COMMAND_EXIT;
    }
    else if (strcmp(userInput, "print") == 0) {
        return COMMAND_PRINT;
    }
    else {
        return COMMAND_UNKNOWN;
    }
}

void appendText(char* text, size_t initialSize, const char* newText) {
    strcat_s(text, initialSize, newText);
}

void input(char** text, size_t size) {
    const size_t bufferSize = 50;
    printf("Enter text to append: ");
    char newText[bufferSize];
    if (scanf_s(" %[^\n]", newText, sizeof(newText)) != NULL)
    {
        size_t endline = findTheEndOfTheText(text, size);
        appendText(text[endline], bufferSize, newText);
    }
}

void printAllText(char** text, size_t numLines) {
    for (size_t i = 0; i < numLines; ++i) {
        if (text[i][0] != '\0') {
            printf("%s\n", text[i]);
        }
    }
}

size_t findTheEndOfTheText(char** text, size_t size)
{
    size_t endline = size - 1; // for /0
    while (endline > 0 && text[endline][0] == '\0')
    {
        endline--;
    }

    return endline;
}


void insertText(char** text, char* row, size_t size, int index, const char* newText)
{
    size_t newTextLen = strlen(newText);
    size_t rowLen = strlen(row);
    char* initialRow = 0;
    char* initialIndex = 0;
    bool userRow = false;

    if (index < 0 || index > rowLen)
    {
        printf("Invalid text position!");
        return; // no need to continue checks
    }

    for (char* i = *text; i; i++)
    {
        if (i == row)
        {
            userRow = true;
        }
    }

    printf("FUNCTION               ");

    printf("%B", userRow);

}

void searchSubstring(char** text, size_t numLines, const char* substring) {
    int found = false;
    size_t substringLen = strlen(substring);
    printf("Searching for substring: \"%s\"...\n", substring);

    for (size_t i = 0; i < numLines; i++) {
        char* row = text[i];

        if (row[0] == '\0')  
        {
            continue;
        }
        size_t rowLen = strlen(row);

        for (size_t j = 0; j <= rowLen - substringLen; j++) {
            if (strncmp(&row[j], substring, substringLen) == 0) {
                printf("Found substring at row %zu, index %zu\n", i, j);
                found = true;
            }
        }
    }

    if (!found)
    {
        printf("No such substring in your text!\n");
    }
}

void saveToFile(char** text, size_t numLines)
{
    const size_t bufferSize = 256;

    char filePath[bufferSize];
    printf("Input full path to file: ");
    if (scanf_s(" %[^\n]", filePath, sizeof(filePath)) == 1)
    {
        FILE* file;
        errno_t err = fopen_s(&file, filePath, "w+");
        if (file != NULL)
        {
            for (size_t i = 0; i < numLines; ++i) {
                if (text[i][0] != '\0') {
                    fprintf(file, "%s\n", text[i]);
                }
            }
            fclose(file);
            printf("Your text was successfully saved!\n");
        }
        else {
            printf("Failed to open the file for writing.\n");
        }
    }
    else {
        printf("Invalid file path.\n");
    }
}

void loadFromFile(char** text, size_t initialSize)
{
    const size_t bufferSize = 256;
    char filePath[bufferSize];
    printf("Input full path to file: ");
    if (scanf_s(" %[^\n]", filePath, (unsigned)_countof(filePath)) == 1)
    {
        FILE* file;
        errno_t err = fopen_s(&file, filePath, "r");
        if (file != NULL) {
            size_t lineIndex = 0; // line counter

            while (lineIndex < initialSize && fgets(text[lineIndex], bufferSize, file) != NULL) {
                size_t len = strlen(text[lineIndex]);

                // puts that '\0' at the end of the text
                if (len > 0 && text[lineIndex][len - 1] == '\n') {
                    text[lineIndex][len - 1] = '\0';
                }
                lineIndex++;
            }

            fclose(file);
            printf("Text was successfully loaded!\n");
        }
        else {
            printf("Failed to open the file for reading.\n");
        }
    }
    else {
        printf("Invalid file path.\n");
    }
}

void search(char** text, size_t initialSize)
{
    const size_t bufferSize = 50;
    printf("Enter the substring to search: ");
    char substring[bufferSize];

    int c;
    while ((c = getchar()) != '\n' && c != EOF); // clears the input buffer

    if (fgets(substring, sizeof(substring), stdin) != NULL) {
        substring[strcspn(substring, "\n")] = '\0'; // trim
        searchSubstring(text, initialSize, substring);
    }
    else {
        printf("Invalid substring! Try again.\n");
    }
}
