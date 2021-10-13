#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "huffman.h"
#include "frequencies.h"
#include "priority_queue.h"
#include "bit_writer.h"

static int _compare(const void* node1, const void* node2) {
    const TreeNode* node1Val = node1; 
    const TreeNode* node2Val = node2; 
    
    if((*node1Val).frequency - (*node2Val).frequency == 0){
        return ((*node1Val).character - (*node2Val).character);
    }
    else{
        return ((*node1Val).frequency - (*node2Val).frequency);
    }
}

Node* make_huffman_pq(Frequencies freqs){

    Node* head = NULL;
    
    for (int i = 0; i < 256; i++) {
        if (freqs[i] > 0) {
            TreeNode* temp = malloc(sizeof(*temp));
            *temp = (TreeNode) {.character = i, .frequency = freqs[i], .left = NULL, .right = NULL};
            pq_enqueue(&head, temp, _compare);
        }
    }
    
    return head;
}

TreeNode* make_huffman_tree(Node* head){
    
    if (head == NULL){
        return NULL;
    }
    else{
        while(head -> next != NULL) {
            Node* dequeValue = pq_dequeue(&head);
            TreeNode* t1 = dequeValue -> a_value;
            Node* dequeValue2 = pq_dequeue(&head);
            TreeNode* t2 = dequeValue2 -> a_value; 
            free(dequeValue);
            free(dequeValue2);

            TreeNode* temp = malloc(sizeof(*temp));
            *temp = (TreeNode) {.character = 255 , .frequency = (t1 -> frequency + t2 -> frequency) , .left = t1, .right = t2};

            pq_enqueue(&head, temp , _compare);
        }

        TreeNode* newVal = head -> a_value; 
        free(head);

        return newVal;
    }

}

void destroy_huffman_tree(TreeNode** a_root){
    
    while (*a_root != NULL) {
        destroy_huffman_tree(&((*a_root) -> left));
        destroy_huffman_tree(&((*a_root) -> right));
        free(*a_root);
        *a_root = NULL;
    }
}

void write_coding_table(TreeNode* root, BitWriter* a_writer){

    if (root == NULL){
        return;
    }

    write_coding_table(root -> left, a_writer);
    write_coding_table(root -> right, a_writer);

    if ((root -> character) != '\0'){
        write_bits(a_writer , 0xff, 1);
        write_bits(a_writer , root -> character, 8);
        //printf("\n\n\n\n%d\n\n\n" , 10);
    }
    else {
        write_bits(a_writer , 0x00, 1);
    }
}

void _traverse(TreeNode* root, _BitCode table[], uint8_t bits, int bitsLeft) {

    if ((root -> character) == 255) {
        _traverse((root -> left), table, (bits << 1) | 0x00, bitsLeft + 1);
        _traverse((root -> right), table, (bits << 1) | 0x01, bitsLeft + 1);
    } 
    else {
        bits = bits << (8 - bitsLeft);
        table[(int)(root -> character)].bits = bits; 
        table[(int)(root -> character)].num_bits = bitsLeft;
    }

}

void write_compressed(TreeNode* root, BitWriter* a_writer, uint8_t* uncompressed_bytes) {
    
    if (root == NULL){
        return;
    }

    _BitCode table[256] = {0};
    uint8_t bits = 0x00;
    int bitsLeft = 0; 

    _traverse(root, table, bits, bitsLeft);

    for (int i = 0; uncompressed_bytes[i] != '\0'; i++){
        write_bits(a_writer , table[i].bits, table[i].num_bits);
    } 
}
