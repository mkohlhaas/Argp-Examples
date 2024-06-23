#include <argp.h>

const char *argp_program_version     = "argp-ex3 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

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
                                        { 0 } };

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[2]; /* arg1 & arg2 */
  int   silent;
  int   verbose;
  char *output_file;
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
    case ARGP_KEY_ARG:
      if (state->arg_num >= 2)
        {
          /* Too many arguments. */
          argp_usage (state);
        }
      arguments->args[state->arg_num] = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 2)
        {
          /* Not enough arguments. */
          argp_usage (state);
        }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Program documentation. */
static char doc[] = "Argp example #3 -- a program with options and arguments using argp";

/* A description of the arguments we accept. */
static char args_doc[] = "ARG1 ARG2";

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
    .output_file = "-",
  };

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  printf ("ARG1 = %s\n"
          "ARG2 = %s\n"
          "OUTPUT_FILE = %s\n"
          "VERBOSE = %s\n"
          "SILENT = %s\n",
          arguments.args[0], arguments.args[1], arguments.output_file, arguments.verbose ? "yes" : "no",
          arguments.silent ? "yes" : "no");
}
