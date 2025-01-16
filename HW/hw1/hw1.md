## Goals
For Homework #1, you will finish our implementation of two C data structures: a `doubly-linked list` (Part A) and a `chained hash table` (Part B). You will gain experience and proficiency with C programming, particularly `memory management`, `pointers`, and `linked data structures`.

## Part A: Doubly-Linked List

Each node in a doubly-linked list has three fields: `a payload`,` a pointer to the previous element in the list` (or NULL if there is no previous element), and `a pointer to the next element in the list`.  If the list is empty, there are no nodes. If the list has a single element, both of its next and previous pointers are NULL.



这意味着list节点中的payload元素需要是list实现的客户提供的指针  
鉴于指针可能指向客户malloc'ed的内容，这意味着我们可能需要在列表被销毁时帮助客户释放负载  
C不是垃圾收集语言：您自己负责管理内存分配和释放。

### LinkedList
包含链接列表元数据的结构，例如头指针和尾指针。当我们的客户要求我们分配一个新的空链表时，我们malloc并初始化这个结构的一个实例，然后将指向这个malloc'ed结构的指针返回给客户。

### LinkedListNode
此结构表示双链接列表中的节点。它包含一个用于隐藏（指向）客户提供的有效负载的字段，以及指向列表中上一个和下一个LinkedListNode的字段。当客户请求我们向链表中添加元素时，我们将malloc一个新的LinkedListNode以存储指向该元素的指针，执行将LinkedListNode拼接到数据结构中的操作，并更新LinkedList的元数据。
### LLIterator
有时客户希望浏览链接列表；为了帮助他们做到这一点，我们为他们提供了一个迭代器。LLIterator包含与迭代器关联的簿记。特别是，它跟踪与迭代器关联的列表以及迭代器当前指向的列表中的节点。请注意，这里存在一致性问题：如果客户通过删除节点来更新链接列表，则某些现有迭代器可能由于引用了已删除的节点而变得不一致。所以，我们让我们的客户承诺，他们将释放任何活迭代器之前，变异的链表。（因为我们很慷慨，所以我们允许客户保留一个迭代器，如果使用该迭代器进行了变异。）当客户请求一个新的迭代器时，我们malloc一个实例并将指向它的指针返回给客户。
### Instructions
1. 确保您熟悉C指针、结构、malloc和free
2. 获取hw1的源文件。$ git pull
3. Look inside the hw1 directory.  
- Makefile: 在CSE-Linux机器上，可以使用Linux命令make编译赋值的Makefile。
- LinkedList.h: 定义和记录链接列表API的头文件。链接列表的客户包括此头文件并使用中定义的函数。请仔细阅读此头文件以了解链接列表的预期行为。
- LinkedList_priv.h: LinkedList.c包含的私有头文件；它定义了我们上面绘制的结构。通过将这个头的内容直接放在LinkedList.c中，这些实现细节通常会从客户机中保留下来；但是，我们选择将它们放在“private.h”中，这样我们的单元测试代码就可以验证链表内部的正确性。
- LinkedList.c: 包含部分完成的双链接列表的实现。您的任务将是完成实施。找到标有“步骤X：”的标签——这些标签标识您将要完成的实现的缺失部分。
- example_program_ll.c: 这是一个客户如何使用链表的简单示例；在其中，您可以看到客户如何分配链表、向链表中添加元素、创建迭代器、使用迭代器进行一点导航，然后进行清理。
- test_linkedlist.cc: 此文件包含我们编写的单元测试，以验证链表实现是否正常工作。单元测试是使用Google测试单元测试框架编写的，它与Java的JUnit测试框架有相似之处。此外，此测试驱动程序将帮助课程工作人员对作业进行评分。当您将更多的部分添加到实现中时，测试驱动程序将进一步完成单元测试，并将打印出一个累积分数。您不需要了解此任务的测试驱动程序中有什么内容，但是如果您查看一下其中的内容，您可能会得到关于您应该在实现中执行哪些操作的提示！
- solution_binaries: 在这个目录中，您将找到一些Linux可执行文件。这些二进制文件是用完整的工作版本LinkedList.c编译的；您可以运行它们来探索当您的作业工作时应该显示什么！
4. Run `make` on a CSE Linux machine。
5. Try `./test_suite` and `./example_program_ll`
6. 完成LinkedList.c的实现。浏览LinkedList.c，找到每个写有“步骤X”的注释，并将工作代码放在那里. `自上而下通读代码` 和 `经常重新编译` 会有帮助
### STEP 1: LinkedList_Allocate
```c
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
  ```
