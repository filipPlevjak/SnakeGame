#include "struktury.h"
#include "hra.h"
#include "CMemLeak.h"


bool mozemVygenerovatJedlo(HRA* hra,BOD jedloAtr);

void nastavSmerHadika(DATA *paData, int paCisloKlienta, char paSmer){
    paData->hra->poleSmerov[paCisloKlienta] = paSmer;
}


bool suRovnakeSuradnice(BOD bod1, BOD bod2){

  return  (bod1.x == bod2.x) && (bod1.y == bod2.y);
}


bool naburalHadik(HRA *hra, BOD hlava){
   return (!mozemVygenerovatJedlo(hra, hlava));
}


bool mozemVygenerovatJedlo(HRA* hra,BOD jedloAtr){
  
    for(int i = 0; i < POLE_KLIENTOV; i++){
      
        if(suRovnakeSuradnice(jedloAtr, hra->hadici[i].hlava))  // ak suradnice jedla sa rovnajú súradnice hlavy znamená že nemôže tu vygenerovať jedlo
            return false;
        
        for(int j = 0; j < hra->hadici[i].velkostTela ; j++){ 
              if(suRovnakeSuradnice(jedloAtr, hra->hadici[i].teloPole[j]))  // ak suradnice jedla sa rovnajú súradniciam bodov tela  znamená že sa tu nemôže vygenerovať jedlo
                  return false;
        }  
    }
    return true; // žiadne súradnice sa nerovnajú, môže sa vygenerovať jedlo
}


void vygenerujJedlo(HRA* hra)
 {
    if(hra->vygenerovaneJedlo  == true)
        return;
    
    BOD jedloAtr;
    
    jedloAtr.x = rand()% MAX_HRACIA_PLOCHA;
    jedloAtr.y = rand()% MAX_HRACIA_PLOCHA;
    
    while(mozemVygenerovatJedlo(hra,jedloAtr) == false){ // pokiaľ nemôžem vygenerovať jedlo súradnice sa budú posúvať
        jedloAtr.x++;
        
            if(jedloAtr.x >= MAX_HRACIA_PLOCHA){
                jedloAtr.x = 0;  
                jedloAtr.y++;  
                
                if(jedloAtr.y >= MAX_HRACIA_PLOCHA)
                    jedloAtr.y =0;
            }    
    }

    hra->jedlo = jedloAtr;
    hra->vygenerovaneJedlo = true;
 }


void posuvHlavy(BOD* hlava, char smer){
  	switch(smer){
            
            case 'w':
             hlava->x--; // vyšší riadok     // hlava->y--;
             if(hlava->x < 0)
                 hlava->x = MAX_HRACIA_PLOCHA -1;
                break;
                
            case 'a':
               hlava->y--;// stlpec vľavo  // hlava->x--;
                 if(hlava->y < 0)
                 hlava->y = MAX_HRACIA_PLOCHA -1;
                break;
                
            case 's':
              hlava->x++; // nižší riadok  hlava->y++;
               if(hlava->x > MAX_HRACIA_PLOCHA -1)
                 hlava->x = 0;
                break;
                
            case 'd':
                hlava->y++; // stlpec vpravo hlava->x++;
                 if(hlava->y > MAX_HRACIA_PLOCHA -1)
                 hlava->y = 0;
                break;
                
            default:
                printf("Error klavesa\n");
                break;
	}
}


void ensureCapacity(HADIK *hadik){
    if(hadik->velkostPola == hadik->velkostTela){
        hadik->teloPole = realloc(hadik->teloPole, 2*hadik->velkostPola*sizeof(BOD));
        hadik->velkostPola*=2;
    }
}

void skontrolujSuradniceZradla(HRA *hra){
        for(int i = 0; i<POLE_KLIENTOV;i++){
            //printf("%d==%d\n", hra->jedlo.x, hra->hadici[i].hlava.x);//pre DEBUG
            //printf("%d==%d\n", hra->jedlo.y, hra->hadici[i].hlava.y);//pre DEBUG
            
            if(suRovnakeSuradnice(hra->hadici[i].hlava, hra->jedlo)){
                hra->hadici[i].velkostTela++;
                hra->vygenerovaneJedlo = false;
               // printf("velkost tela %d\n", hra->hadici[i].velkostTela);//pre DEBUG
                
                ensureCapacity(&(hra->hadici[i]));
            
                    for(int k = hra->hadici[i].velkostTela-1; k > 0; k--){
                        hra->hadici[i].teloPole[k] =  hra->hadici[i].teloPole[k-1]; 
                         
                    }
            
                hra->hadici[i].teloPole[0] =  hra->hadici[i].predchadzajuciBodNaKonciHadika;// hra->hadici[i].predchadzajuciBodNaKonciHadika;
              
            }  
        } 
}


