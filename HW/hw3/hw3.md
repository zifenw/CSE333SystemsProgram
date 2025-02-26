## Test
scp "C:\Users\Wang\Desktop\lab2-bomb.tar" zifenw@attu.cs.washington.edu:~/lab2/
rm -r directoryname
wget https://example.com/file.txt

valgrind --leak-check=full ./solution_binaries/test_suite

valgrind --leak-check=full ./test_tree/tiny  /tmp/tiny.idx

valgrind --leak-check=full ./test_suite
python3 ../cpplint.py --clint *.cc






g++ -Wall -g -std=c++17 -o ex14 ex14.cc
./ex14
cpplint ex14.cc

valgrind --leak-check=full ./ex12
python3 cpplint.py --clint ex12.cc

## Goals & Guidelines
In this assignment you will build on top of your HW2 implementation: In Part A, you will write code that takes an in-memory inverted index produced by HW2 and writes it out to disk in an architecture-neutral format. In Part B, you will write C++ code that walks through an on-disk index to service a lookup. Finally, in Part C, you will write a query processor that serves queries from multiple on-disk indices. 在本作业中，您将在HW2实现的基础上进行构建：在A部分，您将编写代码，该代码采用HW2生成的内存倒排索引，并以架构中立的格式将其写入磁盘。在B部分中，您将编写C++代码，遍历磁盘上的索引以提供查找服务。最后，在C部分中，您将编写一个查询处理器，为来自多个磁盘索引的查询提供服务。

As before, please read through this entire document before beginning the assignment, and please start early! As usual, there is a lot of code involved with HW3, and since this is your first large-scale attempt to use C++ you should expect to encounter a lot of problems along the way. Also, manipulating on-disk data is trickier than in-memory data structures, so plan for some time to get this part right. 如前所述，请在开始作业前通读整个文档，并请尽早开始！与往常一样，HW3涉及大量代码，由于这是您首次大规模尝试使用C++，您应该会遇到很多问题。此外，操作磁盘上的数据比内存中的数据结构更棘手，因此请计划一段时间来正确处理这部分。

In HW3, as with HW2, you don't need to worry about propagating errors back to callers in all situations. You will use Verify333()'s to spot errors and cause your program to crash out if they occur. We will not be using C++ exceptions in HW3. 在HW3中，与HW2一样，您不需要担心在所有情况下都会将错误传播回调用者。您将使用Verify333（）来发现错误，并在发生错误时导致程序崩溃。我们不会在HW3中使用C++异常。

Also, as before, you may not modify any of the existing header files or class definitions distributed with the code. If you wish to add extra "helper" functions you can to do that by including additional static functions in the implementation (.cc) files. 此外，与以前一样，您不能修改随代码分发的任何现有头文件或类定义。如果你想添加额外的“辅助”函数，你可以通过在实现（.cc）文件中包含额外的静态函数来实现。

## Disk-Based File System Search Engine, Part A: Memory-to-File Index Marshaller
Keeping a search engine index in memory is problematic, since memory is expensive and also volatile. So, in Part A, you're going to write some C++ code that takes advantage of your HW2 implementation to first build an in-memory index of a file subtree, and then it will write that index into an index file in an architecture-neutral format.将搜索引擎索引保存在内存中是有问题的，因为内存既昂贵又不稳定。因此，在A部分中，您将编写一些C++代码，利用HW2实现首先构建文件子树的内存索引，然后将该索引以架构中立的格式写入索引文件。

What do we mean by architecture-neutral? Every time we need to store a binary integer in the file's data structure, we will store it in big endian representation. This is the representation that is conventionally used for portability, but the bad news is that this is the opposite representation than most computers you use: x86 computers are little endian. So, you will need to convert integers (whether 16-bit, 32-bit, or 64-bit) into big endian before writing them into the file. We provide you with helper functions to do this.我们所说的建筑中立是什么意思？每次我们需要在文件的数据结构中存储一个二进制整数时，我们都会将其存储在大端表示中。这是传统上用于可移植性的表示法，但坏消息是，这与您使用的大多数计算机相反：x86计算机是小字节序。因此，在将整数写入文件之前，您需要将整数（无论是16位、32位还是64位）转换为大端序。我们为您提供辅助功能来实现这一点。


