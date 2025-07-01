#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../lib/common.h" /* Manditory before other imports */
#include "fnode.h"
#include "../lib/platform.h"

#ifdef OS_WINDOWS
    #include <windows.h>
#endif /* OS_WINDOWS */

#ifdef OS_POSIX_COMPLIANT
    #include <dirent.h>
#endif /* OS_POSIX_COMPLIANT */


/*
 * ==================================================
 * Macros
 * ==================================================
 */

/* #define XYZ "ABC" */

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

typedef struct fnode_StackItem {
    FNode* node;
    Int16 depth;
} StackItem;

typedef struct fnode_FNodeStack {
    StackItem** nodes;
    StackItem** top;
    Int16 capacity;
} FNodeStack;

static StackItem* stackItem_new(FNode* node, Int16 depth);

static FNodeStack* stack_new();

static void stack_destroy(FNodeStack* stack);

static Bool stack_isEmpty(FNodeStack* stack);

static void stack_push(FNodeStack* stack, FNode* node, Int16 depth);

static StackItem* stack_pop(FNodeStack* stack);


static void populateNode(FNode* parent, Int16 maxDepth);


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

static StackItem* stackItem_new(FNode* node, Int16 depth) {
    StackItem* item = NULL;
    
    if (node == NULL) return NULL;
    
    item = (StackItem*)malloc(sizeof(StackItem));
    if (item == NULL) return NULL;

    item->node = node;
    item->depth = depth;

    return item;
}

static FNodeStack* stack_new() {
    FNodeStack* stack = NULL;

    /* Allocate the stack and initialize it */
    stack = (FNodeStack*)malloc(sizeof(FNodeStack));
    if (stack == NULL) {
        stack_destroy(stack);
        return NULL;
    }

    stack->nodes = NULL;
    stack->top = NULL;
    stack->capacity = ARRAY_MIN_THRESHOLD;


    /* Allocate the inner list */
    stack->nodes = (StackItem**)malloc(sizeof(StackItem*) * stack->capacity);
    if (stack->nodes == NULL) {
        stack_destroy(stack);
        return NULL;
    }

    stack->top = stack->nodes;

    return stack;
}

static void stack_destroy(FNodeStack* stack) {
    if (stack == NULL) return;

    if (stack->nodes != NULL) {
        while (!stack_isEmpty(stack)) {
            free(stack_pop(stack));
        }

        free(stack->nodes);
    }

    free(stack);
}

static Bool stack_isEmpty(FNodeStack* stack) {
    return stack->top == stack->nodes;
}

static void stack_push(FNodeStack* stack, FNode* node, Int16 depth) {
    if ((stack->top - stack->nodes) + 1 > stack->capacity) {
        Int16 count = stack->top - stack->nodes;
        stack->capacity = stack->capacity * ARRAY_GROW_FACTOR;
        stack->nodes = realloc(stack->nodes, sizeof(StackItem*) * stack->capacity);
        stack->top = stack->nodes + count;
    }
    
    *stack->top = stackItem_new(node, depth);
    stack->top++;
}

static StackItem* stack_pop(FNodeStack* stack) {
    if (stack->top == stack->nodes) return NULL;

    return *(--stack->top);
}

static void populateNode(FNode* parent, Int16 maxDepth) {
    FNodeStack* stack = stack_new();
    FNode* childNode = NULL;

    if (parent == NULL) return;

    /* Prime the stack with the parent */
    stack_push(stack, parent, 0);


    /* Run the loop */
    while (!stack_isEmpty(stack)) {
        StackItem* item = stack_pop(stack);
        FNode* node = item->node;
        Int16 depth = item->depth;

#if defined(OS_WINDOWS)
        char path[MAX_PATH_LENGTH];
        char searchGlob[MAX_PATH_LENGTH];
        WIN32_FIND_DATA findData;
        HANDLE findHandle;

        if (depth >= maxDepth) continue;

        /* Loop the node if it's a folder */
        if (node->type == FNODE_FILE) return;


        /* Create search path glob and init the search operation */
        sprintf(searchGlob, "%s\\*", node->path);
        findHandle = FindFirstFile(searchGlob, &findData);
        if (findHandle == INVALID_HANDLE_VALUE) continue;


        /* Search the folder for items */
        do {
            /* Check if the path is an relative item */
            if (
                STR_EQ(findData.cFileName, ".")
                || STR_EQ(findData.cFileName, "..")
            ) continue;


            /* Concatenate the full path */
            _snprintf(
                path, 
                MAX_PATH_LENGTH, 
                "%s\\%s", 
                node->path, 
                findData.cFileName
            );


            /* Create the child node for the provided path */
            childNode = fnode_new(path, node);


            /* If the child node is a folder and we have depth left, call again */
            if (maxDepth == 1) continue;
            if (childNode->type == FNODE_FILE) continue;

            stack_push(stack, childNode, depth + 1);
        } while (FindNextFile(findHandle, &findData));

#elif defined(OS_POSIX_COMPLIANT)


#else
        #error Unsupported platform, \
            no compliant implementation of function 'populateNode'.
#endif
    }

    stack_destroy(stack);
}


