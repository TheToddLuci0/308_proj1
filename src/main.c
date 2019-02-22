#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

#include <argp.h>

/** The longest allowable line */
#define MAX_LINE 1000


/************************************/
/*        ARGP SECTION              */
/************************************/
const char *argp_program_version = "version 0.0.1_prerelease";
const char *argp_program_bug_address = "woolery@iastate.edu";

/** This struct holds the arguments list */
struct arguments {
    char *prompt;
    char *args[1];
};

/** The list of options argp uses for mapping, terminated with a null object */
static struct argp_option options[] = {
        {"prompt", 'p', "PROPMPT", 0, "Prompt to display"},
        {0}
};

/**
 * The function responsible for the assigning of the command line options to the arguments struct
 */
static error_t
parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 'p':
            /** This means that `-p` was passed. */
            arguments->prompt = arg;
            break;
        case ARGP_KEY_ARG:
            /** Handle argp built ins (--help, -?, --version, ect). None of the arguments handled here affect the
             * program internally */
            if (state->arg_num >= 1) {
                argp_usage(state);
            }
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            /** Signal for no more args to parse */
            if (state->arg_num > 1) {
                argp_usage(state);
            }
            break;
        default:
            /** Handle arguments that are not supported */
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/** The documentation of mandatory arguments, In this case, none */
static char args_doc[] = "";
/** The summary of the program */
static char doc[] = "Shell emulator written for CPR E 308";

static struct argp argp = {options, parse_opt, args_doc, doc};




/************************************/
/*        LOGIC SECTION             */
/************************************/
/** This is the function that handles the parsing of commands */
void parse_line(char str[]) {
    int REAL_FORK = 0;
    char raw[MAX_LINE];
    strcpy(raw, str);
    /** Determine if the command is meant to be daemonized. If so, set REAL_FORK and remove the '&' */
    if (raw[strlen(raw) - 1] == '&') {
        raw[strlen(raw) - 1] = '\0';
        REAL_FORK = 1;
    }
    /** Make sure there is actually something to do here */
    if (!strlen(raw)) return;
    char *command = strtok(str, " ");

    /** This tree checks to see if the command is a builtin */
    if (!strcmp(command, "exit")) {
        // Terminate and accept no further input from the user
        exit(0);
    } else if (!strcmp(command, "ppid")) {
        // Print parent process ID
        printf("%d\n", getppid());
    } else if (!strcmp(command, "pid")) {
        // Print the PID
        printf("%d\n", getpid());
    } else if (!strcmp(command, "cd")) {
        // Change the working directory, $HOME if no arg given
        char *dir[MAX_LINE];
        *dir = strtok(NULL, " ");
        if (*dir == NULL || strcmp(*dir, "") == 0) {
            *dir = getenv("HOME");
        }
        int stat = chdir(*dir);
        printf("%s\n", *dir);
        if (stat) {
            printf("Could not change directory. %d\n", stat);
            printf("%s\n", strerror(errno));
        }
    } else if (!strcmp(command, "pwd")) {
        // Print the current directory.
        char buf[1000];
        printf("%s\n", getcwd(buf, 1000));
    } else if (!strcmp(command, "set")) {
        // Set an environmental variable
        int err = setenv(strtok(NULL, " "), strtok(NULL, " "), 1);
        if (err) {
            printf("%s\n", strerror(errno));
        }

    } else if (!strcmp(command, "get")) {
        // Get an environmental variable
        char *var = getenv(strtok(NULL, " "));
        if (var == NULL) {
            printf("Variable not set\n");
        } else {
            printf("%s\n", var);
        }
    } else {
        // The command was not a builtin, so run it
        int ar = 0, i = 0;
        // count the number of arguments
        for (i = 0; i < strlen(raw); i++) {
            if (raw[i] == ' ') {
                ar++;
            }
        }
        ar++;
        char *arr[ar + 2];
        i = 1;
        arr[0] = strtok(raw, " ");
        while (i < ar) {
            arr[i] = strtok(NULL, " ");
            i++;
        }
        int status;
        int p = fork();
        if (!p) {
            // This is the child, do the work
            printf("Am a kid, number %d\n", getpid());
            int stat = execvp(command, arr);
            if (stat == -1) {
                printf("%s: %s\n", command, strerror(errno));
            } else {
                printf("Status: %d\n", stat);
            }
            exit(stat);
        } else if (!REAL_FORK) {
            // This is the parent, and we are waiting for the child
            waitpid(p, &status, 0);
            const int es = WEXITSTATUS(status);
            printf("exit status was %d\n", es);
        }
    }
    // Flush the buffers, in case we have any prints waiting to print
    fflush(stdout);
    fflush(stdin);
}

// Suppress warning about the infinite loop in some compilers
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main(int argc, char **argv) {
    // Set up for argp
    struct arguments arguments;
    int arg_count = 1;
    arguments.prompt = "";

    // Do the parsing
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    char *prompt;
    // Decide what the prompt should be
    if (arguments.prompt == "") {
        prompt = "308sh>";
    } else {
        prompt = arguments.prompt;
    }
    while (1) {
        int status;
        // See if there were any recently exited children
        if (waitpid(-1, &status, WNOHANG) > 0) {
            const int es = WEXITSTATUS(status);
            printf("exit status was %d\n", es);
        }
        char str[MAX_LINE];
        // Print the prompt
        printf("%s ", prompt);
        fflush(stdin);
        // Read the line
        fgets(str, 1000, stdin);
        if (str[strlen(str) - 1] == '\n')
            str[strlen(str) - 1] = '\0';
        // Hand the line to the parser
        parse_line(str);
    }
}
// Stop suppressing the warnings about infinite loops, we're past it.
#pragma clang diagnostic pop