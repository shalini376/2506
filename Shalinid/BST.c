// On my honor:
//
// - I have not discussed the C language code in my program with
// anyone other than my instructor or the teaching assistants
// assigned to this course.
//
// - I have not used C language code obtained from another student,
// the Internet, or any other unauthorized source, either modified
// or unmodified.
//
// - If any C language code or documentation used in my program
// was obtained from an authorized source, such as a text book or
// course notes, that has been clearly noted with a proper citation
// in the comments of my program.
//
// - I have not designed this program in such a way as to defeat or
// interfere with the normal operation of the grading code.
// 
// Shalini Dubey 
// Shalinid

//BST.c
#include "BST.h"
#include <stdlib.h>
#include <stdio.h>

// Declare static functions here, as needed:

static void BST_displayHelper(FILE* fp, const BSTNode* pNode, uint32_t level,
                              void (*displayFn)(FILE* fp, const BSTNode* const pNode));
static BSTNode* BST_findHelper(const BSTNode* pNode, const BSTNode* const userNode, BST* pTree);

static bool BST_insertHelper(BST* const pTree, BSTNode* const userNode, BSTNode** pNode);

static void BST_destroyHelper(BST*  pTree, BSTNode* pNode);


static void writeIndentation(FILE* fp, uint32_t level);

/** Create a proper, empty binary search tree object.
 * 
 *  Pre: compare is the name of a user-defined function satisfying the BST specification
 *       display is the name of a user-defined function satisfying the BST specification
 *       destroy is the name of a user-defined function satisfying the BST specification
 * 
 *  Returns: a BST object with NULL root and configured to use the two user-supplied
 *           functions for comparings and printing user-defined data objects stored
 *           in the tree
 */
BST BST_create(int32_t (*compare)(const BSTNode* const left, const BSTNode* const right),
              void     (*display)(FILE* fp, const BSTNode* const pD),
              void     (*destroy)(BSTNode* pNode)) {
	
	//Create BST tree object 

  BST tree;

    // Initialize root as NULL 
    //initialize the compare, display, and destroy 
	tree.root = NULL; 
  tree.compare  = compare; 
  tree.display = display; 
  tree.destroy = destroy; 

	return tree;
}

//////////////////////////////////////////////////////////////////////////////////// insertion fns
/** Inserts user data object into a BST, unless it duplicates an existing object.
 * 
 *  Pre:  pTree points to a proper BST object
 *        userNode points to a BSTNode in a client wrapper object
 * 
 *  Returns: true iff the insertion was performed; the implementation will not
 *           insert a new element that is equal to one that's already in the
 *           BST (according to the user-supplied comparison function)
 */
bool BST_insert(BST* const pTree, const BSTNode* const userNode) {
    if (userNode == NULL){
      return false; 
    }
    return BST_insertHelper(pTree, userNode, &(pTree->root)); 
}
static bool BST_insertHelper(BST* const pTree, BSTNode* const userNode, BSTNode** pNode){
    //if tree is empty, make the userNode the root 
    if ( *pNode == NULL){
      *pNode = userNode; 
      return true; 
    }

    //check the left subtree if the userNode is less than the pNode
    if (pTree->compare(userNode, *pNode) < 0){
      return BST_insertHelper(pTree, userNode, &((*pNode)->lchild));

    }
    //check the right subtree if the userNode is greater than the pNodde
    else if (pTree->compare(userNode, *pNode) > 0){
      return BST_insertHelper(pTree, userNode, &((*pNode)->rchild));
    }

    // if compare is equal, or other cases return false 
    else {
      return false; 
    }
}

//////////////////////////////////////////////////////////////////////////////////// search fns
/** Searches a proper BST for an occurence of a user data object that equals
 *  *pData (according to the user-supplied comparison function).
 * 
 *  Pre: pTree points to a proper BST object
 *       userNode points to a proper BSTNode inside a user's wrapper object
 * 
 *  Returns: pointer to BSTNode containing a matching user data object; 
 *           NULL if no match is found
 */
BSTNode* BST_find(const BST* const pTree, const BSTNode* const userNode) {
  if (userNode == NULL){
    return NULL; 
  }
  if (pTree != NULL){
   return BST_findHelper(pTree->root, userNode, pTree);
  }

	return NULL;
}

static BSTNode* BST_findHelper(const BSTNode* pNode, const BSTNode* const userNode, BST* pTree) {
  
  //compare the userNode to the pNode 
  int32_t direction = pTree->compare(userNode, pNode);

      if(pNode == NULL){  
        return NULL; 
      }  

      //if pNode is equal to userNode, return node 
      if (direction == 0) {
        return pNode; 
      }
      
      //if userNode is less than pNode, check right child 
      if ( direction < 0 ) {
        return BST_findHelper(pNode->lchild, userNode, pTree);
      }

      //if userNode is greater than pNode, check right child 
      if (direction > 0) {
        return BST_findHelper(pNode->rchild,userNode, pTree);
      }
    
}

//////////////////////////////////////////////////////////////////////////////////// destruction fns
/** Deallocates all dynamic memory associated with a proper BST object.
 * 
 *  Pre:   *pTree is a proper BST object
 *  Post:  all the user payloads and payload wrappers associated with *pTree
 *             have been freed
 *         the BST object itself is NOT freed since it may or may not have
 *             been allocated dynamically; that's the responsibility of the caller
 *  
 *  Calls: Payload_destroy() to handle destruction of the user's data object
 */
void BST_destroy(BST* const pTree) {
  BST_destroyHelper(pTree, pTree->root);
}
static void BST_destroyHelper(BST* pTree, BSTNode* pNode) {
  if (pNode != NULL) { 
 
    // first recur on left subtree
    if (pNode->lchild != NULL) { 
        BST_destroyHelper(pTree, pNode->lchild);
    }
    // then recur on right subtree
    if (pNode->rchild != NULL) {
        BST_destroyHelper(pTree, pNode->rchild);
    }

    // now deal with the node
    pTree->destroy(pNode);

  }
}

//////////////////////////////////////////////////////////////////////////////////// display fns
/** Writes a formatted display of the contents of a proper BST.
 * 
 *  Pre: fp is open on an output device
 *       pTree points to a proper BST object
 */
void BST_display(FILE* fp, const BST* const pTree) {
	
	if ( pTree->root == NULL ) {
		fprintf(fp, "Tree is empty.\n");
		return;
	}
	
	BST_displayHelper(fp, pTree->root, 0, pTree->display);
}


static void BST_displayHelper(FILE* fp, const BSTNode* pNode, uint32_t level,
                              void (*display)(FILE* fp, const BSTNode* const pD)) {
	
	if ( pNode == NULL ) {
		writeIndentation(fp, level);
		fprintf(fp, "%c\n", '*');
		return;
	}
	
	BST_displayHelper(fp, pNode->rchild, level + 1, display);
	
	writeIndentation(fp, level);
	display(fp, pNode);
	
	BST_displayHelper(fp, pNode->lchild, level + 1, display);
}

static void writeIndentation(FILE* fp, uint32_t level) {
	
	uint32_t spaces = 10 * level;
	char fmt[100] = {0};   // want: "%3*<level>c"
	fmt[0] = '%';
	sprintf(fmt+1, "%"PRIu32"c", spaces);
	fprintf(fp, fmt, ' ');
}

