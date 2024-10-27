#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

// Function to hash the given word and return its MD5 hash
char* TryWord(char *word) 
{
    return md5(word, strlen(word));
}

int main(int argc, char *argv[]) 
{
    // Check if the correct number of command-line arguments is provided
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <hash file> <dictionary file>\n", argv[0]);
        return 1;
    }

    // Open the hash file for reading
    FILE *hashFile = fopen(argv[1], "r");
    if (!hashFile) 
    {
        perror("Error opening hash file");
        return 1;
    }

    // Open the dictionary file for reading
    FILE *dictFile = fopen(argv[2], "r");
    if (!dictFile) 
    {
        perror("Error opening dictionary file");
        fclose(hashFile);
        return 1;
    }

    // MD5 hashes are 32 characters + null terminator
    char hash[33];
    // Buffer to hold words from the dictionary
    char word[100];
    
    // Read each hash from the hash file
    while (fscanf(hashFile, "%32s", hash) != EOF) 
    {
        // Reset the dictionary file for each hash
        rewind(dictFile);
        
        // Read each word from the dictionary
        while (fgets(word, sizeof(word), dictFile)) 
        {
            // Remove the newline character from the word, if present
            word[strcspn(word, "\n")] = '\0';
            
            // Hash the word and compare it with the current hash
            char *generatedHash = TryWord(word);
            if (strcmp(generatedHash, hash) == 0) 
            {
                // Print the hash and the matching word
                printf("%s %s\n", hash, word);
                free(generatedHash);
                break;
            }

            // Free the allocated memory after each hash generation
            free(generatedHash);
        }
    }

    // Close the files
    fclose(hashFile);
    fclose(dictFile);

    return 0;
}
