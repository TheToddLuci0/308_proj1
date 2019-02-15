#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

#include <argp.h>
#include <argz.h>


/************************************/
/*        ARGP SECTION              */
/************************************/
const char *argp_program_version = "version 0.0.1_prerelease";
const char *argp_program_bug_address = "woolery@iastate.edu";


struct arguments {
    char *prompt;
    char *args[1];
};

static struct argp_option options[] = {
        {"prompt", 'p', "PROPMPT", 0, "Prompt to display"},
        {0}
};

static error_t
parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 'p':
            arguments->prompt = arg;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1) {
                argp_usage(state);
            }
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            if (state->arg_num > 1) {
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static char args_doc[] = "[Command]";
static char doc[] = "Shell emulator written for CPR E 308";

static struct argp argp = {options, parse_opt, args_doc, doc};




/************************************/
/*        LOGIC SECTION             */
/************************************/

void parse_line(char *str) {
    char *raw;
    strcpy(raw, str);
    char *command = strtok(str, " ");
    if (!strcmp(command, "exit")) {
        // Terminate and accept no further input from the user
        exit(0);
    } else if (!strcmp(command, "ppid")) {
        // Print parent process ID
        printf("%d\n", getppid());
        return;
    } else if (!strcmp(command, "pid")) {
        // Print the PID
        printf("%d\n", getpid());
        return;
    } else if (!strcmp(command, "cd")) {
        // Change the working directory, $HOME if no arg given
        char *copy;
        strcpy(copy, str);
        char *dir = strtok(NULL, " ");
        if (dir == NULL) {
            dir = getenv("HOME");
        } else {
            dir = copy;
        }
        chdir(dir);
        return;
    } else if (!strcmp(command, "pwd")) {
        // Print the current directory.
        char buf[1000];
        printf("%s\n", getcwd(buf, 1000));
        return;
    } else if (!strcmp(command, "set")) {
        printf("Test successful");
        return;
    } else if (!strcmp(command, "get")) {
        char *var = getenv(strtok(NULL, " "));
        if (var == NULL) {
            printf("Variable not set\n");
        } else {
            printf("%s\n", var);
        }
    } else {
        printf("Test failed, command was '%s'\n", command);
        printf("Raw input was %s\n", raw);
        int ar, i = 0;
//        for (i = 0; i < strlen(str); i++) {
//            if (str[i] == ' ') {
//                ar++;
//            }
//        }
//        char strs[ar][500+1];
        i=0;
//        while (str != NULL) {
//            arr[i] = strtok(NULL, " ");
//        }
//        int p = fork();
//        if(!p) {
//            execvp(command, arr);
//        } else{
//            waitpid(p, NULL, 0);
//        }
    }
}


int main(int argc, char **argv) {
    struct arguments arguments;
    int arg_count = 1;
    arguments.prompt = "";

    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    char *prompt;
    if (arguments.prompt == "") {
        prompt = "308sh>";
    } else {
        prompt = arguments.prompt;
    }
    char* str;
    while (1) {
        printf("%s ", prompt);
        fgets(str, 100, stdin);
        parse_line(str);
    }
}