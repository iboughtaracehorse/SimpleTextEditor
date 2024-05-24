#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Commands
{
	COMMAND_APPEND = 1,
	COMMAND_INSERT = 2,
	COMMAND_HELP
};

void appendText(char** text, size_t initialSize, const char* newText);
void help();

int main() {

	char userInput[7];
	const size_t initialSize = 100;
	char** text = (char**)malloc(initialSize * sizeof(char*));
	// second dimension 
	for (size_t i = 0; i < initialSize; i++) {
		text[i] = (char*)malloc(initialSize * sizeof(char));
		text[i][0] = '\0';  
	}

	strcpy_s(text[0], initialSize, "Hello");
	strcpy_s(text[1], initialSize, "World");

	while (true)
	{
		printf("Enter a command or 'help' to see the list of available comands: ");
		scanf_s("%s", userInput);
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

//void appendText(char* text, size_t initialSize, char* newText) {
//	strncat_s(text[1], initialSize, newText);
//}

