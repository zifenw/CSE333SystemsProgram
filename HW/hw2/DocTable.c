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
#include "./DocTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libhw1/CSE333.h"
#include "libhw1/HashTable.h"

#define HASHTABLE_INITIAL_NUM_BUCKETS 2

// This structure represents a DocTable; it contains two hash tables, one
// mapping from document id to document name, and one mapping from
// document name to document id.
struct doctable_st {
  HashTable *id_to_name;  // mapping document id to document name
  HashTable *name_to_id;  // mapping document name to document id
  DocID_t    max_id;      // max doc ID allocated so far
};

DocTable* DocTable_Allocate(void) {
  DocTable *dt = (DocTable*) malloc(sizeof(DocTable));
  Verify333(dt != NULL);

  dt->id_to_name = HashTable_Allocate(HASHTABLE_INITIAL_NUM_BUCKETS);
  dt->name_to_id = HashTable_Allocate(HASHTABLE_INITIAL_NUM_BUCKETS);
  dt->max_id = 1;  // we reserve max_id = 0 for the invalid docID

  return dt;
}

void DocTable_Free(DocTable *table) {
  Verify333(table != NULL);

  // STEP 1.
  // Free the id_to_name and name_to_id hash tables.
  HashTable_Free(table->id_to_name, &free);
  HashTable_Free(table->name_to_id, &free);

  free(table);  // Free the DocTable structure itself
}

int DocTable_NumDocs(DocTable *table) {
  Verify333(table != NULL);
  return HashTable_NumElements(table->id_to_name);
}

DocID_t DocTable_Add(DocTable* table, char* doc_name) {
  char *doc_copy;
  DocID_t *doc_id;
  DocID_t res;
  HTKeyValue_t kv, old_kv;

  Verify333(table != NULL);

  // STEP 2.
  // Check to see if the document already exists.  Then make a copy of the
  // doc_name and allocate space for the new ID.
  // Check if doc_name is NULL.

  // Hash the document name
  res = FNVHash64((unsigned char *) doc_name, strlen(doc_name));
  // Look up the document name in the name->id hash table
  int retval = HashTable_Find(table->name_to_id, res, &old_kv);
  Verify333(retval != -1);  // Ensure no errors occurred

  if (retval == 1) {
    // Return the existing DocID if the document is already in the table
    return *((DocID_t *) old_kv.value);
  }
  // Allocate memory for the document name copy
  doc_copy = (char *) malloc(1 + strlen(doc_name));
  Verify333(doc_copy != NULL);  // Ensure malloc succeeded
  // Copy the document name into the allocated memory
  snprintf(doc_copy, strlen(doc_name) + 1, "%s", doc_name);
  // Ensure the copy matches the original
  Verify333(strcmp(doc_copy, doc_name) == 0);

  // Allocate memory for the new DocID
  doc_id = (DocID_t *) malloc(sizeof(DocID_t));   
  Verify333(doc_id != NULL);  // Ensure malloc succeeded

  *doc_id = table->max_id;
  table->max_id++;

  // STEP 3.
  // Set up the key/value for the id->name mapping, and do the insert.
  kv.key = *doc_id;
  kv.value = doc_copy;
  // Insert into the id->name hash table
  retval = HashTable_Insert(table->id_to_name, kv, &old_kv);
  Verify333(retval == 0);  // Ensure the insert succeeded

  // STEP 4.
  // Set up the key/value for the name->id, and/ do the insert.
  // Be careful about how you calculate the key for this mapping.
  // You want to be sure that how you do this is consistent with
  // the provided code.
  kv.key = res;
  kv.value = doc_id;
  // Insert into the name->id hash table
  retval = HashTable_Insert(table->name_to_id, kv, &old_kv);
  Verify333(retval == 0);  // Ensure the insert succeeded

  return *doc_id;  // Return the new DocID
}

DocID_t DocTable_GetDocID(DocTable *table, char *doc_name) {
  HTKey_t key;
  HTKeyValue_t kv;
  DocID_t res;

  Verify333(table != NULL);
  Verify333(doc_name != NULL);

  // STEP 5.
  // Try to find the passed-in doc in name_to_id table.
  // Hash the document name
  key = FNVHash64((unsigned char *) doc_name, strlen(doc_name));
  // Look up the document name in the name->id hash table
  res = HashTable_Find(table->name_to_id, key, &kv);
  Verify333(res != -1);  // Ensure no errors occurred

  if (res == INVALID_DOCID) {
    // Return INVALID_DOCID if the document is not found
    return INVALID_DOCID;
  } else {
    // Return the DocID if the document is found
    return *((uint64_t *) kv.value);
  }
}

char* DocTable_GetDocName(DocTable *table, DocID_t doc_id) {
  HTKeyValue_t kv;

  Verify333(table != NULL);
  Verify333(doc_id != INVALID_DOCID);

  // STEP 6.
  // Lookup the doc_id in the id_to_name table,
  // and either return the string (i.e., the (char *)
  // saved in the value field for that key) or
  // NULL if the key isn't in the table.
  int res;
  // Look up the DocID in the id->name hash table
  res = HashTable_Find(table->id_to_name, doc_id, &kv);
  Verify333(res != -1);  // Ensure no errors occurred

  if (res == INVALID_DOCID) {
    // Return NULL if the DocID is not found
    return NULL;
  } else {
    // Return the document name if the DocID is found
    return ((char *) kv.value);
  }
}

HashTable* DT_GetIDToNameTable(DocTable *table) {
  Verify333(table != NULL);
  return table->id_to_name;
}

HashTable* DT_GetNameToIDTable(DocTable *table) {
  Verify333(table != NULL);
  return table->name_to_id;
}
