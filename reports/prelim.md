# Project 1 Preliminary Report
### Software Systems
##### Joey, Apurva, Sean

#### Project Goals

[updated version of same section in proposal]

#### Project-Related Cards

* HFC 2.5
  * [Joey's work](https://github.com/joeylmaalouf/ExercisesInC/tree/master/exercises/ex02.5)
  * [Apurva's work]()
  * [Sean's work](https://github.com/SeanCCarter/ExercisesInC/tree/master/exercises/ex02.5)
* HFC 9
  * [Joey's work](https://github.com/joeylmaalouf/ExercisesInC/tree/master/exercises/ex09)
  * [Apurva's work]()
  * [Sean's work](https://github.com/SeanCCarter/ExercisesInC/tree/master/exercises/ex09)
* ThinkOS 6
  * [Joey's work](https://github.com/joeylmaalouf/ExercisesInC/blob/master/reading_questions/thinkos.md#chapter-6)
  * [Apurva's work]()
  * [Sean's work](https://github.com/SeanCCarter/ExercisesInC/blob/master/reading_questions/thinkos6.md)
* Parsing
  * [`parse` function](../project1/shell.c#L9)

  #### Preliminary Results

Interactive mode:

```
$ ./shell
» ls
shell shell.c test.txt
» ls -lsa
total 32
 4 drwxrwxr-x 2 joey joey  4096 Feb  6 18:35 .
 4 drwxrwxr-x 5 joey joey  4096 Jan 29 17:30 ..
16 -rwxrwxr-x 1 joey joey 13666 Feb  6 18:35 shell
 4 -rw-rw-r-- 1 joey joey  4027 Feb  6 18:52 shell.c
 4 -rw-rw-r-- 1 joey joey   112 Feb  6 18:15 test.txt
» echo  "hello, world!"
hello, world!
» pwd
/home/joey/github/SoftSysDramaticDragons/project1
```

Batch mode (see [`test.txt`](../project1/shell.c#L9)):

```
$ ./shell test.txt
/home/joey/github/SoftSysDramaticDragons/project1
   hello  world; how are you? #;_;#
I'm doing well :)
total 32
 4 drwxrwxr-x 2 joey joey  4096 Feb  6 18:35 .
 4 drwxrwxr-x 5 joey joey  4096 Jan 29 17:30 ..
16 -rwxrwxr-x 1 joey joey 13666 Feb  6 18:35 shell
 4 -rw-rw-r-- 1 joey joey  4027 Feb  6 18:52 shell.c
 4 -rw-rw-r-- 1 joey joey   112 Feb  6 18:15 test.txt
PING google.com (172.217.2.14) 56(84) bytes of data.
64 bytes from lga15s45-in-f14.1e100.net (172.217.2.14): icmp_seq=1 ttl=56 time=42.3 ms
64 bytes from lga15s45-in-f14.1e100.net (172.217.2.14): icmp_seq=2 ttl=56 time=20.4 ms
64 bytes from lga15s45-in-f14.1e100.net (172.217.2.14): icmp_seq=3 ttl=56 time=42.1 ms
^C
--- google.com ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2002ms
rtt min/avg/max/mdev = 20.432/34.962/42.316/10.277 ms
```

  #### Annotated Bibliography

[updated version of same section in proposal]

  #### Product Backlog

https://trello.com/b/aKHeJdbc/softsysdramaticdragons

  #### Roadblocks

[updated version of same section in proposal]
