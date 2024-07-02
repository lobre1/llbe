#ifndef COMMANDS_H

extern int curLine;

int cmd_exec(char cmd_name[], char *args[]);
int start_edit();
int quit();

#endif