void vyprazdniPole(HRA *hra){
      for(int i = 0; i<MAX_HRACIA_PLOCHA;i++){
            for(int j = 0; j<MAX_HRACIA_PLOCHA;j++){
                hra->pole[i][j] = '-';    
            }
      }
}


void vlozHadikovDoPola(HRA *hra)
{
    BOD specialnyBod = {-1,-1};
        hra->pole[hra->jedlo.x][hra->jedlo.y] = 'Z';
        for(int i = 0; i<POLE_KLIENTOV;i++){
//          if(suRovnakeSuradnice(hra->hadici[i].hlava,specialnyBod))//nevlozi daneho hada pretoze uz prehral
//              continue;
          
            for(int j = 0; j< hra->hadici[i].velkostTela ;j++){
                hra->pole[hra->hadici[i].teloPole[j].x][hra->hadici[i].teloPole[j].y] =1+ i + '0' ;
            }
             
            char pomocna;
            if((hra->poleSmerov[i]) == 'w' ||'a' ||'s' ||'d')  
                pomocna = toupper(hra->poleSmerov[i]);
            else
                pomocna = 'L'; // dead_Lock, do budúcna dorobit stlacenie ineho znaku na hlavičku hada L
            
            hra->pole[hra->hadici[i].hlava.x][hra->hadici[i].hlava.y] = pomocna;
         
        }
}


void posunHadika(HADIK *hadik, char smer){
    int i=0;
    hadik->predchadzajuciBodNaKonciHadika = hadik->teloPole[0]; //posledny stav 
    
        if(hadik->velkostTela == 0){
            hadik->predchadzajuciBodNaKonciHadika = hadik->hlava;
        }
    
        for(i = 1; i < hadik->velkostTela; i++){
            hadik->teloPole[i-1] = hadik->teloPole[i];
        }
    
        if(hadik->velkostTela > 0)
            hadik->teloPole[i-1] = hadik->hlava;
      
    if(hadik->velkostTela == 1){
        hadik->teloPole[0] = hadik->hlava;
    }
    
    posuvHlavy(&(hadik->hlava), smer);  
}


void posunHadikov(HRA *paHra){
    for(int i = 0; i<POLE_KLIENTOV;i++){
        //printf("posun hadikov smer:%c  hadik:%d.\n",paHra->poleSmerov[i], i);//pre DEBUGING
        posunHadika(&(paHra->hadici[i]),paHra->poleSmerov[i]);//poslem adresu daneho hadika z pola a dany smer hadika
    }
}
 

void posliPoleHry(const DATA* data){
   int pocet[2];
   pocet[0]= MAX_HRACIA_PLOCHA*MAX_HRACIA_PLOCHA + MAX_HRACIA_PLOCHA + 1;
   
   int k = 0;
   char *buffer  = malloc(pocet[0] * sizeof(char));
    
   pthread_mutex_lock(data->mutex);
        buffer[k++] = ((int)data->koniecHry) + '0';
   pthread_mutex_unlock(data->mutex);
   
        for (int m = 0; m< MAX_HRACIA_PLOCHA;m++){
            for (int n = 0; n< MAX_HRACIA_PLOCHA;n++){
                buffer[k] = data->hra->pole[m][n];
                k++; 
            }
            
            buffer[k] = '\n';   
            k++;
        }
       
    buffer[k-1] = '\0';
        
        for (int i = 0; i< POLE_KLIENTOV;i++){
            pocet[1] =  data->hra->hadici[i].velkostTela;
            write(data->hra->poleSocketov[i],pocet,sizeof(int) *2);  
            write(data->hra->poleSocketov[i],buffer,pocet[0] * sizeof(char));
            printf("\033c");//premaže terminal
            //printf("tu začal\n%s\ntu skončil\n", buffer);//kontolny vypis zaciatku a konca, pre Debug do budúcna
        }
    
   free(buffer);
   buffer = NULL;
}


void hra(DATA* data){
    
    while(!data->koniecHry){
        vyprazdniPole(data->hra);
        vygenerujJedlo(data->hra);
        vlozHadikovDoPola(data->hra);
        posliPoleHry(data);
        skontrolujSuradniceZradla(data->hra);
        posunHadikov(data->hra);
        
        usleep(1900000 -200000*data->hra->rychlostHry); 
        
    }        
}