The good news is that we're going to keep roughly the same data structure inside the file as you built up in memory: we'll have chained hash tables that are arrays of buckets containing linked lists. And, our inverted index will be a hash table containing a bunch of embedded hash tables. But, we need to be very precise about the specific layout of these data structures within the file. So, let's first walk through our specification of an index file's format. We'll do this first at a high level of abstraction, showing the major components within the index file. Then, we'll zoom into these components, showing additional details about each.好消息是，我们将在文件中保留与您在内存中构建的数据结构大致相同的数据结构：我们将拥有链式哈希表，这些哈希表是包含链表的桶数组。而且，我们的倒排索引将是一个包含一堆嵌入式哈希表的哈希表。但是，我们需要非常精确地了解这些数据结构在文件中的具体布局。那么，让我们首先浏览一下索引文件格式的规范。我们将首先在高抽象级别上执行此操作，显示索引文件中的主要组件。然后，我们将放大这些组件，显示每个组件的其他详细信息。

At a high-level, the index file looks like the figure on the right. The index file is split into three major pieces: a header, the doctable, and the index. We'll talk about each in turn.从高层来看，索引文件看起来像右边的图。索引文件分为三个主要部分：标题、文档表和索引。我们将依次讨论每一个。

**`Header`**: an index file's header contains metadata about the rest of the index file.标头：索引文件的标头包含有关索引文件其余部分的元数据。

The first four bytes of the header are a `magic number`, or format indicator. Specifically, we use the 32-bit number 0xCAFEF00D. We will always write the magic number out as the last step in preparing an index file. This way, if the program crashes partway through writing one, the magic number will be missing, and it will be easy to tell that the index file is corrupt.标头的前四个字节是一个幻数或格式指示符。具体来说，我们使用32位数字0xCAFEF00D。在准备索引文件的最后一步，我们总是会写出神奇的数字。这样，如果程序在写入过程中中途崩溃，则魔术数字将丢失，并且很容易判断索引文件已损坏。

The next four bytes are a `checksum` of the doctable and index regions of the file. A checksum is a mathematical signature of a bunch of data, kind of like a hash value. By including a checksum of most of the index file within the header, we can tell if the index file has been corrupted, such as by a disk error. If the checksum stored in the header doesn't match what we recalculate when opening an index file, we know the file is corrupt and we can discard it.接下来的四个字节是文件的文档表和索引区域的校验和。校验和是一组数据的数学签名，有点像哈希值。通过在标头中包含大多数索引文件的校验和，我们可以判断索引文件是否已损坏，例如磁盘错误。如果存储在标头中的校验和与打开索引文件时重新计算的值不匹配，我们就知道文件已损坏，可以丢弃它。

The next four bytes store the size of the doctable region of the file. The size is stored as a 32-bit, signed, big endian integer.接下来的四个字节存储文件的doctable区域的大小。大小存储为32位、有符号、大端序整数。

The final four bytes of the header store the size of the index region of the file, in exactly the same way. 标头的最后四个字节以完全相同的方式存储文件索引区域的大小。



**`Doctable`**: Let's drill down into the next level of detail by examining the content of the doctable region of the file. The doctable is a hash table that stores a mapping from 64-bit document ID to an ASCII string representing the document's filename. This is the docid_to_docname HashTable that you built up in HW2, but stored in the file rather than in memory.让我们通过检查文件的Doctable区域的内容来深入到下一个细节级别。doctable是一个哈希表，它存储了从64位文档ID到表示文档文件名的ASCII字符串的映射。这是您在HW2中构建的docid_to_docname HashTable，但存储在文件中而不是内存中。

The doctable consists of three regions; let's walk through them, and then drill down into some details.文档表由三个区域组成；让我们逐一介绍它们，然后深入了解一些细节。

