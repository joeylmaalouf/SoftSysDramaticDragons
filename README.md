# SoftSys Dramatic Dragons

## Project 1: Shell

#### Joey Maalouf, Apurva Raman, Sean Carter

### About `shell`
`shell` is a Unix shell written in C modeled after `bash`. It supports a batch mode and loading specific batch configuration files, an interactive mode, aliasing, and running processes. For more details, see our [full report on the project website]().

### Getting Started
To run `shell`, you do not need anything that does not come with a typical Unix environment.

Once you clone this repository, enter the directory with this repository and run:

    cd project1; make

### Usage

#### Running the shell
After running `make`, you can run `shell` with

    ./shell [BATCHFILE]
for batch mode or just

    ./shell
for interactive mode.

#### Testing

To run our tests for the code, run:

    ./shell test.txt

This tests that the code correctly:
  * executes processes
  * parses arguments to commands
  * parses commands separated by semicolons
  * interprets multiline quotes
  * ignores comments

### License
This software is provided under the [MIT License](https://github.com/joeylmaalouf/SoftSysDramaticDragons/blob/master/LICENSE.md).
