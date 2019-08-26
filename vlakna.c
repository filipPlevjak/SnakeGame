#include "vlakna.h"

void* vlakno1(void* atr){
    DATA *data = (DATA*)atr;
    
    while(data->koniecHry != true){
        char value;
        
            if(data->koniecHry == true)
                return NULL;
        
        read( data->hra->poleSocketov[0], &value, sizeof(char)); //server cita znak smeru od prveho klienta 
  
        nastavSmerHadika(data, 0, value);
       
    }
    return NULL;
}

void* vlakno2(void* atr){
    DATA *data = (DATA*)atr;
    
        while(data->koniecHry != true){
            char value;; 
                if(data->koniecHry == true)
                    return NULL;
        
            read( data->hra->poleSocketov[1], &value, sizeof(char)); //server cita znak smeru od prveho klienta
                
            nastavSmerHadika(data, 1, value);
        }    
    return NULL;
}

void* vlakno3(void* atr){
   DATA *data = (DATA*)atr;
    
    while(data->koniecHry != true){
        char value;
            if(data->koniecHry == true)
                return NULL;
        
        read(data->hra->poleSocketov[2], &value, sizeof(char));
        nastavSmerHadika(data, 2, value);    
    }    
   return NULL;
}


void* vlaknoUkonciAplikaciu(void* atr){
      DATA *data = (DATA*)atr;
      disable_waiting_for_enter();
      
      char vypni;
      vypni = getchar();
           while(vypni != 'k'){
               if(vypni >= '0' && vypni <= '9'){
              
                    data->hra->rychlostHry = vypni - '0';
              
                }
                vypni = getchar();
            }
      pthread_mutex_lock(data->mutex);
      data->koniecHry = true;
      pthread_mutex_unlock(data->mutex); 
      posliPoleHry(data);
      
      return NULL;
}
 