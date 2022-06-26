#include <stdio.h>

extern etext;
extern end;
extern int edata;

main(){
    char c, *s;

    printf("&etext  =  0x%lx\n", &etext);
    printf("&end    =  0x%lx\n", &end);;
    printf("&edata  =  0x%lx\n", &edata);
    printf("sbrk(0) =  0x%lx\n\n", sbrk(0));


    printf("Enter memory location in hex (start with 0x): ");

    fflush(stdout);
    scanf("0x%lx", &s);
    long unsigned int address = s;
    long unsigned int address_etext = &etext;
    long unsigned int address_edata = &edata;

    printf("\n\n");
    if (address < address_etext) {
        printf("------Read-only segment!!!------\n\n");
        printf("Reading 0x%lx: ", s);
        fflush(stdout);
        c = *s;
        printf("Reading is OK\n");
    }

    else if (address < edata) {
        printf("------Read-write segment!!!------\n\n");
        printf("Reading 0x%lx: ", s);
        fflush(stdout);
        c = *s;
        printf("Reading is OK\n");

        printf("%d\n", c);
        printf("Writing %d back to 0x%lx: ", c, s);
        fflush(stdout);
        *s = c;
        printf("Writing is OK\n");
    }
    
}