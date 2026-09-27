#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENAME "journal.txt"
#define DELIM "###END###\n"

// This finds today's date, like looking at a calendar
void get_today(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 20, "%Y-%m-%d", tm_info);
}

// This lets you write a brand new diary page
void new_entry() {
    char date[20];
    get_today(date);

    char mood[30];
    printf("Mood (one word): ");
    scanf(" %29s", mood);
    getchar();

    printf("Write your entry (end with a single '.' on its own line):\n");

    FILE *f = fopen(FILENAME, "a");
    fprintf(f, "DATE:%s\nMOOD:%s\nTEXT:\n", date, mood);

    char line[500];
    while (fgets(line, sizeof(line), stdin)) {
        if (strcmp(line, ".\n") == 0) break;
        fprintf(f, "%s", line);
    }
    fprintf(f, DELIM);
    fclose(f);
    printf("Entry saved.\n");
}

// This shows you every page you've ever written
void view_entries() {
    FILE *f = fopen(FILENAME, "r");
    if (!f) { printf("No entries yet.\n"); return; }

    char line[500];
    int count = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "DATE:", 5) == 0) {
            count++;
            printf("\n[%d] %s", count, line);
        } else if (strncmp(line, DELIM, 9) != 0) {
            printf("%s", line);
        }
    }
    fclose(f);
}

// This looks for a word across all your pages, like a treasure hunt
void search_entries() {
    char keyword[100];
    printf("Search keyword: ");
    scanf(" %99[^\n]", keyword);

    FILE *f = fopen(FILENAME, "r");
    if (!f) { printf("No entries yet.\n"); return; }

    char buffer[2000] = "";
    char line[500];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, DELIM, 9) == 0) {
            if (strstr(buffer, keyword)) {
                printf("\n--- Match ---\n%s", buffer);
            }
            buffer[0] = '\0';
        } else {
            strcat(buffer, line);
        }
    }
    fclose(f);
}

// This tears out a page you don't want anymore
void delete_entry() {
    FILE *f = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!f) { printf("No entries yet.\n"); return; }

    int target;
    printf("Entry number to delete (run 'View' first to check numbers): ");
    scanf("%d", &target);

    char buffer[2000] = "";
    char line[500];
    int count = 0;
    while (fgets(line, sizeof(line), f)) {
        strcat(buffer, line);
        if (strncmp(line, DELIM, 9) == 0) {
            count++;
            if (count != target) fprintf(temp, "%s", buffer);
            buffer[0] = '\0';
        }
    }
    fclose(f);
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt", FILENAME);
    printf("Deleted entry %d.\n", target);
}

// This is the big menu you see when you open the app
int main() {
    int choice;
    do {
        printf("\n--- CLI Journal ---\n");
        printf("1. New Entry\n2. View Entries\n3. Search\n4. Delete Entry\n5. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: new_entry(); break;
            case 2: view_entries(); break;
            case 3: search_entries(); break;
            case 4: delete_entry(); break;
            case 5: printf("Bye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}