FNode* fnode_new(const char* path, FNode* parent) {
    char nameBuff[MAX_PATH_LENGTH / 2];
    FNode* newNode = NULL;

    /* Allocate memory for the node */
    newNode = (FNode*)malloc(sizeof(FNode));
    if (newNode == NULL) {
        return NULL;
    }


    /* Initialize default values */
    newNode->type = (platform_isFile(path) ? FNODE_FILE : FNODE_FOLDER);
    newNode->path = NULL;
    newNode->name = NULL;
    newNode->isHidden = platform_isHidden(path);
    newNode->isLink = platform_isLink(path);
    newNode->parent = parent;
    newNode->children = NULL;
    newNode->childrenCount = 0;
    newNode->childrenCapacity = 0;

    
    /* Copy the string */
    newNode->path = (char*)malloc(sizeof(char) * strlen(path) + 1);
    if (newNode->path == NULL) {
        fnode_destroy(newNode);
        return NULL;
    }

    troot_safeStrCpy(newNode->path, path, strlen(path) + 1);

    
    /* Extract the file name */
    platform_extractFileName(path, nameBuff);
    newNode->name = (char*)malloc(sizeof(char) * strlen(nameBuff) + 1);
    if (newNode->name == NULL) {
        fnode_destroy(newNode);
        return NULL;
    }

    troot_safeStrCpy(newNode->name, nameBuff, strlen(nameBuff) + 1);


    /* Setup child array if the type is a folder */
    if (newNode->type == FNODE_FOLDER) {
        newNode->children = (FNode**)malloc(
            sizeof(FNode*) * ARRAY_MIN_THRESHOLD
        );
        if (newNode->children == NULL) {
            fnode_destroy(newNode);
            return NULL;
        }

        newNode->childrenCapacity = ARRAY_MIN_THRESHOLD;
    }


    /* Add this to the parent */
    if (parent != NULL) {
        fnode_addChild(parent, newNode);
    }


    return newNode;
}


void fnode_destroy(FNode* fnode) {
    if (fnode == NULL) return;

    if (fnode->path != NULL) {
        free(fnode->path);
        fnode->path = NULL;
    }

    if (fnode->name != NULL) {
        free(fnode->name);
        fnode->name = NULL;
    }

    if (fnode->children != NULL) {
        UInt16 i = 0;
        for (i = 0; i < fnode->childrenCount; i++) {
            free(fnode->children[i]);
            fnode->children[i] = NULL;
        }

        free(fnode->children);
        fnode->children = NULL;
    }

    free(fnode);
}


Bool fnode_addChild(FNode* parent, FNode* child) {
    if (parent == NULL) return FALSE;
    if (child == NULL) return FALSE;
    if (parent->children == NULL) return FALSE;

    if (parent->childrenCapacity < parent->childrenCount + 1) {
        parent->childrenCapacity = 
            parent->childrenCapacity < ARRAY_MIN_THRESHOLD
                ? ARRAY_MIN_THRESHOLD
                : parent->childrenCapacity * ARRAY_GROW_FACTOR;
            
        parent->children = realloc(
            parent->children,
            sizeof(FNode*) * parent->childrenCapacity
        );
    }

    parent->children[parent->childrenCount] = child;
    parent->childrenCount++;

    return TRUE;
}


void fnode_print(FNode* node, Int16 depth) {
    UInt16 i = 0;

    if (node == NULL) return;

    printf(
        "%*s %s\n",
        (depth == 0 ? 0 : depth * 2) + 2,
        depth == 0 ? " -" : "|-",
        node->name
    );

    for (i = 0; i < node->childrenCount; i++) {
        fnode_print(node->children[i], depth + 1);
    }
}


FNode* fnode_getNodeFromPath(const char* path, Int16 maxDepth) {
    char* normalizedPath = NULL;
    FNode* node = NULL;

    /* Copy the path to give us a working version of it */
    normalizedPath = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (normalizedPath == NULL) return NULL;
    troot_safeStrCpy(normalizedPath, path, strlen(path) + 1);
    

    /* Normalize the path */
    platform_normalizePath(normalizedPath);


    /* Make the root parent from the provided path */
    node = fnode_new(normalizedPath, NULL);
    free(normalizedPath);
    normalizedPath = NULL;

    if (node == NULL) return NULL;


    /* If we are going 0 layers deep, stop now */
    if (maxDepth == 0) {
        return node;
    }


    /* Check if this is the last depth */
    populateNode(node, maxDepth);

    return node;
}


/*void fnode_dump(FNode* node) {
    if (node == NULL) return;

    printf(
        "FNode(@ %p){" \
        "type: %d, " \
        "path(@ %p): \"%s\", " \
        "name(@ %p): \"%s\", " \
        "isHidden: %d, " \
        "isLink: %d, " \
        "parent(@ %p): [struct FNode], " \
        "children(@ %p): [struct FNode*], " \
        "childrenCount: %d, " \
        "childrenCapacity: %d" \
        "}\n",
        node,
        node->type,
        node->path, node->path,
        node->name, node->name,
        node->isHidden,
        node->isLink,
        node->parent,
        node->children,
        node->childrenCount,
        node->childrenCapacity
    );
}*/

