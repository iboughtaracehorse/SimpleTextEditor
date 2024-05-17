#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	char userinput[20];
	printf("hello. enter you string?\n");
	fgets(userinput, 20, stdin);
	printf("your string: %s\n", userinput);

	const size_t initialSize = 256;
	char localText[initialSize][initialSize];

	char** text = (char**)malloc(initialSize * sizeof(char*));
	for (int i = 0; i < 4; i++)
		text[i] = (char*)malloc(initialSize * sizeof(char));

	strcpy_s(text[0], initialSize, "Hello");
	strcpy_s(text[1], initialSize, "World");

	text[1][0] = '!';

	printf("%s, %s\n", text[0], text[1]);

    // Free the allocated memory
    for (int i = 0; i < 2; i++)
        free(text[i]);
    free(text);

	return 0;
}
