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
### STEP 1: initialize the newly allocated record structure.
  ll->num_elements = 0;
  ll->head = NULL;
  ll->tail = NULL;
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
###
###