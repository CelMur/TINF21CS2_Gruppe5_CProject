#ifndef student_h
#define student_h

struct Student;


typedef struct Student{
    char lastname[50];
    char matrikelNr[20];

    Date start;
    Date end;
    Date bithday;
}Student;


#endif

