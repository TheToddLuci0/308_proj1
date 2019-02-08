#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <argz.h>
#include <string.h>
#include "commad_parser.h"


const char *argp_program_version = "version 0.0.1_prerelease";
const char *argp_program_bug_address = "woolery@iastate.edu";


struct arguments{
	char *prompt;
	char *args[1];
};

static struct argp_option options[] = {
	{"prompt", 'p', "PROPMPT", 0, "Prompt to display"},
	{0}
};

static error_t
parse_opt(int key, char *arg, struct argp_state *state){
	struct arguments *arguments = state->input;
	switch(key){
		case 'p':
			arguments->prompt = arg;
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num >= 1){
	  			argp_usage(state);
			}
      		arguments->args[state->arg_num] = arg;
      		break;
    	case ARGP_KEY_END:
      		if (state->arg_num > 1){
	  			argp_usage (state);
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


int main(int argc, char **argv){
	struct arguments arguments;
	int arg_count = 1;
	arguments.prompt = "";
	
	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	printf("PROMPT: %s\n", arguments.prompt);
	char *prompt;
	if(arguments.prompt == ""){
		prompt = "308sh>";
	}else{
		prompt = arguments.prompt;
	}
	char str[300];
	while(1){
		printf("%s ", prompt);
		gets(str);
		parse_line(str);		
	}
}


