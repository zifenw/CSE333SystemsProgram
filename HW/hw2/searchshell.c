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

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
#define INITIAL_BUF_SIZE 128   // Avoid magic number for buffer size
#define MAX_WORDS 100          // Avoid magic number for word storage

/**
 * @brief Prints usage instructions and exits the program.
 */
static void Usage(void);

/**
 * @brief Handles user queries, searches the index, and prints results.
 * 
 * @param dt Document table storing document metadata.
 * @param mi Memory index used for searching documents.
 */
static void ProcessQueries(DocTable *dt, MemIndex *mi);

/**
 * @brief Reads a line from a file, converts it to lowercase, and returns it.
 * 
 * @param f File stream to read from.
 * @param ret_str Pointer to store the allocated string.
 * @return 0 on success, -1 on error or EOF.
 */
static int GetNextLine(FILE *f, char **ret_str);


//////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char **argv) {
  if (argc != 2) {
    Usage();
  }

  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - Crawl from a directory provided by argv[1] to produce and index
  //  - Prompt the user for a query and read the query from stdin, in a loop
  //  - Split a query into words (check out strtok_r)
  //  - Process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.
  //
  // Note that you should make sure the fomatting of your
  // searchshell output exactly matches our solution binaries
  // to get full points on this part.

  DocTable *dt;
  MemIndex *mi;
  // Crawl the directory provided to build the DocTable and MemIndex.
  if (!CrawlFileTree(argv[1], &dt, &mi)) {
    fprintf(stderr,
      "Error: Failed to crawl the directory '%s'.\n", argv[1]);
    Usage();
  }
  Verify333(dt != NULL);
  Verify333(mi != NULL);
  printf("Indexing '%s'\n", argv[1]);

  // Process user queries in a loop.
  ProcessQueries(dt, mi);

  printf("shutting down...\n");
  // Free all dynamically allocated memory and resources.
  DocTable_Free(dt);
  MemIndex_Free(mi);

  // Exit successfully
  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Helper function definitions

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative " \
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}

static void ProcessQueries(DocTable *dt, MemIndex *mi) {
  char *query = NULL;
  LLIterator *llit;
  SearchResult *rs;

  while (1) {
    printf("enter query:\n");
    if (GetNextLine(stdin, &query) == -1) {
      break;
    }
    // Split the query into words using strtok_r.
    char *saveptr;
    char *word = strtok_r(query, " ", &saveptr);  // Split the query into words
    char *words[MAX_WORDS];  // Array to store up to 100 words
    int word_count = 0;

    while (word != NULL && word_count < MAX_WORDS) {
      words[word_count++] = word;  // Store the word in the array
      word = strtok_r(NULL, " ", &saveptr);  // Get the next word
    }

    // Process the query against the MemIndex and print the results.
    LinkedList *results = MemIndex_Search(mi, words, word_count);
    if (results != NULL) {
      llit = LLIterator_Allocate(results);
      Verify333(llit != NULL);

      // print out all the matching documents under
      // the directory argv[1] and the rank for the query
      do {
        LLIterator_Get(llit, (void **) &rs);
        printf("  %s (%u)\n", DocTable_GetDocName(dt, rs->doc_id), rs->rank);
      } while (LLIterator_Next(llit));

      LLIterator_Free(llit);
    }

      free(query);
    }
}

static int GetNextLine(FILE *f, char **ret_str) {
    if (f == NULL || ret_str == NULL) {
        return -1;  // Invalid arguments
    }

    size_t buf_size = INITIAL_BUF_SIZE;  // Initial buffer size
    size_t pos = 0;
    char *buf = (char *)malloc(buf_size);
    if (!buf) {
        return -1;  // Memory allocation failed
    }

    int ch;
    while ((ch = fgetc(f)) != EOF && ch != '\n') {
        // Resize buffer if needed
        if (pos + 1 >= buf_size) {
            size_t new_size = buf_size * 2;
            char *new_buf = (char *)realloc(buf, new_size);
            if (!new_buf) {
                free(buf);
                return -1;  // Memory allocation failed
            }
            buf = new_buf;
            buf_size = new_size;
        }

        buf[pos++] = tolower(ch);
    }

    // If no characters were read and EOF is reached, return error
    if (pos == 0 && ch == EOF) {
        free(buf);
        return -1;
    }

    buf[pos] = '\0';  // Null-terminate the string
    *ret_str = buf;   // Return the allocated string
    return 0;         // Success
}
