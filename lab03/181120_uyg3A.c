#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h> 			//for usleep(); if you need to sleep shorter than 1 sec. //usleep((int)(100000)); for 0.1 sec.s
#include <sys/stat.h>		//for the "last modified time" stat
#include <pthread.h>
char *pathRep;

//paylaşılan değişken
struct stat attr;

//değiştirilme tarihinde bir farklılık var mı diye kontrol edip, varsa işlemleri yapan fonksiyon threadi.
void *funcprintTimeAndChanges(void *pathSub) {

	char pathLogFile[250];
    //log dosyasının dizinini belirledik.
	sprintf(pathLogFile, "%s/181120_uyg3A_repHist.log", pathRep);
    char *_pathSub = pathSub;

    //bu chkdattr struct'ı son güncellemeyle aynı mı değil mi diye kontrol etmemizi sağlıyor. İlk başta güncel versiyon statını tutuyor.
	struct stat chkdattr = attr;

	while(1) {

        //farklılık oldugu zaman, yani ikisinin modified time'ı farklı gözükürse buraya girecek.
		if(attr.st_mtime != chkdattr.st_mtime) {

            //lab'ta yapılması istenenler bu kısımda yapılıyor.
			printf("Last modified time: %s", ctime(&attr.st_mtime)); //If you need some ctime comparison, use (a.st_mtime == b.st_mtime)
            char myChar[500];
            
            sprintf(myChar, "ls -R1rs; ls -R1rs >> %s", pathLogFile);
            system(myChar);

            sprintf(myChar, "cp -a pathSubmission/. pathRepository/.");
            system(myChar);

            //işimiz bittikten sonra kontrol etmemizi sağlayan değişken tekrardan güncel attr'nin değerini alıyor. Bu sayede her değişiklik oldugunda algılayabiliyor.
            chkdattr = attr;
		}

		printf("\r\033[1A%c",  system("date")); // \r: set cursor to the first char of the line; \033[1A: move cursor up 1 line;
		fflush(stdout);	//clear output buffer
		usleep((int)(1000000));
	}

	printf("This line should never be executed. Server program must always be running!\n");
}

//stat'ı güncelleyen fonksiyon
void *funcChecker(void *pathSub) {
    while(1) { 
        //sürekli olarak global'de tutulan attr kısmı 
        stat(pathSub, &attr);
        usleep(1000000);
    }
	// printf("This line should never be executed. Server program must always be running!\n");
}

int main(int argc,char *argv[]) {
    
    
    if(argc != 3){
        printf("You have to give ONLY the parameters pathRepository and pathSubmission! Exiting!\n");
        exit(0);
    }
    else {
        pathRep = argv[1];
        char *pathSub = argv[2];
        char myChar[100];

        //dosya pathlerini aldıktan sonra folderları açtık.
        sprintf(myChar,"mkdir -p %s; mkdir -p %s", pathRep, pathSub);
        system(myChar);

        //thread olusturma
        pthread_t threadChecker;
        pthread_t threadWorker_TimeAndChanges;

        //thredleri yaratma
        pthread_create(&threadChecker, NULL, funcChecker, argv[2]);
        pthread_create(&threadWorker_TimeAndChanges, NULL, funcprintTimeAndChanges, argv[2]);

        //threadleri fonksiyonları parametre verip joinledik.
        pthread_join(threadChecker, NULL);
        pthread_join(threadWorker_TimeAndChanges, NULL);
    }

	return 0;
}