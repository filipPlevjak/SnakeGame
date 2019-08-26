#ifndef VLAKNA_H
#define VLAKNA_H

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* VLAKNA_H */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "struktury.h"


void nastavSmerHadika(DATA *paData, int paCisloKlienta, char paSmer);
void disable_waiting_for_enter(void);
void posliPoleHry(const DATA* data);
