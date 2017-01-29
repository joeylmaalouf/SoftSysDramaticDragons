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
* Griffiths, David; Griffiths, Dawn. *Head First C*. O'Reilly Media, 2012.
* Ramey, C. "Bash Reference Manual." Bash Reference Manual. N.p., n.d. Web. 29 Jan. 2017. <https://www.gnu.org/software/bash/manual/bashref.html>.
