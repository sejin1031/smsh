#define LINE_LEN	80	// 각종상수값정의
#define MAX_ARGS	64
#define MAX_ARGS_LEN	16
#define MAX_PATHS	64
#define MAX_PATH_LEN	96

#define TRUE -1
#define FALSE 0

#ifndef NULL
#define NULL 0
#endif

struct command_t {	// 명령이저장되는구조체
	char *name;
	int argc;
	char *argv[MAX_ARGS];
};