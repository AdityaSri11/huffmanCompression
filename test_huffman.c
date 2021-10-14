#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "huffman.h"
#include "frequencies.h"
#include "miniunit.h"

static int _length_compare(const void* leftCheck, const void* rightCheck){
    size_t len_left = strlen(leftCheck);
    size_t len_right = strlen(rightCheck);

    return len_left - len_right; 
}

static void _free_string(void* a_str){
    free(a_str);
}

static char* _compy(const char* a_str) {
    size_t charCount = strlen(a_str);
    char* stringCount = malloc(sizeof(*stringCount)* (charCount + 1));
    return strcpy(stringCount , a_str);
}

void _pq_checker(){
    Node* headValue = NULL; 
    pq_enqueue(&headValue, _compy("Yes,"), _length_compare);
    pq_enqueue(&headValue, _compy("My"), _length_compare);
    pq_enqueue(&headValue, _compy("name"), _length_compare);
    pq_enqueue(&headValue, _compy("is"), _length_compare);
    pq_enqueue(&headValue, _compy("Aditya"), _length_compare);
    pq_enqueue(&headValue, _compy("Srikanth"), _length_compare);

    for(Node* current = headValue; current != NULL; current = current -> next) {
        printf("%s\n" , (char*)(current -> a_value));
    }

    Node* temp;
    temp = pq_dequeue(&headValue);
    destroy_list(&temp, _free_string);
    destroy_list(&headValue, _free_string);

    headValue = NULL; 
    
    Node* tempAgain; 
    tempAgain = pq_dequeue(&headValue);
    destroy_list(&tempAgain, _free_string);
    destroy_list(&headValue, _free_string);
}

static void _print_frequencies(Frequencies freqs){
    for (int i = 0; i <= 255; i++){
        printf("%c: %ld\n" , i, freqs[i]);
    }
}

int _test_notNullFile(){
    mu_start();

    Frequencies freqs;

    for(int i = 0; i <= 255; i++){
        freqs[i] = 0; 
    }

    const char* path = "FileCheck.txt";
    const char* a_error;

    bool is_success = calc_frequencies(freqs, path, &a_error); 
    Node* test = make_huffman_pq(freqs);
    TreeNode* head = make_huffman_tree(test);

    unsigned char stringTest[] = "Aditya Srikanth";
    unsigned char* varPass = &stringTest[0];

    BitWriter writer = open_bit_writer("test3.bits");
    write_coding_table(head , &writer);
    write_compressed(head, &writer, varPass);
    close_bit_writer(&writer);

    destroy_huffman_tree(&head);
    //_print_frequencies(freqs);
    mu_check(is_success);

    mu_end();
}

int _test_null_tree() {
    mu_start();

    make_huffman_tree(NULL);

    mu_end();
}

int _test_NullFile(){
    mu_start();

    Frequencies freqs;
    const char* path = "FileNull.txt";
    const char* a_error;

    bool is_success = calc_frequencies(freqs, path, &a_error); 

    mu_check(!is_success);
    mu_check(a_error == strerror(errno));

    mu_end();
}

int _stack_checker(){
    mu_start();

    bool is_success = false; 
    
    Node* headValue = NULL; 
    Node* temp1;
    temp1 = stack_pop(&headValue);
    if (temp1 == NULL){
        is_success = true;
    }
    mu_check(is_success);
    destroy_list(&temp1 , _free_string);
    destroy_list(&headValue, _free_string);
    
    headValue = NULL; 

    stack_push(&headValue, _compy("Check"));
    mu_check(is_success);
    destroy_list(&headValue , _free_string);

    temp1 = pq_dequeue(&headValue);
    temp1 = stack_pop(&headValue);
    destroy_list(&headValue , _free_string);
    destroy_list(&headValue , _free_string);

    pq_enqueue(&headValue, _compy("Check"), _length_compare);
    Node* temp3 = stack_pop(&headValue);
    destroy_list(&temp3 , _free_string);
    destroy_list(&headValue , _free_string);

    mu_end();
}

void _testNULL(){

    TreeNode* root = NULL; 
    BitWriter writer = open_bit_writer("new_file.bits");
    
    unsigned char stringTest[] = "Aditya Srikanth";
    unsigned char* varPass = &stringTest[0];

    write_coding_table(root , &writer);
    write_compressed(root, &writer, varPass);
    close_bit_writer(&writer);
}

void _testNULLOne(){

    TreeNode root = (TreeNode) {.character = '\0'};
    BitWriter writer = open_bit_writer("new_file.bits");

    write_coding_table(&root , &writer);
    close_bit_writer(&writer);
}


int main(int argc, char* argv[]) {

    _pq_checker(); 
    mu_run(_stack_checker);
    mu_run(_test_notNullFile);
    mu_run(_test_NullFile);
    mu_run(_test_null_tree);
    _testNULL();
    _testNULLOne();

}