- `num_buckets`: this region is the simplest; it is just a 32-bit big endian integer that represents the number of buckets inside the hash table, exactly like you stored in your HashTable.这个区域最简单；它只是一个32位大端序整数，表示哈希表内的桶数，与您存储在哈希表中的桶数完全相同。
- `an array of bucket_rec records`: this region contains one record for each bucket in the hash table. A bucket_rec record is 8 bytes long, and it consists of two four-byte fields. The chain len field is a four byte integer that tells you the number of elements in the bucket's chain. (This number might be zero if there are no elements in that chain!) The bucket position field is a four byte integer tells you the offset of the bucket data (i.e., the chain of bucket elements) within the index file. The offset is just like a pointer in memory, or an index of an array, except it points within the index file. So, for example, an offset of 0 would indicate the first byte of the file, an offset of 10 would indicate the 11th byte of the file, and so on.该区域包含哈希表中每个bucket的一条记录。bucket_rec记录的长度为8字节，由两个4字节的字段组成。chain len字段是一个四字节的整数，它告诉存储桶链中的元素数量。（如果该链中没有元素，则该数字可能为零！）bucket位置字段是一个四字节整数，用于告诉您索引文件中bucket数据（即bucket元素链）的偏移量。偏移量就像内存中的指针或数组的索引，除了它指向索引文件内。因此，例如，偏移量0表示文件的第一个字节，偏移量10表示文件的第11个字节，以此类推。
- `an array of buckets`: this region contains one bucket for each bucket in the hash table. A bucket is slightly more complex; it is a little embedded data structure. Specifically, each bucket contains: bucket数组：该区域为哈希表中的每个bucket包含一个bucket。水桶稍微复杂一些；它是一种嵌入式数据结构。具体来说，每个桶都包含：
  - `an array of element positions`: since elements are variable-sized, rather than fixed-sized, we need to know where each element of the bucket lives inside the bucket. For each element, we store a four-byte integer containing the position (i.e., offset) of the element within the index file. 元素位置数组：由于元素的大小是可变的，而不是固定的，我们需要知道桶中的每个元素在桶内的位置。对于每个元素，我们存储一个四字节的整数，其中包含元素在索引文件中的位置（即偏移量）。
  - `an array of elements`: at each position specified in the element positions array lives an element. Since this is the docid-to-filename hash table, an element contains a 64-bit document ID and a filename. The document ID is an unsigned, big endian integer. Next, we store a 16-bit (2 byte) signed, big endian integer that contains the number of characters in the file name. Finally, we store the filename characters (each character is a single ASCII byte). Note that we do NOT store a null-terminator at the end of the filename; since we have the filename length in an earlier field, we don't need it!元素数组：在元素位置数组中指定的每个位置都有一个元素。由于这是docid到文件名的哈希表，因此元素包含一个64位文档ID和一个文件名。文档ID是一个无符号的大端序整数。接下来，我们存储一个16位（2字节）有符号大端序整数，其中包含文件名中的字符数。最后，我们存储文件名字符（每个字符都是一个ASCII字节）。请注意，我们不会在文件名末尾存储空终止符；因为我们在前面的字段中有文件名长度，所以我们不需要它！


- `Index`: The index is the most complicated of the three regions within the index file. The great news is that it has pretty much the same structure as the doctable: it is just a hash table, laid out exactly the same way. The only part of the index that differs from the doctable is the structure of each element. Let's focus on that.索引是索引文件中三个区域中最复杂的一个。好消息是，它的结构与doctable几乎相同：它只是一个哈希表，布局完全相同。索引中与doctable不同的唯一部分是每个元素的结构。让我们专注于此。

An index maps from a word to an embedded docID hash table, or docID table. So, each element of the index contains enough information to store all of that. Specifically, an index table element contains:索引将单词映射到嵌入式docID哈希表或docID表。因此，索引的每个元素都包含足够的信息来存储所有这些信息。具体来说，索引表元素包含：


- a two-byte signed integer that specifies the number of characters in the word.一个两字节的带符号整数，指定单词中的字符数。
- a four-byte signed integer that specifies the number of bytes in the embedded docID table.一个四字节的带符号整数，指定嵌入式docID表中的字节数。
- an array of ASCII characters that represents the word; as before, we don't store a NULL terminator at the end.表示单词的ASCII字符数组；如前所述，我们不会在末尾存储NULL终止符。
- finally, the element contains some variable number of bytes that represents the docID table. So, all we need to understand now is the format of the docID table. We're sure it's format will come as no surprise at this point...最后，该元素包含一些表示docID表的可变字节数。所以，我们现在需要了解的是docID表的格式。我们确信，在这一点上，它的格式不会令人惊讶。。。


