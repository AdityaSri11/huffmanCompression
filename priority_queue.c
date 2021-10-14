#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "priority_queue.h"
#include "huffman.h"

Node* pq_enqueue(Node** a_head, void* a_value, int (*cmp_fn)(const void*, const void*)) {
	
	Node* nodeValue = malloc(sizeof(*nodeValue));
	*nodeValue = (Node) {.a_value = a_value, .next = NULL};


	if (*a_head == NULL){
		*a_head = nodeValue;
		return nodeValue; 
	}
	if (cmp_fn(nodeValue -> a_value, (*a_head) -> a_value) <= 0){
		nodeValue -> next = *a_head;
		*a_head = nodeValue; 
		return nodeValue; 
	}
	else {
		Node* tempValue = *a_head; 

		while(tempValue -> next != NULL && cmp_fn(nodeValue -> a_value, tempValue -> next -> a_value) > 0){
			tempValue = tempValue -> next;
		}

		nodeValue -> next = tempValue -> next; 
		tempValue -> next = nodeValue; 

		return nodeValue; 
	}
}

Node* pq_dequeue(Node** a_head) {
	if(*a_head == NULL) {
		return NULL; 
	}

	Node* tempValue = *a_head;
	*a_head = (*a_head) -> next;
	tempValue -> next = NULL; 
	return tempValue; 
}


Node* stack_push(Node** a_head, void* a_value) {
	
	Node* returnValue = malloc(sizeof(*returnValue));
	*returnValue = (Node) {.a_value = a_value, .next = NULL};

	returnValue -> next = *a_head;
	(*a_head) = returnValue; 

	return returnValue;  
}


Node* stack_pop(Node** a_head) {
	return pq_dequeue(a_head);
}


void destroy_list(Node** a_head, void (*destroy_value_fn)(void*)) {

	while(*a_head != NULL){
		Node* tempHead = (*a_head) -> next; 
		destroy_value_fn((*a_head) -> a_value);
		free(*a_head);
		*a_head = tempHead;
	}

}


#define HUFFMAN_C_V1
