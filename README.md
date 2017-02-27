# SoftSys Dramatic Dragons

## Project 1: Shell

#### Joey Maalouf, Apurva Raman, Sean Carter

### About `shell`

`shell` is a Unix shell written in C and modeled after `bash`. It supports an interactive mode, a batch file mode, configuration files, aliasing, and more. For further details, see our [full report on the project website](https://joeylmaalouf.github.io/SoftSysDramaticDragons).


### Getting Started

Once you clone this repository, enter its directory and run:

````
cd project1
make shell
```

All of `shell`'s dependencies are standard C libraries.

### Usage

#### Running the shell

After running `make`, you can run `shell` with
```
./shell [batchfile1] ... [batchfileN]
```
for batch mode or just
```
./shell
```
for interactive mode.

If a `.shellrc` exists in the current directory, its contents will be run before processing any other file or user input; this is useful for configurating and setting up an environment, with support for things like aliases and a custom prompt.


#### Testing

To run our tests for the code, run:

    ./shell test.sh

This tests that the code correctly:
  * executes processes
  * parses arguments to commands
  * parses commands separated by semicolons
  * interprets multiline quotes
  * ignores comments
  * ignores repeated whitespace between tokens


### License

This software is provided under the [MIT License](https://github.com/joeylmaalouf/SoftSysDramaticDragons/blob/master/LICENSE.md).
