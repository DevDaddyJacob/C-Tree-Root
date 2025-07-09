#include <stdlib.h>
#include <string.h>

#include "../config.h" /* Manditory in all files */

#include "table.h"
#include "strings.h"


/*
 * ==================================================
 * Macros
 * ==================================================
 */


/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

static TableEntry* findEntry(
    TableEntry* entries, 
    int32_t capacity, 
    const char* key
);


static void resizeTable(HashTable* table, int32_t capacity);


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

/* int magicNumber = 420; */


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static TableEntry* findEntry(
    TableEntry* entries, 
    int32_t capacity, 
    const char* key
) {
    uint64_t index = str_hash64((const char*)key) & (capacity - 1);
    TableEntry* tombstone = NULL;

    for (;;) {
        TableEntry* entry = &entries[index];

        if (entry->key == NULL) {
            if (T_ITEM_IS_EMPTY(entry->value)) {
                return tombstone != NULL ? tombstone : entry;
            }

            if (T_ITEM_IS_TOMBSTONE(entry->value)) {
                if (tombstone == NULL) tombstone = entry;
            }
        }

        if (entry->key != NULL && str_equals(entry->key, key, strlen(key))) {
            return entry;
        }

        index = (index + 1) & (capacity - 1);
    }
}


static void resizeTable(HashTable* table, int32_t capacity) {
    /* Validate the parameters */
    if (table == NULL) return;


    /* Reallocate the entries array */
    TableEntry* entries = (TableEntry*)malloc(sizeof(TableEntry) * capacity);
    for (int32_t i = 0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].value = T_ITEM_EMPTY;
    }


    /* Re-hash the entries */
    table->count = 0;
    for (int32_t i = 0; i < table->capacity; i++) {
        TableEntry* entry = &table->entries[i];
        if (entry->key == NULL) continue;

        TableEntry* dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;

        table->count++;
    }


    /* Release the old array and redefine to the new one */
    free(table->entries);
    table->entries = entries;
    table->capacity = capacity;
}


HashTable* hashTable_new(TableItemType type) {
    /* Validate the input */
    if (type == TBL_ITEM_EMPTY || type == TBL_ITEM_TOMBSTONE) return NULL;


    /* Attempt to allocate the table in memory */
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (table == NULL) return NULL;


    /* Initialize the values */
    table->type = type;
    table->capacity = 0;
    table->count = 0;
    table->entries = NULL;

    return table;
}


void hashTable_destroy(HashTable* table) {
    if (table == NULL) return;

    if (table->entries != NULL) {
        for (int32_t i = 0; i < table->capacity; i++) {
            TableEntry* entry = &table->entries[i];

            if (entry->key != NULL) {
                free(entry->key);
            }

            free(entry);
        }

        free(table->entries);
    }

    free(table);
}


byte_t hashTable_tryGet(
    HashTable* table, 
    const char* key, 
    TableItem* outValue
) {
    /* Validate the input */
    if (table == NULL || key == NULL) return 0;


    /* Check the table entry count */
    if (table->count <= 0) return 0;


    /* Try to find an antry for the given key */
    TableEntry* entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL) return 0;


    /* Assign the output argument */
    if (outValue != NULL) *outValue = entry->value;

    return 1;
}


byte_t hashTable_trySet(HashTable* table, const char* key, TableItem value) {
    /* Validate input */
    if (table == NULL || key == NULL || value.type != table->type) return 0;


    /* Ensure the table has room */
    if (table->count + 1 > table->capacity * HASH_TABLE_MAX_LOAD) {
        int32_t capacity = (table->capacity < HASH_TABLE_MIN_THRESHOLD)
            ? HASH_TABLE_MIN_THRESHOLD 
            : table->capacity * HASH_TABLE_GROW_FACTOR;
        
        resizeTable(table, capacity);
    }


    /* Check if it is a new key */
    TableEntry* entry = findEntry(table->entries, table->capacity, key);
    byte_t isNewKey = entry->key == NULL;


    /* Increment only if the bucket is truly empty */
    if (isNewKey && T_ITEM_IS_EMPTY(entry->value)) table->count++;


    /* Set the key and value */
    entry->key = str_clone(key, strlen(key) + 1);
    entry->value = value;

    return 1;
}


byte_t hashTable_tryRemove(HashTable* table, const char* key) {
    /* Validate input */
    if (table == NULL || key == NULL) return 0;


    /* Try to find the corresponding entry */
    TableEntry* entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL) return 0;


    /* Fill the value with a tombstone */
    entry->key = NULL;
    entry->value = T_ITEM_TOMBSTONE;

    return 1;
}