- `docIDtable`: like the "doctable" table, each embedded "docIDtable" table within the index is just a hash table! A docIDtable maps from a 64-bit docID to a list of positions with that document that the word can be found in. So, each element of the docID table stores exactly that:与“doctable”表一样，索引中的每个嵌入式“docIDtable”表都只是一个哈希表！docIDtable从64位docID映射到该文档中可以找到单词的位置列表。因此，docID表的每个元素都存储了以下内容：

  - a 64-bit (8-byte) unsigned integer that represents the docID.表示docID的64位（8字节）无符号整数。
  - a 32-bit (4-byte) signed integer that indicates the number of word positions stored in this element.一个32位（4字节）有符号整数，表示存储在该元素中的单词位置的数量。
  - an array of 32-bit (4-byte) signed integers, sorted in ascending order, each one containing a position within the docID that the word appears in.一个32位（4字节）有符号整数数组，按升序排序，每个整数都包含单词在docID中出现的位置。

So, putting it all together, the entire index file contains a header, a doctable (a hash table that maps from docID to filename), and an index. The index is a hash table that maps from a word to an embedded docIDtable. The docIDtable is a hash table that maps from a document ID to a list of word positions within that document. 因此，将所有内容放在一起，整个索引文件包含一个标头、一个doctable（一个从docID映射到文件名的哈希表）和一个索引。索引是一个从单词映射到嵌入式docIDtable的哈希表。

**Instructions**
The bulk of the work in this homework is in this step. We'll tackle it in parts.这个家庭作业的大部分工作都在这一步。我们将分部分解决这个问题。

