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
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

// Function to get all file names in the current directory
char** get_files_in_current_dir(size_t *count) {
    DIR *dir;
    struct dirent *entry;
    size_t capacity = 10; // Initial capacity for the array
    size_t size = 0;      // Current number of files
    char **files = malloc(capacity * sizeof(char *));

    if (!files) {
        perror("Failed to allocate memory");
        return NULL;
    }

    // Open the current directory
    dir = opendir(".");
    if (!dir) {
        perror("Failed to open directory");
        free(files);
        return NULL;
    }

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Resize the array if necessary
        if (size >= capacity) {
            capacity *= 2;
            char **new_files = realloc(files, capacity * sizeof(char *));
            if (!new_files) {
                perror("Failed to reallocate memory");
                closedir(dir);
                for (size_t i = 0; i < size; i++) {
                    free(files[i]);
                }
                free(files);
                return NULL;
            }
            files = new_files;
        }

        // Allocate memory for the file name and copy it
        files[size] = strdup(entry->d_name);
        if (!files[size]) {
            perror("Failed to allocate memory for file name");
            closedir(dir);
            for (size_t i = 0; i < size; i++) {
                free(files[i]);
            }
            free(files);
            return NULL;
        }

        size++;
    }

    // Close the directory
    closedir(dir);

    // Resize the array to the exact number of files
    char **result = realloc(files, size * sizeof(char *));
    if (!result && size > 0) {
        perror("Failed to reallocate memory");
        for (size_t i = 0; i < size; i++) {
            free(files[i]);
        }
        free(files);
        return NULL;
    }

    *count = size;
    return result;
}

int main() {
    size_t count;
    char **files = get_files_in_current_dir(&count);

    if (!files) {
        fprintf(stderr, "Failed to get files in the current directory\n");
        return 1;
    }

    // Print the file names
    printf("Files in the current directory:\n");
    for (size_t i = 0; i < count; i++) {
        printf("\n ============================= \n %s\n ============================= \n", files[i]);
        char *content = read_file_content(files[i]);
        if (content) {
            printf("\n%s\n", content);
            free(content);
        }

    }

    // Free the allocated memory
    for (size_t i = 0; i < count; i++) {
        free(files[i]);
    }
    free(files);

    return 0;
}