///////////////////////////////////////////////////////////////////////
////////////////////////HRA SNAKE//////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////main server/////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#include "mainS.h"
#include "CMemLeak.h"


void prehraKlientov(DATA *data){
    
    for(int i = 0; i < POLE_KLIENTOV; i++){
        if(naburalHadik(data->hra,data->hra->hadici[i].hlava)){
            
            pthread_mutex_lock(data->mutex);
            data->poleKoncovHry[i]  = true;
            pthread_mutex_unlock(data->mutex);
            data->hra->hadici[i].hlava.x = -1;
            data->hra->hadici[i].hlava.y = -1;
        } 
    }
 
}


static struct termios oldt;
void restore_terminal_settings(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void disable_waiting_for_enter(void)
{
    struct termios newt;
    // Make terminal read 1 char at a time 
    tcgetattr(0, &oldt);  // Save terminal settings 
    newt = oldt;  // Init new settings 
    newt.c_lflag &= ~(ICANON | ECHO);  // Change settings 
    tcsetattr(0, TCSANOW, &newt);  // Apply settings 
    atexit(restore_terminal_settings); // Make sure settings will be restored when program ends  
}

void pripojKlientov(HRA* hra){
    
    struct sockaddr_in newAddr, serverAddr; 
    int sockFd,ret;
    
    printf("kontrolny vypis\n");
    socklen_t addr_size;

    sockFd = socket(AF_INET, SOCK_STREAM,0);
        if(sockFd <0){
            printf("[-] Error in connection");
            exit(1);
        }
    
    printf("[+] Server Socket is created");
  
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port = htons(SOCKET_PORT);
    serverAddr.sin_addr.s_addr=INADDR_ANY;
    
    ret = bind(sockFd, (struct sockaddr*)&serverAddr,sizeof(serverAddr));
        if(sockFd <0){
            printf("[-] Error in binding-\n");
            exit(1);
        }
    printf("[+] Bind to port %d\n", SOCKET_PORT);
     
     
    if(listen(sockFd,3) == 0){
        printf("Listening...\n");
    }
    else{
        printf("[-] Error in Listening. %d\n", SOCKET_PORT);
    }
     
    addr_size =sizeof(newAddr);

    int podmienka = 0;
    while(podmienka < 3){       
        hra->poleSocketov[podmienka] = accept(sockFd,(struct sockaddr*)&newAddr, &addr_size);
        printf("cislo new Socketu:%d\n", hra->poleSocketov[podmienka]);
              
            if(hra->poleSocketov[podmienka] == -1){
                printf("Error accept\n");
                exit(1);
            }
            printf("Connection accepted from: %s:\n",inet_ntoa(newAddr.sin_addr) );
            podmienka++;
      }
}

void inicializaciaHada(HADIK *hadik, BOD paBodHlava){
    hadik->hlava = paBodHlava;
    hadik->predchadzajuciBodNaKonciHadika = paBodHlava;
    hadik->teloPole = calloc(5, sizeof(BOD));
    hadik->velkostPola = 5;
    hadik->velkostTela = 0;
    hadik->znakHadika = 'X';
}

void freeAllocation(HADIK *hadik){
    
    free(hadik->teloPole);
    hadik->teloPole = NULL;
}

int main(int argc, char *argv[]){
    
    pthread_mutex_t mutex;
    
    HADIK ST_hadici[POLE_KLIENTOV];
   
        for (int i = 0;i<3;i++){
            BOD ST_bod = {i*3+3,i*3};
            inicializaciaHada(&(ST_hadici[i]), ST_bod);
        }
    
    BOD ST_bodJedlo = {5,5};
    HRA ST_hra = {1,false, ST_bodJedlo,{0}, {'d','w','a'}, ST_hadici,{0}    };
 
    pripojKlientov(&ST_hra);//nastavi to sockety v strukture ST_hra
    DATA ST_data = { {false},0,&ST_hra, &mutex};
 
    pthread_t thread_1, thread_2, thread_3;
    pthread_t skonciThread;
      
    pthread_create(&thread_1, NULL, vlakno1, &ST_data);
    pthread_create(&thread_2, NULL, vlakno2, &ST_data);
    pthread_create(&thread_3, NULL, vlakno3, &ST_data);
    pthread_create(&skonciThread, NULL, vlaknoUkonciAplikaciu, &ST_data);

    srand(time(NULL));
    hra(&ST_data);
     
    pthread_join(skonciThread,NULL);
    pthread_join(thread_1,NULL);
    pthread_join(thread_2,NULL);
    pthread_join(thread_3,NULL);
    
        for (int i = 0;i<3;i++){//dealokovanie dynamicky allokovaneho miesta
            close(ST_hra.poleSocketov[i]);
                freeAllocation(&(ST_hra.hadici[i]));   
        }
    
 
      
    system("pause"); 
    //ukoncit sokety
}