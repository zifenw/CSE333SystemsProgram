/*
 * Copyright ©2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 03/06/25
 */

#include "SimpleQueue.h"

SimpleQueue::SimpleQueue() {
  size = 0;
  front = nullptr;
  end = nullptr;
  pthread_mutex_init(&lock, nullptr);  // Initialize the mutex
}

SimpleQueue::~SimpleQueue() {
  while (front != nullptr) {
    node *next = front->next;
    delete front;
    front = next;
  }
  pthread_mutex_destroy(&lock);  // Destroy the mutex
}

void SimpleQueue::Enqueue(string item) {
  pthread_mutex_lock(&lock);
  node *new_node = new node();
  new_node->next = nullptr;
  new_node->item = item;
  if (end != nullptr) {
    end->next = new_node;
  } else  {
    front = new_node;
  }
  end = new_node;
  size++;
  pthread_mutex_unlock(&lock);
}

bool SimpleQueue::Dequeue(string *result) {
  pthread_mutex_lock(&lock);
  if (size == 0) {
    pthread_mutex_unlock(&lock);
    return false;
  }
  *result = front->item;
  node *next = front->next;
  delete front;
  if (end == front) {
    end = front = next;
  } else {
    front = next;
  }
  size--;
  pthread_mutex_unlock(&lock);
  return true;
}

int SimpleQueue::Size() const {
  pthread_mutex_lock(&lock);
  int current_size = size;
  pthread_mutex_unlock(&lock);
  return current_size;
}

bool SimpleQueue::IsEmpty() const {
  pthread_mutex_lock(&lock);
  bool is_empty = (size == 0);
  pthread_mutex_unlock(&lock);
  return is_empty;
}