1. Change to the directory containing your CSE333 GitLab repository. Use git pull to retrieve the new hw3/ folder for this assignment. You still will need the hw1/, hw2/, and projdocs directories in the same folder as your new hw3 folder since hw3/ links to files in those previous directories, and the test_tree folder also still needs to be present.切换到包含CSE333 GitLab存储库的目录。使用git pull检索此分配的新hw3/文件夹。您仍然需要将hw1/、hw2/和projdocs目录与新的hw3文件夹放在同一个文件夹中，因为hw3/链接到这些以前目录中的文件，并且test_tree文件夹也仍然需要存在。
2. Look around inside of hw3/ to familiarize yourself with the structure. Note that there is a libhw1/ directory that contains a symlink to your libhw1.a, and a libhw2/ directory that contains a symlink to your libhw2.a. You can replace your libraries with ours (from the appropriate solution_binaries directories) if you prefer.看看hw3/的内部，熟悉一下它的结构。请注意，有一个libhw1/目录包含指向libhw1.a的符号链接，还有一个libhw2/目录包含指向你的libhw2.a的符号连接。如果你愿意，你可以用我们的库（来自适当的solution_binaries目录）替换你的库。
3. Next, run make to compile the three HW3 binaries. One of them is the usual unit test binary. Run it, and you'll see the unit tests fail, crash out, and you won't yet earn the automated grading points tallied by the test suite.接下来，运行make来编译三个HW3二进制文件。其中之一是常见的单元测试二进制文件。运行它，你会看到单元测试失败、崩溃，你还没有获得测试套件记录的自动评分点。
4. Now, take a look inside `Utils.h` and `LayoutStructs.h`. These header files contains some useful utility routines and classes you'll take advantage of in the rest of the assignment. We've provided the full implementation of Utils.cc. Next, look inside `WriteIndex.h`; this header file declares the WriteIndex() function, which you will be implementing in this part of the assignment. Also, look inside buildfileindex.cc; this file makes use of WriteIndex() and your HW2 CrawlFileTree(), to crawl a file subtree and write the resulting index out into an index file. Try running the solution_binaries/buildfileindex program to build one or two index files for a directory subtree, and then run the solution_binaries/filesearchshell program to try out the generated index file.现在，看看Utils.h和LayoutStructs.h内部。这些头文件包含一些有用的实用程序例程和类，您将在作业的其余部分中利用它们。我们已经提供了Utils.cc的完整实现。接下来，请查看WriteIndex.h；此头文件声明了WriteIndex（）函数，您将在赋值的这一部分实现该函数。此外，请查看buildfileindex.cc内部；此文件使用WriteIndex（）和HW2 CrawlFileTree（）来抓取文件子树，并将结果索引写入索引文件。尝试运行solution_binaries/buildfileindex程序为目录子树构建一个或两个索引文件，然后运行solution.binaries/filesearchshell程序尝试生成的索引文件。
5. Finally, it's time to get to work! Open up WriteIndex.cc and take a look around inside. It looks complex, but all of the helper routines and major functions correspond pretty directly to our walkthrough of the data structures above. Start by reading through WriteIndex(); we've given you part of its implementation. Then, start recursively descending through all the functions it calls, and implement the missing pieces. (Look for STEP: in the text to find what you need to implement.)
```cpp
// Helper function declarations and constants.

static constexpr int kFailedWrite = -1;

// 将 DocTable 写入文件，从指定的 offset 处开始写入，并返回写入的字节数。如果出错，返回负值。
static int WriteDocTable(FILE *f, DocTable *dt, IndexFileOffset_t offset);

// 将 MemIndex 写入文件，从 offset 处开始写入，并返回写入的字节数。如果出错，返回负值。
static int WriteMemIndex(FILE *f, MemIndex *mi, IndexFileOffset_t offset);

//写入索引文件的头部信息，并在最后原子性地写入 kMagicNumber（用于检查完整性）。
// 如果写入成功，返回头部字节数，否则返回负值。
static int WriteHeader(FILE *f, int doctable_bytes, int memidx_bytes);

// 用于 WriteHashTable 的函数指针，负责写入哈希表中的单个键值对（HTKeyValue_t）
typedef int (*WriteElementFn)(FILE *f, IndexFileOffset_t offset,
                              HTKeyValue_t* kv);

// 将哈希表 HashTable 写入索引文件，包括表头、桶记录（BucketRecord）以及桶的内容。
// 它是文件核心的写入逻辑，支持不同类型的哈希表。

static int WriteHashTable(FILE *f, IndexFileOffset_t offset, HashTable *ht,
                          WriteElementFn fn);

// 写入哈希表的桶记录（BucketRecord），包括桶的元素数量和桶在文件中的偏移量。
static int WriteHTBucketRecord(FILE *f, IndexFileOffset_t offset,
                               int32_t num_elts,
                               IndexFileOffset_t bucket_offset);

// 写入哈希表的桶内容，包括元素位置记录（ElementPositionRecords）和实际存储的元素。
// 由于桶是 链表（LinkedList） 形式存储的，所以需要序列化链表元素并写入文件。
static int WriteHTBucket(FILE *f, IndexFileOffset_t offset,
                         LinkedList *li, WriteElementFn fn);



  // STEP 3.
  CRC32 crc;
  // int fseek(FILE *stream, long int offset, int whence);
  fseek (f, sizeof(IndexFileHeader), SEEK_SET);

  for (int i = 0; i < doctable_bytes + memidx_bytes; i++) {
    uint8_t next_byte;
    // size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
    if (fread(&next_byte, sizeof(next_byte), 1, f) != 1) { // &next_byte是next_byte的地址，type是一个指针
      return kFailedWrite;
    }
    crc.FoldByteIntoCRC(next_byte);
  }

  // STEP 6.
  // Write the BucketRecord.
  // size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
  if (fwrite(&bucket_rec, sizeof(BucketRecord), 1, f) != 1) {
    return kFailedWrite;
  }

    // STEP 8.
    // Write the element itself, using fn.
    HTKeyValue_t *kv;
    LLIterator_Get(it, reinterpret_cast<LLPayload_t *>(&kv));
    int elt_byte = fn(f, element_pos, kv);
    if (elt_byte < 0) {
      LLIterator_Free(it);
      return kFailedWrite;
    }

```

### **`reinterpret_cast<>` vs. `static_cast<>`**

