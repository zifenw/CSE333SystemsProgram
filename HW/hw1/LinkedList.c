/*
 * Copyright ©2025 Hal Perkins.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>
#include <stdlib.h>

#include "CSE333.h"
#include "LinkedList.h"
#include "LinkedList_priv.h"


///////////////////////////////////////////////////////////////////////////////
// LinkedList implementation.

LinkedList* LinkedList_Allocate(void) {
  // Allocate the linked list record.
  LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
  Verify333(ll != NULL);

  // STEP 1: initialize the newly allocated record structure.
  ll->num_elements = 0;
  ll->head = NULL;
  ll->tail = NULL;
  // Return our newly minted linked list.
  return ll;
}

void LinkedList_Free(LinkedList *list,
                     LLPayloadFreeFnPtr payload_free_function) {
  Verify333(list != NULL);
  Verify333(payload_free_function != NULL);

  // STEP 2: sweep through the list and free all of the nodes' payloads
  // (using the payload_free_function supplied as an argument) and
  // the nodes themselves.
  while (list->head != NULL) {
    // using function pointer free the head's payload:
    // function is ExamplePayload_Free(LLPayload_t payload)
    // can be found in example_problem_ll.c
    payload_free_function(list->head->payload);
    // save current node pointer to temp
    LinkedListNode* temp = list->head;
    // move head pointer to next
    list->head = list->head->next;
    // free the temp pointer
    free(temp);
  }
  // free the LinkedList
  free(list);
}

int LinkedList_NumElements(LinkedList *list) {
  Verify333(list != NULL);
  return list->num_elements;
}

void LinkedList_Push(LinkedList *list, LLPayload_t payload) {
  Verify333(list != NULL);

  // Allocate space for the new node.
  LinkedListNode *ln = (LinkedListNode *) malloc(sizeof(LinkedListNode));
  Verify333(ln != NULL);

  // Set the payload
  ln->payload = payload;

  if (list->num_elements == 0) {
    // Degenerate case; list is currently empty
    Verify333(list->head == NULL);  // if not NULL can continue
    Verify333(list->tail == NULL);
    ln->next = ln->prev = NULL;
    list->head = list->tail = ln;
    list->num_elements = 1;
  } else {
    // STEP 3: typical case; list has >=1 elements
    // make sure head and tail not empty
    Verify333(list->head != NULL);
    Verify333(list->tail != NULL);
    // let ln become head
    ln->next = list->head;
    ln->prev = NULL;
    list->head->prev = ln;
    list->head = ln;
    list->num_elements +=1;
  }
}

bool LinkedList_Pop(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  // STEP 4: implement LinkedList_Pop.  Make sure you test for
  // and empty list and fail.  If the list is non-empty, there
  // are two cases to consider: (a) a list with a single element in it
  // and (b) the general case of a list with >=2 elements in it.
  // Be sure to call free() to deallocate the memory that was
  // previously allocated by LinkedList_Push().

  // pop false for empty list
  if (list->num_elements == 0) {
    return false;
  }

  // save payload to payload_ptr
  *payload_ptr = list->head->payload;
  // save the head pointer to temp
  LinkedListNode* temp = list->head;

  if (list->num_elements == 1) {
    // a list with a single element in it
    list->head = NULL;
    list->tail = NULL;
  } else {
    // a list with >=2 elements in it
    list->head = list->head->next;
    list->head->prev = NULL;
  }

  list->num_elements -= 1;
  // free the previous head
  free(temp);
  // pop succeeded
  return true;
}

void LinkedList_Append(LinkedList *list, LLPayload_t payload) {
  Verify333(list != NULL);

  // STEP 5: implement LinkedList_Append.  It's kind of like
  // LinkedList_Push, but obviously you need to add to the end
  // instead of the beginning.

  // Allocate space for the new node.
  LinkedListNode *ln = (LinkedListNode *) malloc(sizeof(LinkedListNode));
  Verify333(ln != NULL);

  // set the payload
  ln->payload = payload;

  // the case that list is empty
  if (list->num_elements == 0) {
    // degenerate case; list is currently empty
    Verify333(list->head == NULL);
    Verify333(list->tail == NULL);

    ln->next = ln->prev = NULL;
    list->head = list->tail = ln;
    list->num_elements = 1U;
  } else {
    // the case that list is not empty
    // make sure head and tail not empty
    Verify333(list->head != NULL);
    Verify333(list->tail != NULL);

    // let ln become tail
    ln->next = NULL;
    ln->prev = list->tail;
    list->tail->next = ln;
    list->tail = ln;
    list->num_elements += 1;
  }
}

void LinkedList_Sort(LinkedList *list, bool ascending,
                     LLPayloadComparatorFnPtr comparator_function) {
  Verify333(list != NULL);
  if (list->num_elements < 2) {
    // No sorting needed.
    return;
  }

  // We'll implement bubblesort! Nnice and easy, and nice and slow :)
  int swapped;
  do {
    LinkedListNode *curnode;

    swapped = 0;
    curnode = list->head;
    while (curnode->next != NULL) {
      int compare_result = comparator_function(curnode->payload,
                                               curnode->next->payload);
      if (ascending) {
        compare_result *= -1;
      }
      if (compare_result < 0) {
        // Bubble-swap the payloads.
        LLPayload_t tmp;
        tmp = curnode->payload;
        curnode->payload = curnode->next->payload;
        curnode->next->payload = tmp;
        swapped = 1;
      }
      curnode = curnode->next;
    }
  } while (swapped);
}


///////////////////////////////////////////////////////////////////////////////
// LLIterator implementation.

LLIterator* LLIterator_Allocate(LinkedList *list) {
  Verify333(list != NULL);

  // OK, let's manufacture an iterator.
  LLIterator *li = (LLIterator *) malloc(sizeof(LLIterator));
  Verify333(li != NULL);

  // Set up the iterator.
  li->list = list;
  li->node = list->head;

  return li;
}

void LLIterator_Free(LLIterator *iter) {
  Verify333(iter != NULL);
  free(iter);
}

bool LLIterator_IsValid(LLIterator *iter) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);

  return (iter->node != NULL);
}

bool LLIterator_Next(LLIterator *iter) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  // STEP 6: try to advance iterator to the next node and return true if
  // you succeed, false otherwise
  // Note that if the iterator is already at the last node,
  // you should move the iterator past the end of the list

  // advance iterator if there is a node after it
  if (iter->node->next != NULL) {
    // if there is a node after iterator, move to next and return true
    iter->node = iter->node->next;
    return true;
  } else {
    // if there is no node after iterator, set NULL and return false
    iter->node = NULL;
    return false;
  }
}

void LLIterator_Get(LLIterator *iter, LLPayload_t *payload) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  *payload = iter->node->payload;
}

bool LLIterator_Remove(LLIterator *iter,
                       LLPayloadFreeFnPtr payload_free_function) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  // STEP 7: implement LLIterator_Remove.  This is the most
  // complex function you'll build.  There are several cases
  // to consider:
  // - degenerate case: the list becomes empty after deleting.
  // - degenerate case: iter points at head
  // - degenerate case: iter points at tail
  // - fully general case: iter points in the middle of a list,
  //                       and you have to "splice".
  //
  // Be sure to call the payload_free_function to free the payload
  // the iterator is pointing to, and also free any LinkedList
  // data structure element as appropriate.


  // free the current node's payload
  payload_free_function(iter->node->payload);
  // pointer to the current node
  LinkedListNode* temp = iter->node;

  // the list becomes empty after deleting.
  if (iter->list->num_elements == 1) {
    iter->node = NULL;
    iter->list->num_elements = 0;
    iter->list->head = NULL;
    iter->list->tail = NULL;

    // free the iter node
    free(temp);
    // return false since the list is empty now
    return false;
  }
  iter->list->num_elements -= 1;

  if (iter->node->prev == NULL) {
    // iter points at head
    iter->node = iter->node->next;
    iter->node->prev = NULL;
    iter->list->head = iter->node;
  } else if (iter->node->next == NULL) {
    // iter points at tail
    iter->node = iter->node->prev;
    iter->node->next = NULL;
    iter->list->tail = iter->node;
  } else {
    // normal case
    iter->node->prev->next = iter->node->next;
    iter->node->next->prev = iter->node->prev;
    iter->node = iter->node->next;
  }

  free(temp);
  // remove succeed return true
  return true;
}


///////////////////////////////////////////////////////////////////////////////
// Helper functions

bool LLSlice(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  // STEP 8: implement LLSlice.
  // return false since nothing can be sliced from the empty list
  if (list->num_elements == 0) {
    return false;
  }
  // store the payload of tail
  *payload_ptr = list->tail->payload;
  // old tails pointer, save to temp and going to free later
  LinkedListNode* temp =list-> tail;

  if (list->num_elements == 1) {
    // only have one node
    list->head = NULL;
    list->tail = NULL;
  } else {
    // normal case
    list->tail = list->tail->prev;
    list->tail->next = NULL;
  }

  list->num_elements -= 1;
  // free the old tail
  free(temp);
  // return true since slice succeed
  return true;
}

void LLIteratorRewind(LLIterator *iter) {
  iter->node = iter->list->head;
}
