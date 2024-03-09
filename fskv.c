/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/main.c to edit this template
 */

/* 
 * File:   fskv.c
 * Author: mazenkapadi
 *
 * Created on March 9, 2024, 12:54 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define DATABASE_FILE "database.txt"

typedef struct {
    int key;
    char value[MAX_LINE_LENGTH];
} KeyValue;

// Function prototypes
void put(int key, char *value);
void get(int key);
void delete(int key);
void clear();
void all();
void loadDatabase();
void saveDatabase();
void handleCommand(char *command);

// Global variables
KeyValue *database = NULL;
int numEntries = 0;

int main(int argc, char *argv[]) {
    loadDatabase();

    // Process command-line arguments
    for (int i = 1; i < argc; i++) {
        handleCommand(argv[i]);
    }

    saveDatabase();
    free(database);

    return 0;
}



void put(int key, char *value) {
    // Check if key already exists
    for (int i = 0; i < numEntries; i++) {
        if (database[i].key == key) {
            printf("Key %d already exists. Updating value.\n", key);
            strcpy(database[i].value, value);
            return;
        }
    }

    // Add new entry to database
    database = realloc(database, (numEntries + 1) * sizeof(KeyValue));
    if (database == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    database[numEntries].key = key;
    strcpy(database[numEntries].value, value);
    numEntries++;
}

void get(int key) {
    for (int i = 0; i < numEntries; i++) {
        if (database[i].key == key) {
            printf("%d,%s\n", database[i].key, database[i].value);
            return;
        }
    }
    printf("K %d not found\n", key);
}

void delete(int key) {
    for (int i = 0; i < numEntries; i++) {
        if (database[i].key == key) {
            // Shift elements to overwrite the deleted entry
            for (int j = i; j < numEntries - 1; j++) {
                database[j] = database[j + 1];
            }
            numEntries--;
            return;
        }
    }
    printf("K %d not found\n", key);
}

void clear() {
    free(database);
    database = NULL;
    numEntries = 0;
}

void all() {
    for (int i = 0; i < numEntries; i++) {
        printf("%d,%s\n", database[i].key, database[i].value);
    }
}

void loadDatabase() {
    FILE *file = fopen(DATABASE_FILE, "r");
    if (file == NULL) {
        return;
    }
    int key;
    char value[MAX_LINE_LENGTH];
    while (fscanf(file, "%d,%s", &key, value) == 2) {
        put(key, value);
    }
    fclose(file);
}

void saveDatabase() {
    FILE *file = fopen(DATABASE_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to save database\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numEntries; i++) {
        fprintf(file, "%d,%s\n", database[i].key, database[i].value);
    }
    fclose(file);
}

void handleCommand(char *command) {
    char *token;
    token = strtok(command, ",\n");
    if (token == NULL) {
        printf("Bad command\n");
        return;
    }

    if (strcmp(token, "p") == 0) {
        int key = atoi(strtok(NULL, ","));
        char *value = strtok(NULL, ",");
        if (value == NULL) {
            printf("Bad command\n");
            return;
        }
        put(key, value);
    } else if (strcmp(token, "g") == 0) {
        int key = atoi(strtok(NULL, ","));
        get(key);
    } else if (strcmp(token, "d") == 0) {
        int key = atoi(strtok(NULL, ","));
        delete(key);
    } else if (strcmp(token, "c") == 0) {
        clear();
    } else if (strcmp(token, "a") == 0) {
        all();
    } else {
        printf("Bad command\n");
    }
}