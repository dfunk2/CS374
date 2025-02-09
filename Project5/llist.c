#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "llist.h"
/*Denise Funk
CS 374*/

//node_alloc()
struct node *node_alloc(int value){
    struct node * new_node = malloc(sizeof(struct node));
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

//node_free 
void node_free(struct node *n){
    if(n != NULL) {
        free(n);
    }
}

//llist_print
void llist_print(struct node *head){
    struct node * curr = head;
    while(curr != NULL){
        if(curr->next != NULL){
            printf("%d -> ", curr->value);
        } else {
            printf("%d\n", curr->value);
        }
        curr = curr->next;
    }
}

//llist_insert_head()
void llist_insert_head(struct node **head, struct node *n){
    n->next = *head;
    *head = n;
}

//llist_insert_tail()
void llist_insert_tail(struct node **head, struct node *n){
    
    if(*head == NULL){
        *head = n;
    }else {
        struct node *curr = *head;
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = n;
    }
}
//llist_delete_head
struct node *llist_delete_head(struct node **head){
    if(*head == NULL){
        return 0;
    } else {
        struct node *curr = *head;
        *head = curr->next;
        free(curr);
    }
    return *head;
}

//llist_free
void llist_free(struct node **head){
    struct node *curr = *head;
    while(curr != NULL){
        struct node *del = curr;
        curr = curr->next;
        free(del);
    }
    *head = NULL;
}

int main(int argc, char *argv[]) {
    struct node *head = NULL;

    //dont accept less than 1 argument
    if(argc < 2) {
        printf("[Empty List]\n");
        exit(1);
    }

    //Parse the command line. Call functions.
    char *token[128] ={0}; //hold parsed command line arg
    int i = 0; //iterates through tokens
    
    for(int k = 1; k < argc; k++) {
        token[i] = argv[k];
        i++;
    }

    int j = 0; 
    while(token[j] != NULL){
        //ih condition
        if(strcmp(token[j], "ih") == 0){
            char *str = token[j+1];
            int x = atoi(str);
            struct node *n = node_alloc(x);
            llist_insert_head(&head, n);
        }
        //it condition
        if(strcmp(token[j], "it") == 0){
            char *string = token[j+1];
            int y = atoi(string);
            struct node *n = node_alloc(y);
            llist_insert_tail(&head, n);
        }
        //dh condition
        if(strcmp(token[j], "dh") == 0){
            llist_delete_head(&head);
        }
        if(strcmp(token[j], "f") == 0){
            llist_free(&head);
        }
        if(strcmp(token[j], "p") == 0){
            struct node *curr = head;
            if(curr == NULL){
                printf("[Empty]\n");
            } else {
                llist_print(head);
            }
        }
        j++;
    }
    return 0;
}