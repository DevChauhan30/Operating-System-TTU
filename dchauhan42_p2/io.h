//Author: Dev Chauhan
//Date: 10/20/2022
//CSC 4100 - Operating Systems
//Professor: Dr. Mike Rogers

#include <stdint.h>

void k_print(char *s, int len, int row, int col);
void k_printstr(char *s, int row, int col);
void go();
void dispatch_leave();
void dispatch();
void lidtr(uint32_t idtr);