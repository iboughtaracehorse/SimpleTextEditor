#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Commands
{
	COMMAND_APPEND = 1,
	COMMAND_INSERT,
	COMMAND_NEW,
	COMMAND_SAVE,
	COMMAND_LOAD,
	COMMAND_SEARCH,
	COMMAND_HELP,
	COMMAND_EXIT,
	COMMAND_UNKNOWN
};

//void appendText(char** text, size_t initialSize, const char* newText);
void help();
int getCommand(char* userInput);

int main() {

	char userInput[20];
	const size_t initialSize = 100;
	char** text = (char**)malloc(initialSize * sizeof(char*));
	// second dimension 
	for (size_t i = 0; i < initialSize; i++) {
		text[i] = (char*)malloc(initialSize * sizeof(char));
		text[i][0] = '\0';
	}

	strcpy_s(text[0], initialSize, "Hello");
	strcpy_s(text[1], initialSize, "World");

	while (1)
	{
		printf("Enter a command or 'help' to see the list of available commands: ");
		scanf_s("%s", userInput, sizeof(userInput));

		int command = getCommand(userInput);

		switch (command)
		{
		case COMMAND_APPEND: {
			printf("Command is not available\n");
			break;
		}case COMMAND_INSERT: {
			printf("Command is not available\n");
			break;
		}case COMMAND_NEW: {
			printf("Command is not available\n");
			break;
		}case COMMAND_SAVE: {
			printf("Command is not available\n");
			break;
		}case COMMAND_LOAD: {
			printf("Command is not available\n");
			break;
		}case COMMAND_SEARCH: {
			printf("Command is not available\n");
			break;
		}case COMMAND_HELP: {
			printf("Command is not available\n");
			break;
		}case COMMAND_EXIT: {
			printf("Command is not available\n");
			break;
		}case COMMAND_UNKNOWN: {
			printf("AAAAAAAAAAAAAAAAAAAA\n");
			break;
		}
		default:
			printf("Unknown command. Please try again: ");
			break;
		}
	}

	// Free the allocated memory
	for (int i = 0; i < initialSize; i++)
		free(text[i]);
	free(text);

	return 0;
}

void help() {
	printf("Available commands:\n");
	printf("  append  -- Append text to the end of the text\n");
	printf("  insert  -- Insert text at a specific position\n");
	printf("  new     -- Start a new line\n\n");

	printf("  save    -- Save into the file\n");
	printf("  load    -- Load an external file\n\n");

	printf("  search  -- Search for a specific word in the text\n\n");

	printf("  help    -- Display all available commands\n");
	printf("  exit    -- Exit editor\n");
}

int getCommand(char* userInput)
{
	if (strcmp(userInput, "append") == 0) {
		return COMMAND_APPEND;
	}
	else if (strcmp(userInput, "insert") == 0) {
		return COMMAND_INSERT;
	}
	else if (strcmp(userInput, "new") == 0) {
		return COMMAND_NEW;
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


	else {
		return COMMAND_UNKNOWN;
	}
}

//void appendText(char* text, size_t initialSize, char* newText) {
//	strncat_s(text[1], initialSize, newText);
//}
