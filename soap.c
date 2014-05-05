/* See LICENSE file for copyright and license details. */
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

typedef struct {
	const char *regex;
	const char *action;
} Pair;

#include "config.h"

int
main(int argc, char *argv[]){
	int i;
	char cmd[BUFSIZ];
	regex_t regex;

	/* we only take one argument */
	if (argc != 2)
		return EXIT_FAILURE;

	/* pass the argument through an environment variable */
	setenv(varname, argv[1]);

	/* check regex and launch action if it matches argv[1] */
	for (i=0; i < sizeof(pairs)/sizeof(*pairs); ++i) {
		if (regcomp(&regex, pairs[i].regex, REG_EXTENDED))
			fprintf(stderr, "invalid regex: %s\n", pairs[i].regex);
		if (!regexec(&regex, argv[1], 0, NULL, 0)) {
			snprintf(cmd, sizeof cmd, pairs[i].action, "\"$" varname "\"");
			system(cmd);
			return EXIT_SUCCESS;
		}
		regfree(&regex);
	}

	/* alternatively, fall back to xdg-open_ */
	snprintf(cmd, sizeof cmd, "xdg-open_ %s", "\"$" varname "\"");
	system(cmd);
	return EXIT_SUCCESS;
}
