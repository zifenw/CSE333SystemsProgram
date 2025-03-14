/*
 * Copyright ©2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 03/06/25
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <string>
#include <ctime>


#include "SimpleQueue.h"

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::cerr;

const int NUM_PIES = 6;                 // # pies created by each producer
static SimpleQueue queue;               // queue of pies
static unsigned int seed = time(NULL);  // initialize random sleep time
static pthread_mutex_t write_lock;      // mutex for cout

// Thread safe print that prints the given str on a line
void thread_safe_print(string str) {
  pthread_mutex_lock(&write_lock);
  // Only one thread can hold the lock at a time, making it safe to
  // use cout. If we didn't lock before using cout, the order of things
  // put into the stream could be mixed up.
  cout << str << endl;
  pthread_mutex_unlock(&write_lock);
}

// Produces NUM_PIES pies of the given type
// You should NOT modify this method at all
void producer(string pie_type) {
  for (int i = 0; i < NUM_PIES; i++) {
    queue.Enqueue(pie_type);
    thread_safe_print(pie_type + " pie ready!");
    int sleep_time = rand_r(&seed) % 500 + 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
  }
}

// Eats 2 * NUM_PIES pies
// You should NOT modify this method at all
void consumer() {
  for (int i = 0; i < NUM_PIES * 2; i++) {
    bool successful = false;
    string pie_type;
    while (!successful) {
      while (queue.IsEmpty()) {
        // Sleep for a bit and then check again
        int sleep_time = rand_r(&seed) % 800 + 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
      }
      successful = queue.Dequeue(&pie_type);
    }
    thread_safe_print(pie_type + " pie eaten!");
  }
}

// Wrapper function for producer to be used with pthreads
void* producer_wrapper(void* arg) {
    string* pie_type = static_cast<string*>(arg);
    producer(*pie_type);
    return nullptr;
}

// Wrapper function for consumer to be used with pthreads
void* consumer_wrapper(void* arg) {
    consumer();
    return nullptr;
}

int main(int argc, char **argv) {
  pthread_mutex_init(&write_lock, NULL);
  // Your task: Make the two producers and the single consumer
  // all run concurrently (hint: use pthreads)

  // Create threads for producers and consumer
  pthread_t apple_producer_thread,
            blackberry_producer_thread,
            consumer_thread;
  string apple_pie = "Apple";
  string blackberry_pie = "Blackberry";
  if (pthread_create(&apple_producer_thread, nullptr,
                     producer_wrapper, &apple_pie) != 0) {
    cerr << "apple_pie producer pthread_create failed!";
  }
  if (pthread_create(&blackberry_producer_thread, nullptr,
                     producer_wrapper, &blackberry_pie) != 0) {
    cerr << "blackberry_pie producer pthread_create failed!";
  }
  if (pthread_create(&consumer_thread, nullptr,
                     consumer_wrapper, nullptr) != 0) {
    cerr << "Consumer pthread_create failed!" << endl;
  }

  if (pthread_join(apple_producer_thread, nullptr) != 0) {
    cerr << "apple_pie producer pthread_join failed!";
  }

  if (pthread_join(blackberry_producer_thread, nullptr) != 0) {
    cerr << "apple_pie producer pthread_join failed!";
  }

  if (pthread_join(consumer_thread, nullptr) != 0) {
    cerr << "Consumer pthread_join failed!" << endl;
  }
  pthread_mutex_destroy(&write_lock);
  return EXIT_SUCCESS;
}
