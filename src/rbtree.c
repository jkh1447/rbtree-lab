#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>
rbtree *new_rbtree(void) {
  // TODO: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  nil->parent = nil;
  nil->left = nil;
  nil->right = nil;
  nil->key = 0;
  
  p->root = nil;
  p->nil = nil;
  return p;
}
// void preorder(rbtree *t, node_t *node){
//   if(node->left != t->nil) preorder(t, node->left);
//   if(node->right != t->nil) preorder(t, node->right);
//   free(node);
// }

void preorder(rbtree *t, node_t *node){
  if(node == t->nil) return;
  preorder(t, node->left);
  preorder(t, node->right);
  free(node);
}


void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if(t->root != t->nil) preorder(t, t->root);
  free(t->nil);
  free(t);
}

void right_rotate(rbtree *t, node_t *x){
  // 매개변수는 회전할 기준 노드
  node_t *y = x->left;

  x->left = y->right; // y->right = x; 위로 옮겨도 상관없을 것 같은데?
  if(y->right != t->nil){//y의 오른쪽 자식이 있다면
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil){
    // root노드이면
    t->root = y;
  }
  else{
    if(x->parent->right == x){
      x->parent->right = y;
    }
    else{
      x->parent->left = y;
    }
    
  }
  y->right = x;
  x->parent = y;
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;

  x->right = y->left;
  if(y->left != t->nil){
    y->left->parent = x;
  }

  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  }
  else{
    if(x->parent->right == x){
      x->parent->right = y;
    }
    else{
      x->parent->left = y;
    }

  }
  y->left = x;
  x->parent = y;

}

void insert_fixup(rbtree *t, node_t *node){
  
  while(node->parent != t->nil && node->parent->color == RBTREE_RED) {
    if(node->parent->parent->left == node->parent){
      //부모가 왼쪽 자식인가
      node_t *uncle = node->parent->parent->right;
      if(uncle->color == RBTREE_RED){
        node->parent->parent->color = RBTREE_RED;
        node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        node = node->parent->parent;
      }
      else{
        if(node->parent->right == node){
          // 부모의 오른쪽 자식일때
          node = node->parent;
          left_rotate(t, node);
        }
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        right_rotate(t, node->parent->parent);
      }
    }
    else{
      //부모가 오른쪽 자식
      node_t *uncle = node->parent->parent->left;
      if(uncle->color == RBTREE_RED){
        node->parent->parent->color = RBTREE_RED;
        node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        node = node->parent->parent;
      }
      else{
        if(node->parent->left == node){
          node = node->parent;
          right_rotate(t, node);
        }
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        left_rotate(t, node->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *root = t->root;
  node_t *parent = NULL;
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  if(root == t->nil){
    new_node->color = RBTREE_BLACK;
    new_node->parent = t->nil;
    new_node->left = t->nil;
    new_node->right = t->nil;
    new_node->key = key;

    t->root = new_node;
    
  }
  else{
    new_node->color = RBTREE_RED;
    new_node->left = t->nil;
    new_node->right = t->nil;
    new_node->key = key;
    while(1){
      if(root->key <= key){
        // 중복값은 큰 경우로
        parent = root;
        root = root->right;
        if(root == t->nil){
          parent->right = new_node;
          new_node->parent = parent;
          break;
        }
      }
      else{
        parent = root;
        root = root->left;
        if(root == t->nil){
          parent->left = new_node;
          new_node->parent = parent;
          break;
        }
      }
    }
  }

  insert_fixup(t, new_node);

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  
  node_t *cur = t->root;
  while(cur->key != key){
    if(cur->key < key){
      cur = cur->right;
    }
    else{
      cur = cur->left;
    }
    if(cur == t->nil) break;
  }

  if(cur == t->nil){
    //존재 하지 않을때
    
    return NULL;
  }
  else{
    
    return cur;
  }

  
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while(cur->left != t->nil){
    cur = cur->left;
  } 

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while(cur->right != t->nil){
    cur = cur->right;
  }
  return cur;
}

node_t *successor(const rbtree* t, node_t* node){
  
  node_t *cur = node->right;
  while(cur->left != t->nil){
    cur = cur->left;
  }
  return cur;
}

void delete_fixup(rbtree* t, node_t* x){
  // doubly black을 어떻게 빨간색 노드로 옮길까
  // x노드가 현재 extra black이 부여된 노드이다.
  // 이 노드가 재조정을 통해 빨간색이 되면 그냥 검은색으로 바꾸면 해결
  while(x != t->root && x->color != RBTREE_RED){
    if(x->parent->left == x){
      // 왼쪽 자식일때
      node_t *w = x->parent->right;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        //w = x->parent->right; //while문 돌면서 알아서 재조정하기때문에 필요없는듯?
        // 케이스1 후 케이스 2 3 4중 한케이스로 해결 가능하기 때문에 
        // 처음 분기문에 등장 & 형제가 빨간색인 것을 먼저 체크해야 함
      }
      else if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        w->right->color = RBTREE_BLACK;
        x->parent->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else{
      node_t *w = x->parent->left;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        
        
        //w = x->parent->left;
        // 케이스1 후 케이스 2 3 4중 한케이스로 해결 가능하기 때문에 
        // 처음 분기문에 등장 & 형제가 빨간색인 것을 먼저 체크해야 함
      }
      else if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        w->left->color = RBTREE_BLACK;
        x->parent->color =RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

void transplant(rbtree *t, node_t *u, node_t *v){
  // u를 루트로하는 서브트리를 v를 루트로하는 서브트리로 교체
  if(u->parent == t->nil){
    t->root = v;
  }
  else if (u == u->parent->left){
    // u가 왼쪽자식일때
    u->parent->left = v;
  }
  else u->parent->right = v;
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
    
    node_t *x = p; // 삭제될 노드
    color_t t_color = x->color; // 삭제되는 색
    node_t *t_node; // 재조정 기준 노드
    if(x->left == t->nil){
      transplant(t, x, x->right);
      t_node = x->right;
    }
    else if(x->right == t->nil){
      transplant(t,x, x->left);
      t_node = x->left;
    }
    else{
      x = successor(t, x);
      t_color = x->color;
      t_node = x->right; // 삭제되는 자리를 대체하는 노드
      if(p->right == x){
        // 바로 오른쪽이 successor일 경우
        t_node->parent = x; // t_node는 nil노드인데, 이후 재조정을 위해 nil노드의 parent를 연결
      }
      else{
        transplant(t, x, x->right); // x의 자리에 오른쪽 자식이 대체되고
        x->right = p->right; // x의 왼쪽에는 x가 대체된 오른쪽 서브트리가 붙고
        x->right->parent = x; // 오른쪽 노드의 부모가 x가 된다.
      }
      transplant(t, p, x);
      x->left = p->left; // 왼쪽 자식도 연결
      x->left->parent = x;
      x->color = p->color;
    }
    
    if(t_color == RBTREE_BLACK){
      delete_fixup(t, t_node);
    }

    free(p);
  
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t **stack = (node_t **)malloc((n+1) * sizeof(node_t *));
  int pos = 0;
  int s = 0;
  node_t *cur = t->root;
  
  
  while(1){
    while(cur != t->nil){
      stack[pos++] = cur;
      cur = cur->left;
    }
    if(pos == 0) break;
    cur = stack[--pos];
    arr[s++] = cur->key;
    cur = cur->right;
    if(s == n) break;

  }
  free(stack);
  return 0;
}
