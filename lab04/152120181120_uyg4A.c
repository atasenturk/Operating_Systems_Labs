#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>

#define FACENG 15
#define FACOTH 30
#define DEPTCE 21
#define DEPTEE 12

#define READ 0
#define WRITE 1

struct stat attr;

bool alarmFlag = false;
void alarmHandler() {

	printf("Alarm Handler -> Alarm clock signal has been received.\n");
	// Fill
	//alarm handler ne kadar beklenmesi gerekirse bekledikten sonra alarmFlag'ı true yapar ve bu şekilde o kadar saniye beklenmiş olur.
	alarmFlag = true;
}

void multipleSignalHandler() {

	printf(" Sig. Handler -> I'll always get suffering (Protection from Ctrl+C | Ctrl+Z)!!!\n");
}

void *funcChecker(void *pathChk) {

	while (1) {
		stat(pathChk, &attr);
		usleep((int)(100000));
	}
	printf("This line should never be executed. Server program must always be running!\n");
	// Fill
}

void printDetailsOfStudentID(int faculty, int dept, int entYear) {

	printf("\t\tFaculty:\t");
	FACENG == faculty ?	printf("Engineering Faculty\n") : (FACOTH == faculty ?	printf("Other University\n") : printf("Other\n"));

	printf("\t\tDepartment:\t");
	DEPTCE == dept ? printf("Computer Engineering\n") : (DEPTEE == dept ? printf("Electrical Engineering\n") : printf("Other\n"));

	printf("\t\tEntrance Year: \t%d\n", entYear);
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("%s must be executed with one additional argument (pathInputFile)!\n", argv[0]);
		printf("Aborted automatically...\n");	
		return -1;
	}
    char *pathInp = argv[1];
    //  = atol(pathInp);

	/*Which data type is suitable for 12 digit number?*/ 
	//12 haneli bir sayıyı numerik olarak tutabilmek için long int değişkenini kullanabiliriz.
    long int studentNumber;

	int pfd[2], child_status;
	int nread;

	//piperesult'a pipe(pfd)değeri döndürülür. Başarılı ise 1, değil ise -1 döndürür.
	int piperesult = pipe(pfd);
	if (piperesult == 1) {
		printf("Pipe was created successfully...\n");
	}
	else if (piperesult == -1) {
		perror("pipe failed");
		exit(1);
	}

	int pid = fork();
	if (pid < 0) {
		perror("fork failed");
		exit(2);
	}
	else if (pid == 0) {	//child process

		printf("Child         -> My PID is %d, PPID is %d\n", getpid(), getppid());		
		//threadChecker oluşturuldu ve pthread_create ile bu thread verildi. Ayrıca threadin işletecği fonksiyon funcChecker olarak belirlendi. Ortak üzerinde çalışılacak dosya ise pathInp olacak.
		pthread_t threadChecker;
		pthread_create(&threadChecker, NULL, funcChecker, argv[1]);
		struct stat chkdattr;
		stat(argv[1], &chkdattr);
		printf("              -> Last modified time: %s", ctime(&chkdattr.st_mtime));
		
		while (1) {
			if(attr.st_mtime != chkdattr.st_mtime) {
				printf("              --> Last  updated time: %s", ctime(&attr.st_mtime));
				//dosyadan okuma islemi. Bu okunanı studentNumber'a eşitler.
				FILE *fp;
				fp = fopen(pathInp, "r");
				fscanf(fp, "%li", &studentNumber);
				fclose(fp);

				printf("                  %s: %li\n", argv[1], studentNumber);
				if(studentNumber/100000000000<0.1)
					printf("                  -- wrong input! few digits...\n");
				else if(studentNumber/100000000000>10)
					printf("                  -- wrong input! number of digits overflowed...\n");
				else
					break;
				chkdattr = attr;
			}
			usleep((int)(100000));
		}
		//pthread_join(threadChecker, NULL);	//infinite loop, never joins
	}
	else {	//parent process

		printf("Parent        -> My PID is %d, PPID is %d\n", getpid(), getppid());
		close(pfd[1]);
		while (0 != (nread = read(pfd[0], &studentNumber, sizeof(studentNumber)))) {

			printf("Parent        -> Incoming message from child is: %li\n", studentNumber);
			printf("            ---> Hint: Incoming message size is fixed %d bytes for this application. Not 12 (digit count) or something other! \n", (int)sizeof(studentNumber));
		}
		close(pfd[0]);
	}

	int faculty = studentNumber / 10000000000;
	int temp = studentNumber % 10000000000;
	int dept = temp / 100000000;
	temp = temp % 100000000;
	int entYear = temp / 10000;

	if (pid == 0) {	//child process
		printf("Child         -> Parsing and printing Student ID (Hardcodded, just for info)\n");
		printDetailsOfStudentID(faculty, dept, entYear);
		
		printf("Child         -> The message is being send. Warning: Type Constraint!\n");
		close(pfd[0]);
		write(pfd[1], &studentNumber, sizeof(studentNumber));
		close(pfd[1]);
	
		printf("Child         -> Sleeping 4 sec.s\n");
		sleep(4);
		printf("Child         -> Slept 4 sec.s\n");
	}
	else {	//parent process
		//Eger fakulte mühendislik mimarlık fakültesi ve departman bilgisayar mühendisliği ise girer.
		if (FACENG == faculty && DEPTCE == dept) {
			printf("Parent        -> Child process is being waited.\n");
			// Fill
			//Cocuk processin işini bitirmesini bekler.
            wait(&child_status);
			printf("Parent        -> Waiting is done.\n");
		}
		else if (2017 <= entYear) {
			printf("Parent        -> I'll kill you when your normal time is up.\n");
			// Fill
			//alarmHandler için 4 saniyelik bir alarm kurulur ve 4 saniye sonucunda alarmFlag true olacağı için 4 saniye sonunda program sonlanır.
			signal(SIGALRM, alarmHandler);
			alarm(4);
			printf("Parent        -> Alarm clock signal was scheduled in %d seconds...\n", 4);
			while (!alarmFlag) {
                pause();
            }
			
			kill(pid, SIGQUIT);
			printf("Parent        -> Child is killed. Or not?\n");
		}
		else if (FACENG != faculty) {
			/*
			First signal
			Second signal
			Third signal
			*/
			//alarmHandler için 6 saniyelik bir alarm kurulur ve 4 saniye sonucunda alarmFlag true olacağı için 4 saniye sonunda program sonlanır.
			//ayrıca ctrl-c ve ctrl-z ignore etmek icin multipleSignalHandler fonksiyonu kullanılır.
		    signal(SIGINT, multipleSignalHandler); //CTRL-C ignore et
            signal(SIGTSTP, multipleSignalHandler); //CTRL-Z ignore et
            signal(SIGALRM, alarmHandler);
			alarm(6);
			printf("Parent        -> Alarm clock signal was scheduled in %d seconds...\n", 10);
			printf("Parent        -> Freedom for 10 seconds.\n");
            while (!alarmFlag) {
                pause();
            }
		}
		//diger durumlar icin gecerli bir sayi olmayacagi icin program direkt sonlanır.
		else {
			return 0;
		}
	}
	printf("Terminating PID: %d, PPID: %d\n", getpid(), getppid());
	
	return 0;
}