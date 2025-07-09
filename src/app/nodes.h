#ifndef TREEROOT_SRC_APP_NODES_H
#define TREEROOT_SRC_APP_NODES_H

#include "../config.h" /* Manditory in all files */


typedef enum treeRoot_NodeType {
    FNODE_FILE,
    FNODE_FOLDER
} NodeType;


typedef struct treeRoot_FNode {
    /**
     * \brief           The type of the node
     */
    NodeType type;

    /**
     * \brief           The path to the node
     */
    char* path;

    /**
     * \brief           The hash of the node's path
     */
    unsigned int pathHash;

    /**
     * \brief           The name of the file or folder
     */
    char* name;

    /**
     * \brief           If the node is hidden
     */
    byte_t isHidden;

    /**
     * \brief           If the node is a link
     */
    byte_t isLink;

    /**
     * \brief           Pointer to the parent FNode
     */
    struct treeRoot_FNode* parent;

    /**
     * \brief           Array of FNode pointers to the child nodes
     */
    struct treeRoot_FNode** children;

    /**
     * \brief           The number of children currently stored
     */
    uint32_t childrenCount;

    /**
     * \brief           The max number of children that can be stored
     */
    uint32_t childrenCapacity;
} FNode;



FNode* fnode_new(const char* path, FNode* parent);


void fnode_destroy(FNode* fnode);


byte_t fnode_addChild(FNode* parent, FNode* child);


void fnode_print(FNode* node, int32_t depth);


FNode* fnode_getNodeFromPath(const char* path, int32_t maxDepth);


#endif /* TREEROOT_SRC_APP_NODES_H */