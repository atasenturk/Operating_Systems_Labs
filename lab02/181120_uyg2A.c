#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
int loopVal;
int mainParent;
void create_tree(int depth){


    if (depth == 0) return;
    if(loopVal==depth) {
        

        int pid=fork();
        if(pid>0){
            //parent
            printf("will continue in 2 sec.\n");
            sleep(2);
            int pid2=fork();
            if(pid2>0){
                //parent
                char myChar[300];
                sprintf(myChar, "mkdir -p logs; cd logs; touch logAll.log; date >> logAll.log; echo 'PID: %d, Depth : %d' >> logAll.log; wait %d",getpid(),loopVal, getpid());
                system(myChar);
                sleep(2);
                mainParent=getpid();

            }
            else if(pid2==0){
                
                printf("PID: %d,PPID: %d\n",getpid(),getppid());
                printf("printing log file content\n");
                system("echo ------------logAll.log-------------; cat logs/logAll.log; echo -------------------------");
                //ilk cocuk (duruma göre sol veya sag diyebiliriz. Ben burayı sağ olarak belirledim.)
                create_tree(--depth);
            }
        }
        else if(pid==0){
            printf("PID: %d,PPID: %d\n",getpid(),getppid());
        }
            
    } 


    // İlk giriş değil ise sağ çocuk sadece bir tane fork işlemi yapması gerekiyor
    // Yani sağ çocuğun sadece bir çocuğu olması gerekiyor. 2 child'a parent'lık yapmaması gerektiği için ilk döngü değilse tek fork işlemi yaptık.
    else {
        int pid=fork();
        if(pid>0) {
            sleep(2);
        }
        if(pid==0){
            //Sag cocuk
            printf("PID: %d,PPID: %d\n",getpid(),getppid());
            create_tree(--depth);
        }

    }


    if(mainParent==getpid()){
        char myChar[300];
        sprintf(myChar, "pstree -p %d >> logs/logAll.log; echo -e \n>> logs/logAll.log; echo '\nthe new tree that will be recorded to log file'; pstree -p %d", mainParent, mainParent);
        system(myChar);
    }
}


int main(int argc,char **argv){
    int option;
    int depth;
    mainParent=getpid();
    depth = atoi(argv[argc-1]);
    if((option = getopt(argc,argv, "i:")) != -1){
        if(option == 'i'){
            printf("atasenturk wrote this code\n");
        }
        depth=atoi(optarg);
    }
    loopVal=depth;
    if(depth<2){
        printf("You have to enter depth minimum 2\nProgram is closing\n");
        exit(1);
    }
    printf("PID: %d,PPID: %d: parent...\nwill create logs/ folder 1 sec. later\n\n",getpid(),getppid());
    sleep(1);
    create_tree(depth);
    sleep(2);
}

