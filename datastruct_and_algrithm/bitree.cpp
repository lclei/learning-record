#include <iostream>
#include <stack>
using namespace std;

// tree node
typedef struct BinaryTreeNode{
  int data;
  BinaryTreeNode* LeftChild;
  BinaryTreeNode* RightChild;
}BinaryTreeNode, *BiTree;

BinaryTreeNode* CreateTree(){
  int d;
  BinaryTreeNode* t;
  scanf("%d",&d);

  if(d == 0){
    t = NULL;
  }
  else{
    t = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    t->data = d;
    printf("add %d\n",d);
    t->LeftChild = CreateTree();
    t->RightChild = CreateTree();
  }
  return t;
}

void print_recursion(BinaryTreeNode * n){
  if(n == NULL){
    cout << "null" << endl;
  }
  else{
    cout << n->data;
    print_recursion(n->LeftChild);
    print_recursion(n->RightChild);
  }
}

void print_Norecursion(BinaryTreeNode *n){
  stack<BinaryTreeNode *> NodeStack;
  while(n!=NULL || !NodeStack.empty()){
    while(n){
      cout << n->data <<" ";
      NodeStack.push(n);
      n = n->LeftChild;
    }
    if(!NodeStack.empty()){
      n = NodeStack.top();
      NodeStack.pop();
      n = n->RightChild;
    }
  }
}

int main(int argc, char *argv[]){
  BinaryTreeNode* root;
  root = CreateTree();
  print_recursion(root);
  print_Norecursion(root);
}
