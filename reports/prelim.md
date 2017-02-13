# Project 1 Preliminary Report
### Software Systems
##### Joey, Apurva, Sean

#### Project Goals

For this project, we want to write our own shell. At minimum, it should be able to read input, either from the user directly or from a given text file, and execute any corresponding program with the right arguments. Thus far, it can do so, with the exception of a few "unique" commands, like `cd`, that require us to keep track of specific states or other information. As a stretch goal, we would like to include such features as piping the output from one command to the input of another, redirecting it to a file, globbing the current directory for files, or using configuration files to store user-defined aliases and custom prompts.

#### Project-Related Cards

* HFC 2.5
  * [Joey's work](https://github.com/joeylmaalouf/ExercisesInC/tree/master/exercises/ex02.5)
  * [Apurva's work](https://github.com/apurvaraman/ExercisesInC/tree/master/exercises/ex02.5)
  * [Sean's work](https://github.com/SeanCCarter/ExercisesInC/tree/master/exercises/ex02.5)
* HFC 9
  * [Joey's work](https://github.com/joeylmaalouf/ExercisesInC/tree/master/exercises/ex09)
  * [Apurva's work](https://github.com/apurvaraman/ExercisesInC/tree/master/exercises/ex09)
  * [Sean's work](https://github.com/SeanCCarter/ExercisesInC/tree/master/exercises/ex09)
* ThinkOS 6
  * [Joey's work](https://github.com/joeylmaalouf/ExercisesInC/blob/master/reading_questions/thinkos.md#chapter-6)
  * [Apurva's work](https://github.com/apurvaraman/ExercisesInC/blob/master/reading_questions/thinkos.md#chapter-6-complete)
  * [Sean's work](https://github.com/SeanCCarter/ExercisesInC/blob/master/reading_questions/thinkos6.md)
* Parsing
  * [fleshed out `parse` function further](../project1/shell.c#L9)
  * [debugged issues with whitespace, empty args, quotes, and comments](../../../commit/c195a1fbe022441f8d5f9369323a9cb920a10375)

  #### Preliminary Results

Interactive mode:

```
$ ./shell
» ls
shell shell.c test.txt
» ls -lsa
total 36
 4 drwxrwxr-x 2 joey joey  4096 Feb 13 11:48 .
 4 drwxrwxr-x 5 joey joey  4096 Feb 13 11:35 ..
16 -rwxrwxr-x 1 joey joey 13666 Feb 13 11:48 shell
 8 -rw-rw-r-- 1 joey joey  4127 Feb 13 11:48 shell.c
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
total 36
 4 drwxrwxr-x 2 joey joey  4096 Feb 13 11:48 .
 4 drwxrwxr-x 5 joey joey  4096 Feb 13 11:35 ..
16 -rwxrwxr-x 1 joey joey 13666 Feb 13 11:48 shell
 8 -rw-rw-r-- 1 joey joey  4127 Feb 13 11:48 shell.c
 4 -rw-rw-r-- 1 joey joey   112 Feb  6 18:15 test.txt
PING google.com (172.217.0.46) 56(84) bytes of data.
64 bytes from lga15s43-in-f46.1e100.net (172.217.0.46): icmp_seq=1 ttl=56 time=17.6 ms
64 bytes from lga15s43-in-f46.1e100.net (172.217.0.46): icmp_seq=2 ttl=56 time=22.4 ms
64 bytes from lga15s43-in-f46.1e100.net (172.217.0.46): icmp_seq=3 ttl=56 time=19.1 ms
^C
--- google.com ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2003ms
rtt min/avg/max/mdev = 17.670/19.762/22.428/1.990 ms
```

#### Annotated Bibliography

* Downey, Allen. *ThinkOS*. Green Tea Press, 2015.

Relevant chapters for this project we identified this sprint were 2.5, 3, and 9. We have covered 2.5 and 9 thoroughly, helping us with managing strings and processes, and plan to make use of our `tee` code for 3 for when we implement redirection in the future, as well as revisit 2.5 for globbing.

* Griffiths, David; Griffiths, Dawn. *Head First C*. O'Reilly Media, 2012.

This sprint, we looked a lot at chapter 6, focusing on memory management. In particular, we emphasized dynamically allocating memory based on the length of the command received, and properly freeing the used memory afterwards.

* "Programming Assignment 1: The Unix Shell." CS 537: Operating Systems -- Schedules. University of Wisconsin-Madison, 19 Jan. 2007. Web. 30 Jan. 2017. <http://pages.cs.wisc.edu/~dusseau/Classes/CS537-F07/Projects/P1/p1.html>.

We used this assignment to give us a sense of what making a shell would be like and the core features to implement (batch and interactive input, execution, forking, basic parsing). This gave us the idea for the project. It also gave us a sense of how to write a specification for our shell.

* Ramey, C. "Bash Reference Manual." Bash Reference Manual. N.p., n.d. Web. 29 Jan. 2017. <https://www.gnu.org/software/bash/manual/bashref.html>.

We used this to identify what features a fully functional shell might have that we could implement in our own shell.

* Rodriguez-Rivera, Gustavo A. Junipero, and Justin Ennen. "Chapter 5. Writing Your Own Shell." Book Online: Introduction to Systems Programming: a Hands-on Approach. Purdue University, 2014. Web. 30 Jan. 2017. <https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf>.

This explained to us additional user actions to consider, especially when dealing with piping and redirections. It also details a robust parser that gives us a complete grammar of what all potential inputs to the shell look like. We don't expect to do anything as complete, but it gives us a list to choose from in terms of what to support.

#### Product Backlog

https://trello.com/b/aKHeJdbc/softsysdramaticdragons

#### Potential Roadblocks

* Keeping track of a lot of state information for specific commands (like `cd`) can get very messy, very quickly.
* We don't currently feel like we know enough regex to properly do globbing, so that will require more research in the future.
