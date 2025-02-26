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

#include "./QueryProcessor.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
extern "C" {
  #include "./libhw1/CSE333.h"
}

using std::list;
using std::sort;
using std::string;
using std::vector;

namespace hw3 {

// This helper function takes two DocIDElementHeader lists as input.
// It finds the common elements (documents with the same doc_id) between
// both lists and returns a new list containing these common doc_ids, with
// the number of occurrences for each doc_id summed from both lists.
static list<DocIDElementHeader> MergeDocIDLists
       (const list<DocIDElementHeader>& list1,
        const list<DocIDElementHeader>& list2);

QueryProcessor::QueryProcessor(const list<string> &index_list, bool validate) {
  // Stash away a copy of the index list.
  index_list_ = index_list;
  array_len_ = index_list_.size();
  Verify333(array_len_ > 0);

  // Create the arrays of DocTableReader*'s. and IndexTableReader*'s.
  dtr_array_ = new DocTableReader*[array_len_];
  itr_array_ = new IndexTableReader*[array_len_];

  // Populate the arrays with heap-allocated DocTableReader and
  // IndexTableReader object instances.
  list<string>::const_iterator idx_iterator = index_list_.begin();
  for (int i = 0; i < array_len_; i++) {
    FileIndexReader fir(*idx_iterator, validate);
    dtr_array_[i] = fir.NewDocTableReader();
    itr_array_[i] = fir.NewIndexTableReader();
    idx_iterator++;
  }
}

QueryProcessor::~QueryProcessor() {
  // Delete the heap-allocated DocTableReader and IndexTableReader
  // object instances.
  Verify333(dtr_array_ != nullptr);
  Verify333(itr_array_ != nullptr);
  for (int i = 0; i < array_len_; i++) {
    delete dtr_array_[i];
    delete itr_array_[i];
  }

  // Delete the arrays of DocTableReader*'s and IndexTableReader*'s.
  delete[] dtr_array_;
  delete[] itr_array_;
  dtr_array_ = nullptr;
  itr_array_ = nullptr;
}

// This structure is used to store a index-file-specific query result.
typedef struct {
  DocID_t doc_id;  // The document ID within the index file.
  int rank;        // The rank of the result so far.
} IdxQueryResult;

vector<QueryProcessor::QueryResult>
QueryProcessor::ProcessQuery(const vector<string> &query) const {
  Verify333(query.size() > 0);

  // STEP 1.
  // (the only step in this file)
  vector<QueryProcessor::QueryResult> final_result;

  // Iterate over each index file to process the query
  for (int i = 0; i < array_len_; i++) {
    IndexTableReader* itr = itr_array_[i];

    // Look up the DocID list for the first query word
    auto first_reader =
    std::unique_ptr<DocIDTableReader>(itr->LookupWord(query[0]));
    if (!first_reader) {  // skip if doesn't exist
      continue;
    }
    list<DocIDElementHeader> current_docs = first_reader->GetDocIDList();
    // Process the subsequent query words,
    // performing intersection and accumulation operations
    for (size_t j = 1; j < query.size(); ++j) {
      auto next_reader =
      std::unique_ptr<DocIDTableReader>(itr->LookupWord(query[j]));

      if (!next_reader) {  // clear results and break if the word doesn't exist
        current_docs.clear();
        break;
      }
      list<DocIDElementHeader> next_docs = next_reader->GetDocIDList();
      current_docs = MergeDocIDLists(current_docs, next_docs);
      if (current_docs.empty()) {  // No intersection found, break early
        break;
      }
    }

    // Convert the current index results into the final QueryResult format
    if (!current_docs.empty()) {
      DocTableReader* dtr = dtr_array_[i];
      for (const auto& doc : current_docs) {
        QueryResult qr;
        Verify333(dtr->LookupDocID(doc.doc_id, &qr.document_name));
        qr.rank = doc.num_positions;
        final_result.push_back(qr);
      }
    }
  }
  // Sort the final results.
  sort(final_result.begin(), final_result.end());
  return final_result;
}
// Merge two DocID lists, take the intersection,
// and accumulate the frequency of occurrences (optimized version)
static list<DocIDElementHeader> MergeDocIDLists(
    const list<DocIDElementHeader>& list1,
    const list<DocIDElementHeader>& list2) {
    list<DocIDElementHeader> merged_list;

    // Use a hash map to record the doc_id and its frequency from list2
    std::unordered_map<DocID_t, int> list2_map;
    for (const auto& elem : list2) {
        list2_map[elem.doc_id] = elem.num_positions;
    }

    // Iterate through list1, find common doc_ids, and accumulate the frequency
    for (const auto& elem1 : list1) {
        auto it = list2_map.find(elem1.doc_id);
        if (it != list2_map.end()) {
            merged_list.emplace_back(DocIDElementHeader{
                elem1.doc_id, elem1.num_positions + it->second});
        }
    }

    return merged_list;
}
}  // namespace hw3
