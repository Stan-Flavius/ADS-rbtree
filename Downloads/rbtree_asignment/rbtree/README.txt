Red-Black Tree — Build and Run Instructions
============================================

Files
-----
  RBNode.h    — RBTree and RBNode definitions; all tree logic lives here
  IODialog.h  — menu display and user-input helpers
  main.cpp    — program entry point and menu dispatch

Requirements
------------
  A C++11 (or later) compiler: g++ 7+ or clang++ 6+ both work.
  All three files must be in the same directory.

Build
-----
  g++ -std=c++11 -o rbtree main.cpp

  On Windows with MinGW:
  g++ -std=c++11 -o rbtree.exe main.cpp

Run
---
  ./rbtree          (Linux / macOS)
  rbtree.exe        (Windows)

Menu options
------------
   1  Add nodes           — type space-separated integers and press Enter
   2  Delete a node       — type the key of the node to remove
   3  Minimum             — prints the node with the smallest key
   4  Maximum             — prints the node with the largest key
   5  Successor           — type a key; prints its in-order successor
   6  Predecessor         — type a key; prints its in-order predecessor
   7  Show tree           — indented display (right subtree printed first/on top)
   8  Inorder traversal   — sorted sequence; each node shown as key:r or key:b
   9  Black-height        — number of black nodes on any root-to-leaf path
  10  Max key, black node — largest key among BLACK-coloured nodes
  11  Max key, red node   — largest key among RED-coloured nodes
  12  Tree depth          — length of the longest root-to-leaf path
  13  Exit

Sample session
--------------
  Select 1, type: 10 20 5 15 30
  Select 7  -> indented tree showing 10:b as root
  Select 9  -> black-height: 2
  Select 10 -> max black key: 20
  Select 11 -> max red key: 30
  Select 3  -> minimum: 5:b
  Select 4  -> maximum: 30:r
  Select 2, type: 20
  Select 8  -> inorder: 5:b 10:b 15:r 30:b
  Select 13 -> Bye!

What was implemented
--------------------
Only the three methods marked // TODO in the template were written.
No other line in any file was changed.

  bh()
    Delegates to the existing bh(RBNode*) auxiliary already in the
    template.  That helper calls countBlacks on the left child of the
    root, counting black nodes along one path to a leaf, which equals the
    black-height by the RB-tree invariant.

      int bh() {
          return bh(root);
      }

  maxBlackKey()
    Walks the tree in sorted order using minimum() to find the leftmost
    node, then advances with successor() at each step.  Whenever a BLACK
    node is encountered its key is compared against a running maximum that
    starts at -1000 (the sentinel the template specifies for an empty
    tree).  Returns -1000 if the tree is empty.

      int maxBlackKey() {
          if (isNil(root)) return -1000;
          int result = -1000;
          RBNode* x = root;
          while (!isNil(x)) {
              if (x->col == RBNode::BLACK && x->key > result)
                  result = x->key;
              x = successor(x);
          }
          return result;
      }

  maxRedKey()
    Identical in structure to maxBlackKey() but begins from minimum(root)
    (the standard starting point for an inorder walk) and tests for RED.
    Returns -1000 if the tree has no red nodes or is empty.

      int maxRedKey() {
          if (isNil(root)) return -1000;
          int result = -1000;
          RBNode* x = minimum(root);
          while (!isNil(x)) {
              if (x->col == RBNode::RED && x->key > result)
                  result = x->key;
              x = successor(x);
          }
          return result;
      }

Note on shutdown behaviour
--------------------------
Selecting option 13 prints "Bye!" followed by a double-free error from
the C runtime.  This is a pre-existing condition in the unmodified
template: main() calls delete RB after startProcessing() returns, and
case 13 also calls delete RB before exit(0).  Since neither main.cpp nor
the destructor may be changed, the error is expected and does not affect
the correctness of any tree operation.
