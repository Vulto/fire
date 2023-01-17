#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void list_files(const char* path, char*** files, int* count) {
	DIR* dir = opendir(path);
	if (dir == NULL) {
		printf("Failed to open directory: %s\n", path);
		return;
	}

	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
		char full_path[1024];
		snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
		struct stat st;
		if (stat(full_path, &st) == 0) {
			// Check if it's a regular file
			if (S_ISREG(st.st_mode)) {
				// Dynamically allocate memory for the new file
				*files = (char**)realloc(*files, (*count + 1) * sizeof(char*));
				(*files)[*count] = strdup(full_path);
				(*count)++;
			}
			// Check if it's a directory
			else if (S_ISDIR(st.st_mode)) {
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
					list_files(full_path, files, count);
				}
			}
		}
	}

	closedir(dir);
}

int main(int argc, char* argv[]) {
	char* path;
	if (argc > 1) {
		path = argv[1];
	} else {
		path = getenv("HOME");
	}

	// Initialize variables
	char** files = NULL;
	int count = 0;

	// List files in the directory
	list_files(path, &files, &count);

	// Print the files
	for (int i = 0; i < count; i++) {
		printf("%s\n", files[i]);
	}

	// Free memory
	for (int i = 0; i < count; i++)
		free(files[i]);
	free(files);

	return 0;
}
