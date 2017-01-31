# Project 1 Proposal
### Software Systems
##### Joey, Apurva, Sean

#### Project Goals

For this project, we want to write our own shell. At minimum, it should be able to read input, either from the user directly or from a given text file, and execute any corresponding program with the right arguments. As a stretch goal, we would like to include bonus features like piping the output from one command to the input of another, or using configuration files to store user-defined aliases.

#### Current Project Progress

Thus far, we have written code to successfully parse user input through both an interactive mode and a batch file mode. Our input parsing code is capable of reading commands delimited by semicolons, newlines, or null-terminators and separating arguments within a single command on spaces or tabs. We implemented a variation of this parsing that supports quotation blocks as a single token and handles repeated whitespace without creating extraneous tokens. After parsing, our shell can then fork a process and execute the commands from the parsed input.

#### Product Backlog

https://trello.com/b/aKHeJdbc/softsysdramaticdragons

#### Annotated Bibliography

* Downey, Allen. *ThinkOS*. Green Tea Press, 2015.

Relevant chapters for this project we identified were 1, 2, 6, and 9. We have covered 1 and 2 thoroughly and are getting familiar with memory management (6) although we are still working towards getting used to handling memory. Threading (9) is something we plan to read to see if we want to implement threads in our shell.

* Griffiths, David; Griffiths, Dawn. *Head First C*. O'Reilly Media, 2012.

Relevant chapters for this project we identified were 1, 2, 6, 9, and 10. We have covered 1 and 2 thoroughly. We went through 9 together to understand forking to make child processes for our shell. We plan on looking at 6 to improve our handling of memory. We plan on reading 10 to interact with the child processes to implement things like typing.

* "Programming Assignment 1: The Unix Shell." CS 537: Operating Systems -- Schedules. University of Wisconsin-Madison, 19 Jan. 2007. Web. 30 Jan. 2017. <http://pages.cs.wisc.edu/~dusseau/Classes/CS537-F07/Projects/P1/p1.html>.

We used this assignment to give us a sense of what making a shell would be like and the core features to implement (batch and interactive input, execution, forking, basic parsing). This gave us the idea for the project. It also gave us a sense of how to write a specification for our shell.

* Ramey, C. "Bash Reference Manual." Bash Reference Manual. N.p., n.d. Web. 29 Jan. 2017. <https://www.gnu.org/software/bash/manual/bashref.html>.

We used this to identify what features a fully functional shell might have that we could implement in our own shell.

* Rodriguez-Rivera, Gustavo A. Junipero, and Justin Ennen. "Chapter 5. Writing Your Own Shell." Book Online: Introduction to Systems Programming: a Hands-on Approach. Purdue University, 2014. Web. 30 Jan. 2017. <https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf>.

This explained to us additional user actions to consider, especially when dealing with piping and redirections. It also details a robust parser that gives us a complete grammar of what all potential inputs to the shell look like. We don't expect to do anything as complete, but it gives us a list to choose from in terms of what to support.

#### Potential Roadblocks

* Parsing more complicated expressions
* Keeping track of responses from child processes for things like redirection
* Making sure the entire group is on the same page regarding our understanding of the code and any relevant topics, so that we can all work efficiently
