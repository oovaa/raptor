#include <stdio.h>
#include <stdlib.h>

// Function to read the entire file content into a buffer
char* read_file_content(const char *filename) {
    FILE *file;
    char *buffer = NULL;
    size_t buffer_size = 0;
    size_t total_read = 0;
    size_t chunk_size = 1024; // Read in chunks of 1KB

    // Open the file in binary mode
    file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    // Dynamically allocate and read the file
    while (1) {
        // Resize the buffer to accommodate the next chunk
        buffer = realloc(buffer, total_read + chunk_size);
        if (!buffer) {
            perror("Failed to allocate memory");
            fclose(file);
            return NULL;
        }

        // Read a chunk of data from the file
        size_t bytes_read = fread(buffer + total_read, 1, chunk_size, file);
        if (bytes_read == 0) {
            // End of file or error
            if (feof(file)) {
                break; // End of file reached
            } else {
                perror("Error reading file");
                free(buffer);
                fclose(file);
                return NULL;
            }
        }

        total_read += bytes_read;
    }

    // Resize the buffer to the exact size of the file content
    buffer = realloc(buffer, total_read + 1); // +1 for null-terminator
    if (!buffer) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    // Null-terminate the buffer (optional, for text files)
    buffer[total_read] = '\0';

    // Close the file
    fclose(file);

    return buffer;
}

int main() {
    const char *filename = "story.txt";

    // Read the file content
    char *file_content = read_file_content(filename);
    if (!file_content) {
        fprintf(stderr, "Failed to read file content\n");
        return 1;
    }

    // Print the file content (for demonstration)
    printf("File content:\n%s\n", file_content);

    // Free the allocated buffer
    free(file_content);

    return 0;
}