| **转换类型** | **C++ 推荐的 cast** | **是否可用 C-style cast?** | **说明** |
| --- | --- | --- | --- |
| `int → double` | ✅ `static_cast<double>(x)` | ✅ `(double)x` | 普通数值转换 |
| `void* → 具体类型指针` | ✅ `static_cast<T*>(p)` | ✅ `(T*)p` | 确保 `p` 原本是 `T*` |
| `int → void*` / `指针 → int` | ✅ `reinterpret_cast<void*>(i)` | ✅ `(void*)i` | 指针和整数转换 |
| `Base* → Derived*` (多态) | ✅ `dynamic_cast<Derived*>(b)` | ❌ `(Derived*)b` | 运行时检查，多态安全转换 |
| `const int* → int*` | ✅ `const_cast<int*>(p)` | ❌ `(int*)p` | 移除 `const` 修饰符 |


6. Once you think you have the writer working, compile and run the test_suite as a first step. Next, use your buildfileindex binary to produce an index file (we suggest indexing something small, like ./test_tree/tiny as a good test case). After that, use the solution_binaries/filesearchshell program that we provide, passing it the name of the index file that your buildfileindex produces, to see if it's able to successfully parse the file and issue queries against it. If not, you need to fix some bugs before you move on! 一旦你认为你有了编写器，编译并运行test_suite作为第一步。接下来，使用您的buildfileindex二进制文件生成一个索引文件（我们建议对一些小的东西进行索引，比如./test_tree/minily作为一个好的测试用例）。之后，使用我们提供的solution_binaries/filesearchshell程序，将buildfileindex生成的索引文件的名称传递给它，看看它是否能够成功解析该文件并对其发出查询。如果不能，您需要在继续之前修复一些错误！

