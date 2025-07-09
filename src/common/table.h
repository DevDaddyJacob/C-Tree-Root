#ifndef TREEROOT_SRC_COMMON_TABLE_H
#define TREEROOT_SRC_COMMON_TABLE_H

#include <stdlib.h>

#include "../config.h" /* Manditory in all files */



#define _T_ITEM_IS(_value, _type) ((_value).type == _type)
#define _T_ITEM_AS(_value, field) ((_value).value.field)
#define _T_ITEM_CAST(_type, field, _value) \
    ((TableItem){ _type, { .field = _value } })

    
#define T_ITEM_IS_TOMBSTONE(_value) _T_ITEM_IS(_value, TBL_ITEM_TOMBSTONE)
#define T_ITEM_IS_EMPTY(_value)     _T_ITEM_IS(_value, TBL_ITEM_EMPTY)
#define T_ITEM_IS_BIGINT(_value)    _T_ITEM_IS(_value, TBL_ITEM_UINT64)


#define T_ITEM_AS_TOMBSTONE(_value) _T_ITEM_AS(_value, dummy)
#define T_ITEM_AS_EMPTY(_value)     _T_ITEM_AS(_value, dummy)
#define T_ITEM_AS_BIGINT(_value)    _T_ITEM_AS(_value, bigint)


#define T_ITEM_TOMBSTONE        _T_ITEM_CAST(TBL_ITEM_TOMBSTONE, dummy, 0x1)
#define T_ITEM_EMPTY            _T_ITEM_CAST(TBL_ITEM_EMPTY, dummy, 0x2)
#define T_ITEM_BIGINT(_value)   _T_ITEM_CAST(TBL_ITEM_UINT64, bigint, _value)



typedef enum treeRoot_TableItemType {
    TBL_ITEM_TOMBSTONE,
    TBL_ITEM_EMPTY,
    TBL_ITEM_UINT64
} TableItemType;

typedef struct treeRoot_TableItem {
    TableItemType type;
    union {
        byte_t dummy;
        uint64_t bigint;
    } value;
} TableItem;


typedef struct treeRoot_TableEntry {
    char* key;
    TableItem value;
} TableEntry;

typedef struct treeRoot_HashTable {
    TableItemType type;
    int32_t capacity;
    int32_t count;
    TableEntry* entries;
} HashTable;



HashTable* hashTable_new(TableItemType type);


void hashTable_destroy(HashTable* table);


byte_t hashTable_tryGet(
    HashTable* table,
    const char* key, 
    TableItem* outValue
);


byte_t hashTable_trySet(HashTable* table, const char* key, TableItem value);


byte_t hashTable_tryRemove(HashTable* table, const char* key);

#endif  /* TREEROOT_SRC_COMMON_TABLE_H */