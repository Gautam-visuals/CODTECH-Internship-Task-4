#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT_LENGTH 1000
#define WORD_LENGTH 50

// Function prototypes
void analyzeText(const char text[]);
void findAndReplace(char text[], const char target[], const char replacement[]);

int main() {
    char text[MAX_TEXT_LENGTH];
    char targetWord[WORD_LENGTH];
    char replacementWord[WORD_LENGTH];

    printf("=== Interactive Text Analysis & Corrector ===\n");
    printf("Enter your text paragraph below:\n");
    
    // Read the main paragraph safely
    if (fgets(text, MAX_TEXT_LENGTH, stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    text[strcspn(text, "\n")] = '\0'; // Remove trailing newline

    // Run the text analyzer
    analyzeText(text);

    // Get the word to find and replace from the user
    printf("\nEnter the word you want to find: ");
    scanf("%s", targetWord);

    printf("Enter the word to replace it with: ");
    scanf("%s", replacementWord);

    // Perform the correction
    findAndReplace(text, targetWord, replacementWord);

    return 0;
}

// Function to calculate text statistics
void analyzeText(const char text[]) {
    int characters = 0, words = 0, sentences = 0;
    int inWord = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        characters++;

        if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            sentences++;
        }

        if (isspace((unsigned char)text[i])) {
            inWord = 0;
        } else if (inWord == 0) {
            inWord = 1;
            words++;
        }
    }

    if (characters > 0 && sentences == 0) {
        sentences = 1;
    }

    printf("\n--- Text Analysis ---\n");
    printf("Characters: %d | Words: %d | Sentences: %d\n", characters, words, sentences);
}

// Function to find any user-defined word and replace it safely
void findAndReplace(char text[], const char target[], const char replacement[]) {
    char buffer[MAX_TEXT_LENGTH] = "";
    char *token = text;
    char *pos;
    int targetLen = strlen(target);
    int replacementsCount = 0;

    // Loop through the text to find occurrences of the target word
    while ((pos = strstr(token, target)) != NULL) {
        
        // Ensure it matches as a whole standalone word, not a substring
        // Check if it is the start of the text or preceded by a space/punctuation
        int isWordStart = (pos == text || isspace((unsigned char)*(pos - 1)) || ispunct((unsigned char)*(pos - 1)));
        
        // Check if it is the end of the text or followed by a space/punctuation
        int isWordEnd = (*(pos + targetLen) == '\0' || isspace((unsigned char)*(pos + targetLen)) || ispunct((unsigned char)*(pos + targetLen)));

        if (isWordStart && isWordEnd) {
            // 1. Copy everything from the current token up to the found word
            strncat(buffer, token, pos - token);
            
            // 2. Append the new replacement word
            strcat(buffer, replacement);
            
            // 3. Slide our tracking pointer past the old target word
            token = pos + targetLen;
            replacementsCount++;
        } else {
            // It was a substring match (e.g., finding "cat" inside "category"). Skip past this match.
            strncat(buffer, token, (pos - token) + 1);
            token = pos + 1;
        }
    }
    
    // Append any remaining text left after the final match
    strcat(buffer, token);
    
    // Save the corrected string back into the original text array
    strcpy(text, buffer);

    printf("\n--- Correction Summary ---\n");
    printf("Successfully replaced %d occurrence(s) of '%s' with '%s'.\n", replacementsCount, target, replacement);
    printf("\nUpdated Text:\n%s\n", text);
}
