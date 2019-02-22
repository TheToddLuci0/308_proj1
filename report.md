# Summary
This project served as an excellent introduction to child processes and the `exec()` function family. One of the key things was the use of C macros to get data about child processes after they have exetied. I also used this project as an opportunity to experiment with `argp`, the GNU argument parser. In short, this project was an excelent learning experiencs.

# Program Structure
My program is broken into three blocks. First, the argp section, which uses the GNU argp library to handle command line options (in this case the `-p [prompt]` flag), the `parse_line()` function, which handles the parsing and execing of commands, and `main`, which serves as the main control loop for the program.

### ARGP Section
This section is what handles the command line argument parsing. I chose to use the `argp` library, a standard C library. In orger to do this, I included both `argp.h` and `argz.h`. Argp provides the parsing funtionallity needed for handling `-p`, but also allows me to help information for the program. For example, calling `./shell -?` or `./shell --help` will display information about the calling of the program. It also displays information about the author, and adds a `--version` flag. Some example output of `-?` would be
``` shell
woolery@iscore-dev:~/CLionProjects/308_proj1$ ./shell -?
Usage: shell [OPTION...] [Command]
Shell emulator written for CPR E 308

  -p, --prompt=PROPMPT       Prompt to display
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to woolery@iastate.edu.
woolery@iscore-dev:~/CLionProjects/308_proj1$
```

To handle the required `-p` flag, I defined an `argp_opts` struct, containing the the arguments the program accepts. I also defined the method `parse_opts()`, which takes the arguments passed to main and fills the `argp_opts` struct with the argument vaules.

### Parse Line Section
`parse_line(char str[])` is the function that does most of the logic for this program. Immediately upon entering the method, a copy of `str` is made. This is because the raw command from the command line will be needed if the command is not a builtin. `strtok()` is then called to get the first line of the command. This is passed through a if/else-if tree to determine if the command is a builtin. If the command is not a built in, it is executed using `execvp`. For commands which may return an error, such as `chdir` or `execvp`, `strerror(errno)` is used to get a more user-friendly error output. Throughout the program, output is intended to be similar to `bash`, to prevent undue confusion.

### Main Section
The main function is simply responsible for the initializing of basic variables, and running the application loop. The loop follows the flow of flush stdin --> print prompt (308sh or value of the `-p` flag) --> read line (blocking) --> parse_line() --> repeat. The main method is deliberately kept rather basic, in keeping with good programming practice.