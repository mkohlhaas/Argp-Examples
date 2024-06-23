#include <argp.h>
#include <error.h>
#include <stdlib.h>

const char *argp_program_version     = "argp-ex4 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@prep.ai.mit.edu>";

static char doc[] = "Argp example #4 -- a program with somewhat more complicated \
options\
\vThis part of the documentation comes *after* the options;\
 note that the text is automatically filled, but it's possible\
 to force a line-break, e.g.\n<- here.";

/* A description of the arguments we accept. */
static char args_doc[] = "ARG1 [STRING...]";

/* Keys for options without short-options. */
#define OPT_ABORT 1 /* –abort */

/* The options we understand. */
static struct argp_option options[] = { {
                                            .name = "verbose",
                                            .key  = 'v',
                                            .doc  = "Produce verbose output",
                                        },
                                        {
                                            .name = "quiet",
                                            .key  = 'q',
                                            .doc  = "Don't produce any output",
                                        },
                                        {
                                            .name  = "silent",
                                            .key   = 's',
                                            .flags = OPTION_ALIAS,
                                        },
                                        {
                                            .name = "output",
                                            .key  = 'o',
                                            .arg  = "FILE",
                                            .doc  = "Output to FILE instead of standard output",
                                        },

                                        {
                                            .doc = "The following options should be grouped together:",
                                        },
                                        {
                                            .name  = "repeat",
                                            .key   = 'r',
                                            .arg   = "COUNT",
                                            .flags = OPTION_ARG_OPTIONAL,
                                            .doc   = "Repeat the output COUNT (default 10) times",
                                        },
                                        {
                                            .name = "abort",
                                            .key  = OPT_ABORT,
                                            .doc  = "Abort before showing any output",
                                        },
                                        { 0 } };

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char  *arg1;         /* arg1 */
  char **strings;      /* [string…] */
  int    silent;       /* ‘-s’ */
  int    verbose;      /* ‘-v’ */
  int    abort;        /* ‘--abort’ */
  char  *output_file;  /* file arg to ‘--output’ */
  int    repeat_count; /* count arg to ‘--repeat’ */
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'q':
    case 's':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'o':
      arguments->output_file = arg;
      break;
    case 'r':
      arguments->repeat_count = arg ? atoi (arg) : 10;
      break;
    case OPT_ABORT:
      arguments->abort = 1;
      break;
    case ARGP_KEY_NO_ARGS:
      argp_usage (state);
      // fall-through
    case ARGP_KEY_ARG:
      /* Here we know that state->arg_num == 0, since we
         force argument parsing to end before any more arguments can
         get here. */
      arguments->arg1 = arg;

      /* Now we consume all the rest of the arguments.
         state->next is the index in state->argv of the
         next argument to be parsed, which is the first string
         we’re interested in, so we can just use
         &state->argv[state->next] as the value for
         arguments->strings.

         In addition, by setting state->next to the end
         of the arguments, we can force argp to stop parsing here and
         return. */
      arguments->strings = &state->argv[state->next];
      state->next        = state->argc;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = {
  .options  = options,
  .parser   = parse_opt,
  .args_doc = args_doc,
  .doc      = doc,
};

int
main (int argc, char **argv)
{
  /* Default values. */
  struct arguments arguments = {
    .output_file  = "-",
    .repeat_count = 1,
  };

  /* Parse our arguments; every option seen by parse_opt will be
     reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  if (arguments.abort)
    {
      error (10, 0, "ABORTED");
    }

  for (int i = 0; i < arguments.repeat_count; i++)
    {
      printf ("ARG1 = %s\n", arguments.arg1);
      printf ("STRINGS = ");
      for (int j = 0; arguments.strings[j]; j++)
        {
          printf (j == 0 ? "%s" : ", %s", arguments.strings[j]);
        }
      printf ("\n");
      printf ("OUTPUT_FILE = %s\nVERBOSE = %s\nSILENT = %s\n", arguments.output_file, arguments.verbose ? "yes" : "no",
              arguments.silent ? "yes" : "no");
    }
}
