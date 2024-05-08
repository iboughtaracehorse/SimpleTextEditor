#include <stdio.h>
#include <stdlib.h>


int main() {
    char* input = 0;
    size_t bufferSize = 256; // Initial buffer size

    // Allocate memory for the input buffer
    input = (char*)malloc(bufferSize * sizeof(char));
    if (input == NULL) {
        printf("Memory allocation failed. Exiting...\n");
        return 1;
    }

    // Prompt the user to enter a string
    printf("Enter a string: ");
    // Read the string from the console
    fgets(input, bufferSize, stdin);

    // Print the string
    printf("You entered: %s", input);

    // Free dynamically allocated memory
    free(input);

    return 0;

}