[Aside: If you write the index files to your personal directories on a CSE lab machine or on attu, you may find that the program runs very slowly. That's because home directories on those machines on a network file server, and buildfileindex does a huge number of small write operations, which can be quite slow over the network. To speed things up dramatically we suggest you write the index files into /tmp, which is a directory on a local disk attached to each machine. Be sure to remove the files when you're done so the disk doesn't fill up.] [旁白：如果你将索引文件写入CSE实验室机器或attu上的个人目录，你可能会发现程序运行非常缓慢。这是因为网络文件服务器上这些机器上的主目录，而buildfileindex会执行大量的小写操作，这在网络上可能会非常缓慢。为了大大加快速度，我们建议你将指数文件写入/tmp，这是连接到每台机器的本地磁盘上的一个目录。完成后一定要删除这些文件，这样磁盘就不会填满。]

7. As an even more rigorous test, try running the hw3fsck program we've provided in solution_binaries against the index that you've produced. hw3fsck scans through the entire index, checking every field inside the file for reasonableness. It tries to print out a helpful message if it spots some kind of problem.作为一项更严格的测试，请尝试根据您生成的索引运行我们在solution_binaries中提供的hw3fsck程序。hw3fsck扫描整个索引，检查文件中的每个字段是否合理。如果发现某种问题，它会尝试打印出有用的信息。
Once you pass hw3fsck and once you're able to issue queries against your file indices, then rerun your buildfileindex program under valgrind and make sure that you don't have any memory leaks or memory errors.通过hw3fsck并能够对文件索引发出查询后，请在valgrind下重新运行buildfileindex程序，并确保没有任何内存泄漏或内存错误。

Congrats, you've passed part A of the assignment!

## Part B: Index Lookup
Now that you have a working memory-to-file index writer, the next step is to implement code that knows how to read an index file and lookup query words and docids against it. We've given you the scaffolding of the implementation that does this, and you'll be finishing our implementation.现在您已经有了文件索引编写器的工作内存，下一步是实现知道如何读取索引文件并查找查询词和文档的代码。我们已经为您提供了实现此功能的框架，您将完成我们的实现。

Instructions

1. Start by looking inside `FileIndexReader.h`. Notice that we're now in full-blown C++ land; you'll be implementing constructors, manipulating member variables and functions, and so on. Next, open up `FileIndexReader.cc`. Your job is to finish the implementation of its constructor, which reads the header of the index file and stores various fields as private member variables. As above, look for "STEP:" to figure out what you need to implement. When you're done, recompile and re-run the test suite. You should pass all the tests for test_fileindex_reader.cc once you have implemented FileIndexReader successfully.首先看看FileIndexReader.h内部。请注意，我们现在处于成熟的C++领域；您将实现构造函数、操纵成员变量和函数等。接下来，打开FileIndexReader.cc。您的工作是完成其构造函数的实现，该构造函数读取索引文件的头部并将各种字段存储为私有成员变量。如上所述，查找“STEP：”以找出需要实施的内容。完成后，重新编译并重新运行测试套件。成功实现FileIndexReader后，您应该通过test_fileindex_reader.cc的所有测试。

```cpp
//关键字 explicit 的作用是 防止隐式类型转换，确保构造函数只能通过直接调用来实例化对象，而不能通过隐式转换的方式进行对象创建。
/*
功能：构造函数，打开指定的索引文件，并可选地进行校验。
file_name：索引文件的路径。
validate：是否验证文件的校验和，默认 true。 
*/
explicit FileIndexReader(const std::string &file_name, bool validate = true);
```
2. Next, move on to `HashTableReader.h`. Read through it to see what the class does. Don't worry about the copy constructor and assignment operator details (though if you're curious, read through them to see what they're doing and why). This class serves as a base class for other subclasses. The job of a HashTableReader is to provide most of the generic hash-table lookup functionality; it knows how to look through buckets and chains, returning offsets to elements associated with a hash value. Open up `HashTableReader.cc`. Implement the "STEP:" components in the constructor and the LookupElementPositions function. When you're done, recompile and run the unit tests to see if you pass test_hashtablereader.cc unit test.接下来，转到HashTableReader.h。通读它，看看类做了什么。不要担心复制构造函数和赋值运算符的细节（不过如果你很好奇，可以通读它们，看看它们在做什么以及为什么）。此类用作其他子类的基类。HashTableReader的工作是提供大部分通用哈希表查找功能；它知道如何查看bucket和链，返回与哈希值关联的元素的偏移量。打开HashTableReader.cc。在构造函数和LookupElementPositions函数中实现“STEP:”组件。完成后，重新编译并运行单元测试，看看是否通过了test_hashtablereader.cc单元测试。

3.Now it's time to move on to `DocTableReader.h`. Read through it, and note that it is a subclass of HashTableReader. It inherits LookupElementPositions() and other aspects, but provides some new functionality. Next, open up `DocTableReader.cc`, and implement the "STEP:" functionality. See how well you do on its unit test (and valgrind) when you're done.现在是时候转到DocTableReader.h了。通读一下，注意它是HashTableReader的一个子类。它继承了LookupElementPositions（）和其他方面，但提供了一些新功能。接下来，打开DocTableReader.cc，并实现“STEP：”功能。当你完成它的单元测试（和valgrind）时，看看你做得有多好。

4. Next, lets move on to `IndexTableReader.h`. Read through it and understand its role. Next, open up `IndexTableReader.cc` and implement the "STEP:" functionality. Test against the unit tests (and valgrind).接下来，让我们继续阅读IndexTableReader.h。通读它并了解它的作用。接下来，打开IndexTableReader.cc并实现“STEP:”功能。根据单元测试（和valgrind）进行测试。

5. Next, do the same with `DocIDTableReader.h` and `DocIDTableReader.cc`.接下来，对DocIDTableReader.h和DocIDTableReader.cc执行相同的操作。

6. We're almost there! Open up `QueryProcessor.h` and understand how it is supposed to work. Check out `test_queryprocessor.cc` for more information. Now open up `QueryProcessor.cc` and read through our implementation of the constructor and destructor.我们快到了！打开QueryProcessor.h，了解它应该如何工作。有关更多信息，请查看test_QueryProcessor.cc。现在打开QueryProcessor.cc，阅读构造函数和析构函数的实现。

```cpp
  // This method processes a query against the indices and returns a
  // vector of QueryResults, sorted in descending order of rank.  If no
  // documents match the query, then a valid but empty vector will be
  // returned.
  std::vector<QueryResult> ProcessQuery(
      const std::vector<std::string> &query) const;
// 此方法处理针对索引的查询，并返回一个QueryResults向量，按排名降序排序。如果没有文档与查询匹配，则将返回一个有效但为空的向量。

```

This part of the assignment is the most open-ended. We've given you the function definition for ProcessQuery(), and also a clue about what you should be building up and returning. But, we've given you nothing about its implementation. You get to implement it entirely on your own; you might want to define helper private member functions, you might want to define other structures to help along the way, etc.; it's entirely up to you. But, once you're finished, you'll need to pass our unit test to know you've done it correctly. 这部分作业是最开放的。我们已经为您提供了ProcessQuery（）的函数定义，以及关于应该构建和返回什么的线索。但是，我们没有给你任何关于它的实施情况。你可以完全靠自己实现它；您可能希望定义辅助私有成员函数，您可能希望在此过程中定义其他结构以提供帮助，等等。；这完全取决于你。但是，一旦你完成了，你需要通过我们的单元测试才能知道你做得对。

As a hint, you should be able to take inspiration from what you did to implement the query processor in HW2. Here, it's only a little bit more complicated. You want to process the query against each index, and then intersect each index's results together and do a final sort (use the STL's sort). Remember that processing a query against an index means ensuring all query words are present in each matching document, and remember how ranking works. Then, once you have query results from each index, you'll append them all together to form your final query results.作为提示，您应该能够从HW2中实现查询处理器的过程中获得灵感。在这里，它只是稍微复杂一点。您希望对每个索引处理查询，然后将每个索引的结果相交在一起并进行最终排序（使用STL的排序）。记住，根据索引处理查询意味着确保所有查询词都出现在每个匹配的文档中，并记住排名是如何工作的。然后，一旦您从每个索引中获得了查询结果，您将把它们全部附加在一起，形成最终的查询结果。

One more hint, once you think you have this working, move on to Part C and finish our filesearchshell implementation. You'll be able to test the output of your filesearchshell against ours (in solution_binaries/) as a final sanity check.还有一个提示，一旦你认为你已经实现了这一点，请继续进行C部分并完成我们的文件搜索外壳实现。作为最终的健全性检查，您将能够根据我们的（在solution_binaries/中）测试您的文件搜索外壳的输出。

Also, now would be a great time to run valgrind over the unit tests to verify you have no memory leaks or memory errors.此外，现在是在单元测试中运行valgrind以验证没有内存泄漏或内存错误的好时机。

You're done with part B!

## Part C: Search Shell
For Part C, your job is to implement a search shell, just like in HW2, but this time using your HW3 infrastructure you completed parts A and B.对于C部分，你的工作是实现一个搜索shell，就像在HW2中一样，但这次使用HW3基础设施，你完成了a和B部分。

1. Open up filesearchshell.cc and read through it. Note that unlike parts A and B, we have given you almost nothing about the implementation of the filesearchshell besides a really long (and hopefully helpful) comment. Implement filesearchshell.cc.打开filesearchshell.cc并通读一遍。请注意，与A和B部分不同，除了一条非常长（希望有用）的注释外，我们几乎没有给你任何关于filesearchshell实现的信息。实现文件earchshell.cc。

2. Try using your filesearchshell binary. You can compare the output of your binary against a transcript of our solution. The transcripts should match precisely, except perhaps for the order of equally ranked matches. You can also walk your filesearchshell against a very tiny index -- tiny.idx -- in the debugger to see if it's reading the correct fields and jumping to the correct offsets.尝试使用您的文件搜索shell二进制文件。您可以将二进制文件的输出与我们解决方案的副本进行比较。成绩单应该精确匹配，除了排名相同的比赛顺序。您还可以在调试器中针对非常小的索引tiny.idx遍历文件搜索外壳，查看它是否读取了正确的字段并跳转到正确的偏移量。

3. Also, note that you can hit control-D to exit the filesearchshell. filesearchshell ought to clean up all allocated memory before exiting. So, run your filesearchshell under valgrind to ensure there are no leaks or errors.此外，请注意，您可以点击control-D退出文件搜索外壳。filesearchshell应该在退出之前清理所有分配的内存。因此，在valgrind下运行您的文件搜索shell，以确保没有泄漏或错误。

Congrats, you're done with (the mandatory parts) of HW3!!