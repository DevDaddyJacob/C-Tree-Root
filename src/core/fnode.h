#ifndef treeroot_core_fnode_h
#define treeroot_core_fnode_h

#include "../lib/common.h" /* Manditory before other imports */

typedef enum fnode_NodeType {
    FNODE_FILE,
    FNODE_FOLDER
} NodeType;


typedef struct fnode_FNode {
    /**
     * \brief           The type of the node
     */
    NodeType type;

    /**
     * \brief           The path to the node
     */
    char* path;

    /**
     * \brief           The name of the file or folder
     */
    char* name;

    /**
     * \brief           If the node is hidden
     */
    Bool isHidden;

    /**
     * \brief           If the node is a link
     */
    Bool isLink;

    /**
     * \brief           Pointer to the parent FNode
     */
    struct fnode_FNode* parent;

    /**
     * \brief           Array of FNode pointers to the child nodes
     */
    struct fnode_FNode** children;

    /**
     * \brief           The number of children currently stored
     */
    UInt16 childrenCount;

    /**
     * \brief           The max number of children that can be stored
     */
    UInt16 childrenCapacity;
} FNode;


FNode* fnode_new(const char* path, FNode* parent);


void fnode_destroy(FNode* fnode);


Bool fnode_addChild(FNode* parent, FNode* child);


void fnode_print(FNode* node, Int16 depth);


FNode* fnode_getNodeFromPath(const char* path, Int16 maxDepth);

#endif /* treeroot_core_fnode_h */