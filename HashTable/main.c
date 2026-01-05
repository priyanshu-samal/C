#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 250
#define TABLE_SIZE 100

typedef struct {
    char name[MAX_NAME_LENGTH];
    int age;
} Person;

typedef struct {
    Person data[TABLE_SIZE];
    bool occupied[TABLE_SIZE];
} HashTable;

// Simple hash function (djb2)
unsigned int hash(const char *name) {
    unsigned long hash = 5381;
    int c;

    while ((c = *name++))
        hash = ((hash << 5) + hash) + c;

    return hash % TABLE_SIZE;
}

// Initialize hash table
void ht_init(HashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++)
        ht->occupied[i] = false;
}

// Insert or update
bool ht_insert(HashTable *ht, const char *name, int age) {
    unsigned int index = hash(name);

    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned int probe = (index + i) % TABLE_SIZE;

        if (!ht->occupied[probe]) {
            strcpy(ht->data[probe].name, name);
            ht->data[probe].age = age;
            ht->occupied[probe] = true;
            return true;
        }

        if (strcmp(ht->data[probe].name, name) == 0) {
            ht->data[probe].age = age; // update
            return true;
        }
    }

    return false; // Table full
}

// Retrieve
bool ht_get(HashTable *ht, const char *name, Person *out) {
    unsigned int index = hash(name);

    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned int probe = (index + i) % TABLE_SIZE;

        if (!ht->occupied[probe])
            return false;

        if (strcmp(ht->data[probe].name, name) == 0) {
            *out = ht->data[probe];
            return true;
        }
    }

    return false;
}

// Print all entries
void ht_print(HashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (ht->occupied[i]) {
            printf("[%d] %s : %d\n",
                   i,
                   ht->data[i].name,
                   ht->data[i].age);
        }
    }
}

int main() {
    HashTable ht;
    ht_init(&ht);

    ht_insert(&ht, "Priyanshu", 25);
    ht_insert(&ht, "vergil", 30);
    ht_insert(&ht, "goku", 22);
    ht_insert(&ht, "Priyanshu", 40); // update

    Person p;
    if (ht_get(&ht, "Priyanshu", &p))
        printf("Found Priyanshu, age = %d\n", p.age);
    else
        printf("Priyanshu not found\n");

    printf("\nFull table contents:\n");
    ht_print(&ht);

    return 0;
}
