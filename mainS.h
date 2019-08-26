#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "struktury.h"




void* vlakno1(void* atr);
void* vlakno2(void* atr);
void* vlakno3(void* atr);    
void* vlaknoUkonciAplikaciu(void* atr);
bool naburalHadik(HRA *hra, BOD hlava);
void hra(DATA* data);