#include<stdio.h>



typedef struct{
    unsigned int day;
    unsigned int month;
    int year;
}Date;


typedef struct{
    char lastname[50];
    char matrikelNr[20];

    Date start;
    Date end;
    Date bithday;
}Student;
