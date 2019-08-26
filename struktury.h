#ifndef STRUKTURY_H
#define STRUKTURY_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* STRUKTURY_H */

#include <stdbool.h>
#include <pthread.h>

#define MAX_HRACIA_PLOCHA 30
#define POLE_KLIENTOV 3//pocet pripojenych klientov
#define SOCKET_PORT 10000
 

typedef struct Bod{
    int x;
    int y;
}BOD;

typedef struct Hadik{
    BOD hlava;
    int velkostTela;
    int velkostPola;//je kolko ma had naalokovanych bodov tela
    BOD *teloPole;//nulty prvok na konci hadika a posledny je na zaciatku pri hlave
    char znakHadika;//na buduce zdokonalenie, ak by si klienti chceli navolit vlastny znak hadika
    BOD predchadzajuciBodNaKonciHadika;//posledny bod tela hadika ped tym ako sa pohnul[ked sa nazere na  tieto suradnice sa prida bod , telo sa rozsiri]
    
}HADIK;

typedef struct Hra{
    int rychlostHry;
    bool vygenerovaneJedlo;
    BOD jedlo;
    char pole[MAX_HRACIA_PLOCHA][MAX_HRACIA_PLOCHA];
    char poleSmerov[POLE_KLIENTOV];
    //HADIK hadici[POLE_KLIENTOV];
    HADIK *hadici;
    int poleSocketov[POLE_KLIENTOV];
}HRA;

typedef struct Data{
    bool poleKoncovHry[POLE_KLIENTOV]; // pole prehier
    bool koniecHry;
    HRA *hra;
    pthread_mutex_t *mutex;
}DATA;