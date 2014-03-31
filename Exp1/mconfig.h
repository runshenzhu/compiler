#ifndef __MCONFIG_H
#define __MCONFIG_H

#define bool int
#define true 1
#define false 0
#define NO_SIMI 1000
#define SIMI 1001

bool look_fwd(int flag);
bool in_follow_set(const char c);
void err_handle();
char* up_char(char *buf);

#endif