#define Black 1
#define Red 0
#include <stdio.h>
#include <stdlib.h>
int bh = 0;
int tb = 0;
int tr = 0;
int total = 0;

typedef struct Node*NodePtr;
struct Node {
   int val;
   NodePtr parent, LF, RT;
   int color;
};


NodePtr node_alloc(int newval) {
   NodePtr self = (NodePtr)malloc(sizeof(struct Node));
   self->val = newval;
   self->parent = NULL;
   self->LF = NULL;
   self->RT = NULL;
   self->color = Black;
   return self;
}


typedef struct RBT* RBTPtr;
struct RBT {
   NodePtr root;
};

struct node
{
   int dt;
   struct node *pro;
};


RBTPtr rbt_alloc() {
   RBTPtr self = (RBTPtr)malloc(sizeof(struct RBT));
   self->root = NULL;
   return self;
}


void LF_Rotate(RBTPtr self, NodePtr n) {
   NodePtr y;
   y = n->RT;
   n->RT = y->LF;
   if (y->LF != NULL)
      y->LF->parent = n;
   y->parent = n->parent;
   if (n->parent == NULL)
      self->root = y;
   else if (n == n->parent->LF)
      n->parent->LF = y;
   else
      n->parent->RT = y;
   y->LF = n;
   n->parent = y;
}


void RT_Rotate(RBTPtr self, NodePtr n) {
   NodePtr y;
   y = n->LF;
   n->LF = y->RT;
   if (y->RT != NULL)
      y->RT->parent = n;
   y->parent = n->parent;
   if (n->parent == NULL)
      self->root = y;
   else if (n == n->parent->RT)
      n->parent->RT = y;
   else
      n->parent->LF = y;
   y->RT = n;
   n->parent = y;
}


void rbt_insert_fixup(RBTPtr self, NodePtr n) {
   NodePtr y;
   while (n->parent != NULL && n->parent->color == Red) {
      if (n->parent == n->parent->parent->LF) {
         y = n->parent->parent->RT;
         if (y != NULL && y->color == Red) {
            n->parent->color = Black;
            y->color = Black;
            n->parent->parent->color = Red;
            n = n->parent->parent;
         }
         else if (n == n->parent->RT) {
            n = n->parent;
            LF_Rotate(self, n);
         }
         else {
            n->parent->color = Black;
            n->parent->parent->color = Red;
            RT_Rotate(self, n->parent->parent);
         }
      }
      else if (n->parent == n->parent->parent->RT) {
         y = n->parent->parent->LF;
         if (y != NULL && y->color == Red) {
            n->parent->color = Black;
            y->color = Black;
            n->parent->parent->color = Red;
            n = n->parent->parent;
         }
         else if (n == n->parent->LF) {
            n = n->parent;
            RT_Rotate(self, n);
         }
         else {
            n->parent->color = Black;
            n->parent->parent->color = Red;
            LF_Rotate(self, n->parent->parent);
         }
      }
   }
   self->root->color = Black;
}


void rbt_insert(RBTPtr self, NodePtr n) {
   NodePtr y = NULL;
   NodePtr x = self->root;
   while (x != NULL) {
      y = x;
      if (n->val < x->val)
         x = x->LF;
      else
         x = x->RT;
   }
   n->parent = y;
   if (y == NULL)
      self->root = n;
   else if (n->val < y->val)
      y->LF = n;
   else
      y->RT = n;
   n->LF = NULL;
   n->RT = NULL;
   n->color = Red;
   rbt_insert_fixup(self, n);
}

int rbt_bh(RBTPtr self, NodePtr tree, int bh) {
   if (tree == NULL) {
      return bh;
   }
   else {
      if (tree->color == Black)
         rbt_bh(self, tree->RT, bh + 1);
      else if (tree->color == Red)
         rbt_bh(self, tree->RT, bh);
   }
}

void rbt_print(RBTPtr self, NodePtr tree, int level) {
   if (tree->RT != NULL)
      rbt_print(self, tree->RT, level + 1);
   for (int i = 0; i < level; i++)
      printf("    ");
   if (tree->color == Black) {
      printf("%dB\n", tree->val);
      tb = tb + 1;
   }
   if (tree->color == Red) {
      printf("%dR\n", tree->val);
      tr = tr + 1;
   }
   if (tree->LF != NULL)
      rbt_print(self, tree->LF, level + 1);
}


void printList(struct node *node)
{
   while (node != NULL)
   {
      printf(" %d ", node->dt);
      node = node->pro;
   }
}

void push(struct node** head_ref, int new_dt)
{
   struct node* new_node = (struct node*) malloc(sizeof(struct node));
   new_node->dt = new_dt;
   new_node->pro = (*head_ref);
   (*head_ref) = new_node;
}

void deleteNode(struct node **head_ref, int key)
{
   struct node* temp = *head_ref, *prev;

   if (temp != NULL && temp->dt == key)
   {
      *head_ref = temp->pro;
      free(temp);
      return;
   }


   while (temp != NULL && temp->dt != key)
   {
      prev = temp;
      temp = temp->pro;
   }

   if (temp == NULL) return;

   prev->pro = temp->pro;

   free(temp);
}


int main()
{
   struct node* current = NULL;
   struct node* head = NULL;
   RBTPtr rbt = rbt_alloc();
   FILE *fp;
   int dt;
   fp = fopen("C:\\Users\\kamak\\Desktop\\고대2-1\\자구\\input.txt", "r");
   while (!feof(fp)) {
      fscanf(fp, "%d", &dt);
      if (dt >0)
         push(&head, dt);
      else if (dt < 0) {
         deleteNode(&head, -1 * dt);
      }
      else if (dt == 0) {
         break;
      }
   }
   current = head;
   while (current != NULL)
   {
      rbt_insert(rbt, node_alloc(current->dt));
      current = current->pro;

   }
   fclose(fp);
   rbt_print(rbt, rbt->root, 0);
   printf("\n\n\n\n");
   printf("bh is %d\n", rbt_bh(rbt, rbt->root, bh));
   printf("nb is %d\n", tb);
   printf("total is %d\n", tb + tr);
   return 0;
}
