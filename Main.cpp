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

void appendText(char* text, size_t initialSize, const char* newText);
void help();
int getCommand(char* userInput);
void printAllText(char** text, size_t numLines);
size_t findTheEndOfTheText(char** text, size_t size);
void insertText(char* row, size_t size, int index, const char* newText);

int main() {

	char userInput[20]; // for commands only!
	const size_t initialSize = 100;
	const size_t bufferSize = 50;
	char** text = (char**)malloc(initialSize * sizeof(char*));
	// second dimension 
	for (size_t i = 0; i < initialSize; i++) {
		text[i] = (char*)malloc(initialSize * sizeof(char));
		text[i][0] = '\0';
	}

	strcpy_s(text[0], initialSize, "Hello");
	strcpy_s(text[1], initialSize, "   ");
	strcpy_s(text[2], initialSize, "World");
	strcpy_s(text[3], initialSize, "   ");


	while (true)
	{
		printf("Enter a command or 'help' to see the list of available commands: ");
		scanf_s("%s", userInput, sizeof(userInput)); // sizeof does not allow scanf_s to read more chars

		int command = getCommand(userInput);

		switch (command)
		{
			// to the end of the text
		case COMMAND_APPEND: {
			printf("Enter text to append: ");
			char newText[bufferSize];
			if (scanf_s("%s", newText, sizeof(newText)) != NULL)
			{
				size_t endline = findTheEndOfTheText(text, initialSize);
				appendText(text[endline], bufferSize, newText);
			}
			break;

			// to a chosen index
		}case COMMAND_INSERT: {
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
					if (row >= 0 && index < initialSize && index >= 0 && index < bufferSize)
					{
						insertText(text[row], bufferSize, index, newText);
					}
					else {
						printf("Insertion cancelled");
					}
				}
			}
			break;

			// appends a newline
		}case COMMAND_NEWLINE: {
			size_t endline = findTheEndOfTheText(text, initialSize);
			appendText(text[endline], bufferSize, "\n");
			break;

		}case COMMAND_SAVE: {
			const char* filePath = "C:\\Users\\dariy\\Documents\\GitHub\\ProgrammingParadigms1";
			
			FILE* file;
			errno_t targetFile = fopen_s(&file, "myfile.txt", "w+");
			if (file != NULL)
			{
				for (size_t i = 0; i < initialSize; ++i) {
					if (text[i][0] != '\0') {
						fprintf(file, "%s\n", text[i]);
					}
				}
				printf("Your text was successfully saved!");
			}
			break;

		}case COMMAND_LOAD: {
			const char* filePath = "C:\\Users\\dariy\\Documents\\GitHub\\ProgrammingParadigms1\\myfile.txt";
			FILE* file;
			errno_t targetFile = fopen_s(&file, filePath, "r");
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
			break;

		}case COMMAND_SEARCH: {
			printf("Command is not available\n");
			break;

			// prints help info to the console
		}case COMMAND_HELP: {
			help();
			break;

		}case COMMAND_EXIT: {
			printf("Exiting...\n");
			goto exit;
		}
		case COMMAND_PRINT: {
			printAllText(text, initialSize);
			break;
		}
		default:
			printf("Unknown command. Please try again: ");
			break;
		}
	}

exit:
	// Free the allocated memory
	for (int i = 0; i < initialSize; i++)
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

int getCommand(char* userInput)
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

void insertText(char* row, size_t size, int index, const char* newText)
{
	size_t newTextLen = strlen(newText);
	size_t rowLen = strlen(row);

	if (index < 0 || index > size)
	{
		printf("Invalid text position!");
		return; // no need to continue checks
	}

	memmove(row + index + newTextLen, row + index, rowLen - index + 1); // move previous text
	memcpy(row + index, newText, newTextLen);
}