### 函数指针
return_type (*pointer_name)(parameter_list);
```c
//(1) 定义一个普通函数
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}
//(2) 声明和初始化函数指针
int (*func_ptr)(int, int);  // 声明一个函数指针
func_ptr = &add;            // 将函数地址赋值给指针

//(3) 使用函数指针调用函数
int result = func_ptr(3, 4);  // 调用函数
printf("Result: %d\n", result);  // 输出结果：7

```
```c
//(1) 定义一个普通函数
void ExamplePayload_Free(LLPayload_t payload) {
  Verify333(payload != NULL);
  free(payload);
}
//(2) 定义类型是函数指针
typedef void(*LLPayloadFreeFnPtr)(LLPayload_t payload); //定义类型是函数指针

void LinkedList_Free(LinkedList *list,
                     LLPayloadFreeFnPtr payload_free_function);

//(3) 使用函数指针调用函数
//&ExamplePayload_Free 将函数地址赋值给指针
LinkedList_Free(list, &ExamplePayload_Free);
```
### 结构
![示例图片](./LinkedList.png)
### STEP 2: LinkedList_Free

```c
void LinkedList_Free(LinkedList *list,
                     LLPayloadFreeFnPtr payload_free_function) {
  Verify333(list != NULL);
  Verify333(payload_free_function != NULL);

  // STEP 2: sweep through the list and free all of the nodes' payloads
  // (using the payload_free_function supplied as an argument) and
  // the nodes themselves.
  while(list->head != NULL){
    //using function pointer free the head's payload:
    //function is ExamplePayload_Free(LLPayload_t payload) 
    //can be found in example_problem_ll.c
    payload_free_function(list->head->payload);
    //save current node pointer to temp
    LinkedListNode* temp = list->head;
    //move head pointer to next
    list->head = list->head->next;
    //free the temp pointer
    free(temp);
  }  
  // free the LinkedList
  free(list);
}

```
### STEP 3: LinkedList_Push

```C
void LinkedList_Push(LinkedList *list, LLPayload_t payload) {
  Verify333(list != NULL);

  // Allocate space for the new node.
  LinkedListNode *ln = (LinkedListNode *) malloc(sizeof(LinkedListNode));
  Verify333(ln != NULL);

  // Set the payload
  ln->payload = payload;

  if (list->num_elements == 0) {
    // Degenerate case; list is currently empty
    Verify333(list->head == NULL);  //if not NULL can continue
    Verify333(list->tail == NULL);
    ln->next = ln->prev = NULL;
    list->head = list->tail = ln;
    list->num_elements = 1;
  } else {
    // STEP 3: typical case; list has >=1 elements
    //make sure head and tail not empty
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
```
### STEP 4: LinkedList_Pop
```c
//payload_ptr不是链表操作的正式输入数据，而是用于将弹出的节点负载（payload）保存到变量中，以便进行后续验证和处理。
bool LinkedList_Pop(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  // STEP 4: implement LinkedList_Pop.  
  // pop false for empty list
  if (list->num_elements == 0){
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
  //pop succeeded
  return true;
}
```
### STEP 5: LinkedList_Append
```c
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
  } else{

  // the case that list is not empty
  //make sure head and tail not empty
  Verify333(list->head != NULL);
  Verify333(list->tail != NULL);

  //let ln become tail
  ln->next = NULL;
  ln->prev = list->tail;
  list->tail->next = ln;
  list->tail = ln;
  list->num_elements += 1;
  } 
}
```
### 迭代器（Iterator）
迭代器的作用
1. 逐个访问容器的元素  
迭代器提供了一种统一的方式来遍历容器，而不需要关心容器的内部结构。

2. 支持不同类型的容器  
无论是数组、链表、哈希表还是树，只要容器支持迭代器，用户就可以通过相同的方式访问其元素。

3. 提高代码的可读性和可维护性  
使用迭代器可以避免繁琐的索引操作或遍历逻辑，使代码更加直观和易懂。

迭代器的工作原理  
迭代器通常是一个对象，它包含了以下三个主要操作：

- 初始化：指向集合的第一个元素。
- 移动：跳转到集合中的下一个元素。
- 访问：获取迭代器当前指向的元素值。

```c
typedef struct ll_iter {
  LinkedList       *list;  // the list we're for
  LinkedListNode   *node;  // the node we are at, or NULL if broken
} LLIterator;
```
### STEP 6: LLIterator_Next
```c
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
```
### STEP 7: LLIterator_Remove
```c
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
  if(iter->list->num_elements == 1){
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
  
  if(iter->node->prev == NULL){
    //iter points at head    iter->node == iter->list->head
    iter->node = iter->node->next;
    iter->node->prev = NULL;
    iter->list->head = iter->node;
  } else if(iter->node->next == NULL){
    //iter points at tail
    iter->node = iter->node->prev;
    iter->node->next = NULL;
    iter->list->tail = iter->node;
  } else {
    //normal case
    iter->node->prev->next = iter->node->next;
    iter->node->next->prev = iter->node->prev;
    iter->node = iter->node->next;
  }

  free(temp);

  return true;  // you may need to change this return value
}
```
### STEP 8: implement LLSlice
```c
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
    //only have one node
    list->head = NULL;
    list->tail = NULL;
  } else {
    //normal case
    list->tail = list->tail->prev;
    list->tail->next = NULL;
  }

  list->num_elements -= 1;
  //free the old tail
  free(temp);
  // return true since slice succeed
  return true;
}
```