typedef struct {
	char *cmdstr;
	void (*func)(const Arg *);
	const Arg arg;
} Command;

static void set_default_namespace_names();

static Command commands[] = {
	/* command              function                     argument */
	{  "default_namespace", set_default_namespace_names, {0} }
};

const char* const DEFAULT_NAMESPACE_NAME_FORMAT[] = {
	"%d: ", // 1
	"%d:", // 2
	"%d: ", // 3
	"%d: ", // 4
	"%d", // 5
	"%d", // 6
	"%d", // 7
	"%d", // 8
	"%d:", // 9
	"%d:" // 10
};

void
set_default_namespace_names(const Arg *arg) {
	char tagnamef[64];

	for (int i = 0; i < LENGTH(tags) && i < LENGTH(DEFAULT_NAMESPACE_NAME_FORMAT); ++i) {
		snprintf(tagnamef, sizeof(tagnamef), DEFAULT_NAMESPACE_NAME_FORMAT[i], i + 1);
		strcpy(tags[i], tagnamef);
	}
}
