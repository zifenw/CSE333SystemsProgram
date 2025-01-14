### Gitlab Guide
[Link] (https://courses.cs.washington.edu/courses/cse333/25wi/resources/git_tutorial.html)

### hw0 partA Git
[Link](https://courses.cs.washington.edu/courses/cse333/25wi/hw/hw0/hw0.html)
```
[zifenw@attu3 ~]$ cat ~/.ssh/id_rsa.pub
cat: /homes/iws/zifenw/.ssh/id_rsa.pub: No such file or directory
[zifenw@attu3 ~]$ ssh-keygen -t rsa -C "zifenw@cs.washington.edu"
Generating public/private rsa key pair.
Enter file in which to save the key (/homes/iws/zifenw/.ssh/id_rsa): /homes/iws/zifenw/.ssh/id_rsa
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /homes/iws/zifenw/.ssh/id_rsa
Your public key has been saved in /homes/iws/zifenw/.ssh/id_rsa.pub
The key fingerprint is:
SHA256:OEl7jzxW+7TiDsu3xq9/uXQKjRADkvuqiQF7Ck/uMEU zifenw@cs.washington.edu
The key's randomart image is:
+---[RSA 3072]----+
|      ...        |
|      .. .       |
|  E   ..  o      |
| .   ..+   o     |
|. .   =.S o      |
| +     +.+ o o   |
|= +    .*.o + o..|
|.O o ..o +=o +oo |
|..= o.  o==*=.o. |
+----[SHA256]-----+
[zifenw@attu3 ~]$ cat ~/.ssh/id_rsa.pub
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQC1fuvNQ5v6lkUml0y7Xp4H0ZGgZjCiIGdAg9xjynvo1ZfeXGdWPhQN/Pc5611jt+xjGPETPogvIRMOeaqfAl07kwM7vqTYBf9AA/fmSGNYqrNuSukn/yW5OHtNZd7kSxBFWvnoZ4T9uGJFcZ7i5Z0WQ+k657gjf5nto+ZakaaPnnkonyopwOGvyAZS0j7l3HOWH47nEPsIHp9PVYSplmQjSzaQws6YTt7ob5IA226ZENb/xOJ5ykIHkAgTHh6dNpmHF3Sv7AzFrRy5fYfP4e0iCGJhbmHAr6EXWFLiW9dIOj+kFmypya9B2tgw68qgcxkY6mU4RH6uCk6JmTGO0OWAwwomx6XQkZphbx5b2RdsBS/XN54Yyy8z5XmtufekHfcHoUtStspPhMuRjAjXjdjem//Bgu+zJ2D9HSe8HYxX5qIglL4KZjMJ+bNALHSicanoqb29Elv7RW7m1d16+lODLkIxETn9X4XKCeiJiqekn+R+tOzj4YQKcNxjOyfrGw8= zifenw@cs.washington.edu

[zifenw@attu3 ~]$ git config --global user.name "Zifeng Wang"
[zifenw@attu3 ~]$ git config --global user.email zifenw@cs.washington.edu
[zifenw@attu3 ~]$ git config --global push.default simple

[zifenw@attu3 ~]$ git clone git@gitlab.cs.washington.edu:cse333-25wi-students/cse333-25wi-zifenw.git

[zifenw@attu3 ~]$ cd cse333-25wi-zifenw/
[zifenw@attu3 cse333-25wi-zifenw]$ touch README.md
[zifenw@attu3 cse333-25wi-zifenw]$ git status
[zifenw@attu3 cse333-25wi-zifenw]$ git add README.md
[zifenw@attu3 cse333-25wi-zifenw]$ git commit -m "First Commit"
[zifenw@attu3 cse333-25wi-zifenw]$ git push


[zifenw@attu3 hw0]$ git rm ../README.md hello_world
[zifenw@attu3 hw0]$ git commit -m "Remove README.md and hello_world files"
[zifenw@attu3 hw0]$ git push

  bash% git pull
  bash% make clean
  bash$ git status

bash% git tag hw0-final
bash% git push --tags 


To eliminate the hw0-final tag, do this (this should not normally be necessary):

  bash% git tag -d hw0-final
  bash% git push origin :refs/tags/hw0-final 
``` 
### hw0 partB Unix-based editor
```
[zifenw@attu3 cse333-25wi-zifenw]$ ls -a
[zifenw@attu3 hw0]$ make
[zifenw@attu3 hw0]$ vim hello_world.c
```
### hw0 partC GDB
```
[zifenw@attu3 hw0]$ gdb ./hello_world
1. 设置断点(在 hello_world.c 文件的第 19 行设置了一个断点)
(gdb) break 19
Breakpoint 1 at 0x401143: file hello_world.c, line 19.

2. 运行程序
(gdb) run
Starting program: /homes/iws/zifenw/cse333-25wi-zifenw/hw0/hello_world
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".

Breakpoint 1, main (argc=1, argv=0x7fffffffe1c8) at hello_world.c:19
19        printf("The magic word is: %X\n", a + b);
Missing separate debuginfos, use: dnf debuginfo-install glibc-2.34-125.el9_5.1.x86_64

3.查看 argv[0]      **p can instead of print**
argv[0] 通常是执行程序时给定的程序名或路径。它是命令行参数数组中的第一个元素。
(gdb) print argv[0]
$1 = 0x7fffffffe472 "/homes/iws/zifenw/cse333-25wi-zifenw/hw0/hello_world"

4.查看变量 a 的值
(gdb) print a
$2 = -889262067

5.查看 a 的十六进制表示
(gdb) print /x a
$3 = 0xcafef00d

**十六进制表示 (/x)**
(gdb) print /x a
$3 = 0xcafef00d

**十进制表示 (/d)**
(gdb) print /d a
$4 = 3405691582

**二进制表示 (/t)**
(gdb) print /t a
$5 = 1101001111101111010000001101

**字符表示 (/c)**
(gdb) print /c a
$6 = 'A'

**字符串表示 (/s)**
(gdb) print /s a
$7 = "Hello, world!"

**浮点数表示 (/f)**
(gdb) print /f a
$8 = 3.14159

** 地址格式 (/a)**
(gdb) print /a a
$9 = 0x7fffffffe5c0


6.查看 a + b 的值
(gdb) print a+b
$4 = -559038737

7.显示调用栈        **bt can instead of backtrace**
(gdb) backtrace
它列出了当前执行的函数以及每个函数的调用位置。在此例中，调用栈仅显示了 main 函数，表示当前程序的执行位置。
#0  main (argc=1, argv=0x7fffffffe1c8) at hello_world.c:19

8.继续程序执行
(gdb) continue
Continuing.
The magic word is: DEADBEEF
[Inferior 1 (process 528665) exited normally]

9.退出 GDB
(gdb) quit
[zifenw@attu3 hw0]$
```
### hw0 partD Valgrind
```
测试代码是否存在内存泄漏或其他内存错误，例如读取未初始化变量的值。

[zifenw@attu3 hw0]$ valgrind --leak-check=full ./hello_world
```
### hw0 partE check for style issues
```
[zifenw@attu3 hw0]$ python3 cpplint.py --clint hello_world.c
```
