
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>

const int MIN_YEAR = 1900;
const int MAX_YEAR = 2100;

static char SAVE_FILE[20] = "saveFile.csv";
static char TEST_SAVE_FILE[20] = "testSaveFile.csv";

typedef struct Date{
  int day;
  int month;
  int year;
}Date;


typedef struct Student{
  char lastname[50];
  char matrikelNr[50];

  struct Date birthday;
  struct Date start;
  struct Date end;

  int isSaved;

  struct Student *next_node;
  struct Student *prev_node;
}Student;


typedef struct List{
  struct Student *first_node;
  struct Student *last_node;

  int length;
}List;

List *StudentList;

int isTestMode = 0;


/*//clearConsole//
  clears the console

  reference: https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c 13.12.2021 13:54
*/
void clearConsole()
{
  system("@cls||clear");
}

/*//initList//
  reserves memory and sets pointer to NULL

  returns 1 if successfull
  returns -1 for any bad parameter
*/
int initList(List *list){

  if(list == NULL) return -1;

  list->first_node = NULL;
  list->last_node= NULL;
  list->length = 0;

  return 1;
}


/*//freeList//

*/
int freeList(List *list){

  if(list == NULL) return -1;

  Student *currentNode = list->first_node;

  while(currentNode != NULL){
    free(currentNode);
  }

  free(StudentList);

  return 1;
}


/*//initStudent//
  reserves memory and sets pointer to NULL

  returns 1 if successfull
  returns -1 for any bad parameter
*/
int initStudent(Student *student){

  if(student == NULL) return -1;
  
  student->isSaved = 0;

  strcpy(student->lastname, "");
  strcpy(student->matrikelNr, "");

  student->birthday = (Date) {0,0,0};
  student->start = (Date) {0,0,0};
  student->end = (Date) {0,0,0};

  student->next_node = NULL;
  student->prev_node = NULL;

  return 1;
}

/*//matrikelNrExists//
  returns 1 if exits
  returns 0 if not exists
  returns -1 for any bad parameter
*/
int matrikelNrExists(List *list, char *targetMatrikelNr){
  if(list == NULL) return -1;
  if(targetMatrikelNr == NULL) return -1;

  Student *currentNode = list->first_node;

  while(currentNode != NULL){
    
    if(strcmp(currentNode->matrikelNr, targetMatrikelNr) == 0)return 1;

    currentNode = currentNode->next_node;
  }

  return 0;
}


/*//dateIsLeapYear//
  return 1 if the date is valid
  returns 0 if the date is invalid

  reference for leapyear-calculation:
    https://docs.microsoft.com/de-de/office/troubleshoot/excel/determine-a-leap-year 11/23/2021 09:27pm
*/
int dateIsLeapYear(int year){
  if(year < 0) return 0;

  if(year % 4 == 0){
    if(year % 100 == 0){
      if(year % 400 == 0){
        return 1;
      }
    }else{
      return 1;  
    }
  }
  return 0;
}


/*//dateIsValid//
  return 1 if date is valid
  returns 0 if date is invalid
  returns -1 for any bad parameter
*/
int dateIsValid(Date *date){

  if(date == NULL) return -1;
  if(date->day == 0 && date->month == 0 && date->year == 0){
    if(!isTestMode) printf("|Invalid Date: --> Wrong Format\n|\n");
    return -1;
  }

  int day = date->day;
  int month = date->month;
  int year = date->year;

  if(year < MIN_YEAR || year > MAX_YEAR) {
    if(!isTestMode) printf("|Invalid Date --> year not in range %d - %d\n|\n", MIN_YEAR, MAX_YEAR);
    return 0;   
  }
  if(month < 1 || month > 12){ 
    if(!isTestMode) printf("|Invalid Date --> month not in range 1 - 12\n|\n");
    return 0; 
  }   
  if(day < 1 || day > 31){
    if(!isTestMode) printf("|Invalid Date --> day not in range 1 - 31\n|\n");
    return 0;
  } 

  if(month == 2){
    if(dateIsLeapYear(year)){
      if(day <= 29){
        return 1;
      }
      if(!isTestMode)  printf("|Invalid Date --> day > 29\n|\n");
      return 0;
    }
    if(day <= 28){
      return 1;
    }
    if(!isTestMode) printf("|Invalid Date --> day > 28\n|\n");
    return 0;
  }

  if(month == 4 || month == 6 || month == 9 || month == 11){
    if(day > 30){
      if(!isTestMode) printf("|Invalid Date --> day > 30\n|\n");
      return 0;
    }
  }
  return 1;
}

/*//compareDates//
  returns 1 if date1 > date2
  returns 0 if date1 == date2
  return -1 if date1 < date2
  return -2 if any parameter is invalid
*/
int compareDates(Date *date1, Date *date2){

  if(date1 == NULL) return -2;
  if(date2 == NULL) return -2;

  if(date1->year > date2->year) return 1;
  if(date1->year < date2->year) return -1;

  if(date1->month > date2->month) return 1;
  if(date1->month < date2->month) return -1;

  if(date1->day > date2->day) return 1;
  if(date1->day < date2->day) return -1;

  return 0;
}

/*//compareDates//
  subtracts date1 from date2 and returns the result as date

  returns NULL if any parameter is bad
*/
Date *computeDiffDates(Date *date1, Date *date2){
  
  if(date1 == NULL) return NULL;
  if(date2 == NULL) return NULL;

  Date *dateResult = (Date *) malloc(sizeof(Date));

  dateResult->day = date2->day - date1->day;
  dateResult->month = date2->month - date1->month;
  dateResult->year = date2->year - date1->year;

  return dateResult;
}


/*//studentIsValid//

  return 1 if valid
  returns 0 if any data is invalid
  returns -1 if student is NULL

*/
int studentIsValid(Student *student){
  if(student == NULL) return -1;

  if(strcmp(student->lastname,"") == 0) return 0;
  if(strcmp(student->matrikelNr, "") == 0) return 0;
  if(dateIsValid(&student->birthday) <= 0) return 0;
  if(dateIsValid(&student->start) <= 0) return 0;
  if(dateIsValid(&student->end) <= 0) return 0;

  if(compareDates(&student->birthday, &student->start) == 0){ 
    if(!isTestMode)  printf("|Invalid Date --> Birthday = Study Start\n|\n");
    return 0;
  }
  if(compareDates(&student->birthday, &student->start) == 1){
    if(!isTestMode)  printf("|Invalid Date --> Birthday > Study Start\n|\n");
    return 0;
  }
  if(compareDates(&student->start, &student->end) == 0){ 
    if(!isTestMode) printf("|Invalid Date --> Study Start = Study End\n|\n");
    return 0;
  }
  if(compareDates(&student->start, &student->end) == 1){
    if(!isTestMode)  printf("|Invalid Date::Study Start > Study End\n|\n");
    return 0;
  }

  return 1;
}


/*//createStudent//
  return a Student *
  returns NULL if any parameter is invalid
*/
Student *createStudent(char *matrikelNr, char *lastname, Date birthday, Date start, Date end, int isSaved){

  Student *student = (Student *) malloc(sizeof(Student));
  initStudent(student);

  if(matrikelNr == NULL || lastname == NULL){

    free(student);
    return NULL;
  } 

  strcpy(student->matrikelNr, matrikelNr);
  strcpy(student->lastname, lastname);
  student->birthday = birthday;
  student->start = start;
  student->end = end;
  student->isSaved = isSaved;

  if(studentIsValid(student) != 1){

    free(student);
    return NULL;
  } 

  return student;
}


/*//setDate//
  returns 1 if successful
  returns 0 if one or more values invalid
  return -1 if date == NULL
*/
int setDate(Date *date, int day, int month, int year){

  if(date == NULL) return -1;
  Date tempDate = {day, month, year};
  if(dateIsValid(&tempDate) == 0) return 0;

  date->day = day;
  date->month = month;
  date->year = year;

  return 1;
}

/*//compareNodes//
    returns the matrikelNr of 'node1' with 'node2'

  'node1'  > 'node2' return 1
  'node1'  < 'node2' return -1
  'node1' == 'node2' return 0

  'node1' == NULL return 1
  'node2' == NULL return 1
*/
int compareNodes(Student *node1, Student *node2){
  if (node1 == NULL) return 1;
  if (node2 == NULL) return 1;

  int cmp = strcmp(node1->matrikelNr, node2->matrikelNr);

  return cmp;
}


/*//printNumberStudents//
  returns 1 if successfull
  returns 0 if not successfull
  returns -1 if list == NULL
*/
int printNumberStudents(List *list){
  if(list == NULL) return -1;
  //TODO: get the number Students from file

  int cntStudentsInMem = 0;
  int cntStudentsSaved = 0;

  Student *currentNode = list->first_node;

  while(currentNode != NULL){
    
    if(currentNode->isSaved == 1) cntStudentsSaved++;
    cntStudentsInMem++;

    currentNode = currentNode->next_node;

  }

  printf("\n");
  printf("|Number of Students (in memory)::%d\n", cntStudentsInMem);
  printf("|Number of Students (saved)::%d\n", cntStudentsSaved);
  printf("\n");

  return 1;
}

/*//getStudentByMatrikelNr//
  returns a Student from 'list' by comparing 'matrikelNr'
*/
Student *getStudentByMatrikelNr(List *list, char *matrikelNr){

  if(list == NULL) return NULL;   /*Case0: list == NULL*/
  if(list->first_node == NULL) return NULL;   /*Case1: list has no nodes*/

  Student *currentStudent = list->first_node;
  int cmpResult;
  int hasNext = 0;

  do{
    cmpResult = strcmp(currentStudent->matrikelNr, matrikelNr);
  
    if(cmpResult == 0) {
      return currentStudent;    /*Case2: matrikelNr found*/
    }

    if(currentStudent->next_node == NULL){
      hasNext = 0;
    }else{
      hasNext = 1;
    }

    currentStudent = currentStudent->next_node;

  }while(hasNext);

  return NULL;    /*Case3: matrikelNr not found*/
}

/*//addStudent//
  inserts 'node' at the alphabetically destined position

  returns 1 if successfull
  return -1 for any bad parameter
*/
int addStudent(List *list, Student *node){

  if(list == NULL) return -1;
  if(node == NULL) return -1;

  if(list->first_node == NULL){
      list->first_node = node;
      list->last_node = node;
      list->length++;
      return 1;
  }

  Student *currentNode = list->first_node;
  Student *prevNode;
  Student *nextNode;

  int currentCmpResult = 0;
  int prevCmpResult = 0;

  do{
    currentCmpResult = compareNodes(currentNode, node);
    if(currentCmpResult > 0 && prevCmpResult < 0){
      prevNode = currentNode->prev_node;
      nextNode = currentNode;

      node->prev_node = prevNode;
      node->next_node = nextNode;

      prevNode->next_node = node;
      nextNode->prev_node = node;
      break;
    }
    else if(currentCmpResult > 0){
      list->first_node = node;
      node->next_node = currentNode;
      currentNode->prev_node = node;
      break;
    }
    else if(currentNode->next_node == NULL){
      currentNode->next_node = node;
      node->prev_node = currentNode;
      list->last_node = node;
      break;
    }

    currentNode = currentNode->next_node;
    prevCmpResult = currentCmpResult;

  }while(1);

  list->length++;
  return 1;
}

/*//deleteStudent//
  removes a Student from 'list' by comparing its 'matrikelNr'

  returns 1 if successfull
  returns 0 if 'matrikelNr' not found
  returns -1 for any bad parameter
*/
int deleteStudent(List *list, char *matrikelNr){
  if(list == NULL) return -1;
  if(matrikelNr == NULL) return -1;

  Student *node = getStudentByMatrikelNr(list, matrikelNr);
  Student *prevNode = NULL;
  Student *nextNode = NULL;

  if(node == NULL){
    return 0;
  }

  if(list->length == 1){

    list->first_node = NULL;
    list->last_node = NULL;
    free(node);

  }else if(list->first_node == node){

    nextNode = node->next_node;
    nextNode->prev_node = NULL;
    list->first_node = nextNode;
    free(node);
    

  }else if(list->last_node == node){

    prevNode = node->prev_node;
    prevNode->next_node = NULL;
    list->last_node = prevNode;
    free(node);


  }else{
    prevNode = node->prev_node;
    nextNode = node->next_node;

    prevNode->next_node = nextNode;
    nextNode->prev_node = prevNode;

    free(node);
  }

  list->length--;
  node = NULL;

  return 1;
}

/*//inputStudent//
  requests userinput for student data and returns it as 'Student'

*/
 Student inputStudent(){
  Student zwischenspeicher;

  printf_s("***********************************\n");
  printf_s("Nachname\n");
  printf_s("***********************************\n");
  
  printf_s("Bitte geben sie den Nachnamen ein:\n");
  scanf_s("%s", zwischenspeicher.lastname);

  printf_s("***********************************\n");
  printf_s("Matrikelnummer\n");
  printf_s("***********************************\n");

  printf_s("Bitte geben sie die Matrikelnummer ein:\n");
  scanf_s("%i", zwischenspeicher.matrikelNr);

  printf_s("***********************************\n");
  printf_s("Geburtsdatum\n");
  printf_s("***********************************\n");

  printf_s("Bitte geben sie den Geburtstag ein:\n");
  scanf_s("%i", &zwischenspeicher.birthday.day);

  printf_s("Bitte geben sie den Geburtmonat ein:\n");
  scanf_s("%i", &zwischenspeicher.birthday.month);

  printf_s("Bitte geben sie das Geburtsjahr ein:\n");
  scanf_s("%i", &zwischenspeicher.birthday.year);

  printf_s("***********************************\n");
  printf_s("Startdatum\n");
  printf_s("***********************************\n");

  printf_s("Bitte geben sie den Starttag ein ein:\n");
  scanf_s("%i", &zwischenspeicher.start.day);

  printf_s("Bitte geben sie den Startmonat ein:\n");
  scanf_s("%i", &zwischenspeicher.start.month);

  printf_s("Bitte geben sie das Startjahr ein:\n");
  scanf_s("%i", &zwischenspeicher.start.month);

  printf_s("***********************************\n");
  printf_s("Enddatum\n");
  printf_s("***********************************\n");

  printf_s("Bitte geben sie den Endtag ein ein:\n");
  scanf_s("%i", &zwischenspeicher.end.day);

  printf_s("Bitte geben sie den Endmonat ein:\n");
  scanf_s("%i", &zwischenspeicher.end.month);

  printf_s("Bitte geben sie das Endjahr ein:\n");
  scanf_s("%i", &zwischenspeicher.end.month);

 return zwischenspeicher;
}

/*//printStudentByMatrikelNr//
  searches for student with 'matrikelNr' and prints its data

  returns 1 if successfull
  returns 0 if student invalid
  returns -1 if any parameter is bad
  returns -2 if matrikelNr not found
*/
int printStudentByMatrikelNr(List *list, char *matrikelNr){

  if(list == NULL) return -1;
  if(matrikelNr == NULL) return -1;

  Student *node = getStudentByMatrikelNr(list, matrikelNr);

  if(node == NULL) return -2;
  if(studentIsValid(node) != 1) return 0;

  printf("|'Student'={\n");
  printf("|'matrikelNr' = '%s',\n", node->matrikelNr);
  printf("|'lastname' = '%s',\n", node->lastname);
  printf("|'birthday' = '%02d/%02d/%d',\n", node->birthday.month, node->birthday.day, node->birthday.year);
  printf("|'studyStart' = '%02d/%02d/%d',\n", node->start.month, node->start.day, node->start.year);
  printf("|'studyEnd' = '%02d/%02d/%d'\n", node->end.month, node->end.day, node->end.year);
  printf("|}\n");

  return 1;
}


/*//printAllStudents//
  returns number of successfully printed students (>= 0) 

  returns -1 list == NULL
*/
int printAllStudents(List *list){
  
  if(list == NULL) return -1;

  Student *currentNode = list->first_node;
  int printStudentResult = 0;
  int counterPrinted = 0;

  while(currentNode != NULL){

    printStudentResult = printStudentByMatrikelNr(list, currentNode->matrikelNr);
    if(printStudentResult == 1){
      counterPrinted++;
    }
    currentNode = currentNode->next_node;
  }
  
  return counterPrinted;
}



/*//save//
  return 1 if successfull
  return -1 for any bad parameter
  return 0 if problems occured while opening the file
*/
int save(List *list, char* fileName){
  
  if(list == NULL) return -1;

  FILE *f = fopen(fileName, "w");

  if(f == NULL){
    return 0;
  }

  Student *currentNode;

  for(int i = 0; i < list->length; i++){

    if(i == 0){
       currentNode = list->first_node;
    }else{
      currentNode = currentNode->next_node;
    }

    fprintf(f, "%s ", currentNode->matrikelNr);
    fprintf(f, "%s ", currentNode->lastname);
    fprintf(f, "'%02d/%02d/%d' ", currentNode->birthday.month, currentNode->birthday.day, currentNode->birthday.year);
    fprintf(f, "'%02d/%02d/%d' ", currentNode->start.month, currentNode->start.day, currentNode->start.year);
    fprintf(f, "'%02d/%02d/%d'", currentNode->end.month, currentNode->end.day, currentNode->end.year);
    
    if(currentNode != list->last_node){
      fprintf(f, "\n");
    }
  }

  fclose(f);

  return 1;
}


/*//readStudentFile//
  readStudentFile valid data from 'fileName' and add to 'list'
  return 0 if error while on open file
  returns -1 for any bad parameter
*/
int readStudentFile(List *list, char *fileName){

  if(list == NULL) return -1;
  if(fileName == NULL) return -1;


  FILE *f = fopen(fileName, "r");

  if(f == NULL){
    //fprintf(stderr, "error while trying file open ...\n");
    return 0;
  }

  Student readResult;
  Student *currentNode = NULL;

  while((fscanf(f, "%s %s '%2d/%2d/%d' '%2d/%2d/%d' '%2d/%2d/%d'\n",
    &readResult.matrikelNr,   
    &readResult.lastname,
    &readResult.birthday.month, &readResult.birthday.day, &readResult.birthday.year,
    &readResult.start.month, &readResult.start.day, &readResult.start.year,
    &readResult.end.month, &readResult.end.day, &readResult.end.year)) != EOF){

      currentNode = createStudent(
        readResult.matrikelNr, 
        readResult.lastname,
        readResult.birthday,
        readResult.start,
        readResult.end,
        1);

      addStudent(list, currentNode);
      initStudent(&readResult);
    }


  fclose(f);

  return 1;
}


/*//test_addStudent_isFistNode//

*/
void test_addStudent_isFirstNode(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));

  initStudent(s0);

  strcpy(s0->matrikelNr, "test2");

  addStudent(list, s0);

  assert(list->first_node == s0);
  assert(list->last_node == s0);
  assert(list->length == 1);

  assert(s0->next_node == NULL);
  assert(s0->prev_node == NULL);

  free(s0);
  free(list);

  printf("success");
  fflush(stdout);  
}

/*//test_addStudent_isLastNode//

*/
void test_addStudent_isLastNode(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  

  initStudent(s0);
  initStudent(s1);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s1->matrikelNr, "test5");

  addStudent(list, s0);
  addStudent(list, s1);

  assert(list->first_node == s0);
  assert(list->last_node == s1);
  assert(list->length == 2);

  assert(s0->next_node == s1);
  assert(s0->prev_node == NULL);

  assert(s1->next_node == NULL);
  assert(s1->prev_node == s0);

  free(s0);
  free(s1);
  free(list);

  printf("success");
  fflush(stdout);
}


/*//test_addStudent_isNotFirstNode_isNotLastNode//

*/
void test_addStudent_isNotFirstNode_isNotLastNode(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  Student *s2 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);
  initStudent(s2);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s1->matrikelNr, "test5");
  strcpy(s2->matrikelNr, "test3");

  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

  assert(list->first_node == s0);
  assert(list->last_node == s1);
  assert(list->length == 3);

  assert(s0->next_node == s2);
  assert(s0->prev_node == NULL);

  assert(s1->next_node == NULL);
  assert(s1->prev_node == s2);

  assert(s2->next_node == s1);
  assert(s2->prev_node == s0);

  free(s0);
  free(s1);
  free(s2);
  free(list);
  
  printf("success");
  fflush(stdout);
}


/*//test_addStudent_ShouldBeFirst_Case1//

*/
void test_addStudent_ShouldBeFirst_Case1(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  Student *s2 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);
  initStudent(s2);

  strcpy(s0->matrikelNr, "test3");
  strcpy(s1->matrikelNr, "test2");
  strcpy(s2->matrikelNr, "test5");

  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

  assert(list->first_node == s1);
  assert(list->last_node == s2);
  assert(list->length == 3);

  assert(s0->next_node == s2);
  assert(s0->prev_node == s1);

  assert(s1->next_node == s0);
  assert(s1->prev_node == NULL);

  assert(s2->next_node == NULL);
  assert(s2->prev_node == s0);

  free(s0);
  free(s1);
  free(s2);
  free(list);
  
  printf("success");
  fflush(stdout);
}


/*//test_addStudent_ShouldBeFirst_Case2//

*/
void test_addStudent_ShouldBeFirst_Case2(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  Student *s2 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);
  initStudent(s2);

  strcpy(s0->matrikelNr, "test3");
  strcpy(s1->matrikelNr, "test5");
  strcpy(s2->matrikelNr, "test2");

  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

  assert(list->first_node == s2);
  assert(list->last_node == s1);
  assert(list->length == 3);

  assert(s0->next_node == s1);
  assert(s0->prev_node == s2);

  assert(s1->next_node == NULL);
  assert(s1->prev_node == s0);

  assert(s2->next_node == s0);
  assert(s2->prev_node == NULL);

  free(s0);
  free(s1);
  free(s2);
  free(list);
  
  printf("success");
  fflush(stdout);
}


/*//test_addStudent//

*/
void test_addStudent(){
  printf("TEST::%s\n", __func__);

  test_addStudent_isFirstNode();
  printf("\n");

  test_addStudent_isLastNode();
  printf("\n");

  test_addStudent_isNotFirstNode_isNotLastNode();
  printf("\n");

  test_addStudent_ShouldBeFirst_Case1();
  printf("\n");
  
  test_addStudent_ShouldBeFirst_Case2();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);
}




/*//test_deleteStudent_isFirstNode//

*/
void test_deleteStudent_isFirstNode(){
  printf("-->%s::", __func__);
  
  List *list = (List *) malloc(sizeof(List));
  initList(list);
  
  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  Student *s2 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);
  initStudent(s2);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s1->matrikelNr, "test3");
  strcpy(s2->matrikelNr, "test5");

  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);
  
  char *targetMatrikelNr = "test2";

  deleteStudent(list, targetMatrikelNr);

  assert(list->first_node == s1);
  assert(list->last_node == s2);
  assert(list->length == 2);

  assert(s1->prev_node == NULL);
  assert(s1->next_node == s2);

  assert(s2->prev_node == s1);
  assert(s2->next_node == NULL);

  free(s1);
  free(s2);
  free(list);
 
  printf("success");
  fflush(stdout);
}


/*//test_deleteStudent_isLastNode//

*/
void test_deleteStudent_isLastNode(){
  printf("-->%s::", __func__);
 
  List *list = (List *) malloc(sizeof(List));
  initList(list);
  
  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  Student *s2 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);
  initStudent(s2);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s1->matrikelNr, "test5");
  strcpy(s2->matrikelNr, "test3");

  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

  char *targetMatrikelNr = "test5";

  deleteStudent(list, targetMatrikelNr);

  assert(list->first_node == s0);
  assert(list->last_node == s2);
  assert(list->length == 2);

  assert(s0->prev_node == NULL);
  assert(s0->next_node == s2);

  assert(s2->prev_node == s0);
  assert(s2->next_node == NULL);

  free(s0);
  free(s2);
  free(list);

  printf("success");
  fflush(stdout);
}

/*//test_deleteStudent_isOnlyNode//

*/
void test_deleteStudent_isOnlyNode(){
  printf("-->%s::", __func__);
 
  List *list = (List *) malloc(sizeof(List));
  initList(list);
  
  Student *s0 = (Student *) malloc(sizeof(Student));

  initStudent(s0);

  strcpy(s0->matrikelNr, "test2");

  addStudent(list, s0);

  char *targetMatrikelNr = "test2";

  deleteStudent(list, targetMatrikelNr);

  assert(list->first_node == NULL);
  assert(list->last_node == NULL);
  assert(list->length == 0);

  free(list);

  printf("success");
  fflush(stdout);
}


/*//test_deleteStudent_isNotFirst_isNotLast//

*/
void test_deleteStudent_isNotFirst_isNotLast(){
  printf("-->%s::", __func__);
  
  List *list = (List *) malloc(sizeof(List));
  initList(list);
  
  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  Student *s2 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);
  initStudent(s2);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s1->matrikelNr, "test3");
  strcpy(s2->matrikelNr, "test5");

  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

  
  char *targetMatrikelNr = "test3";

  deleteStudent(list, targetMatrikelNr);

  assert(list->first_node == s0);
  assert(list->last_node == s2);
  assert(list->length == 2);

  assert(s0->prev_node == NULL);
  assert(s0->next_node == s2);

  assert(s2->prev_node == s0);
  assert(s2->next_node == NULL);

  free(s0);
  free(s2);
  free(list);
  
  printf("success");
  fflush(stdout);
}


/*//test_deleteStudent//

*/
void test_deleteStudent(){
  printf("TEST::%s\n", __func__);

  test_deleteStudent_isFirstNode();
  printf("\n");

  test_deleteStudent_isLastNode();
  printf("\n");

  test_deleteStudent_isNotFirst_isNotLast();
  printf("\n");

  test_deleteStudent_isOnlyNode();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);
}


void test_getStudentByMatrikelNr_IfStudentFound_ShouldReturn_Pointer(){
  printf("-->%s::", __func__);
  
  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s1->matrikelNr, "test5");

  addStudent(list, s0);
  addStudent(list, s1);

  char *targetMatrikelNr = "test5";
  Student *node = getStudentByMatrikelNr(list, targetMatrikelNr);

  assert(node != NULL);
  assert(node == s1);

  free(s0);
  free(s1);
  free(list);

  printf("success");
  fflush(stdout);
}

void test_getStudentByMatrikelNr_IfStudentNotFound_ShouldReturnNULL(){
  printf("-->%s::", __func__);
  
  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);

  addStudent(list, s0);
  addStudent(list, s1);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s1->matrikelNr, "test5");

  char *targetMatrikelNr = "test3";

  Student *node = getStudentByMatrikelNr(list, targetMatrikelNr);

  assert(node == NULL);

  free(s0);
  free(s1);
  free(list);

  printf("success");
  fflush(stdout);
}

/*//test_getStudentByMatrikelNr_IfListIsNULL_ShouldReturnNULL//

*/
void test_getStudentByMatrikelNr_IfListIsNULL_ShouldReturnNULL(){
  printf("-->%s::", __func__);

  List *list = NULL;
  Student *node = NULL;

  char *targetMatrikelNr = "test5";

  node = getStudentByMatrikelNr(list, targetMatrikelNr);

  assert(node == NULL);

  printf("success");
  fflush(stdout);
}


void test_getStudentByMatrikelNr(){
  printf("TEST::%s\n", __func__);

  test_getStudentByMatrikelNr_IfStudentFound_ShouldReturn_Pointer();
  printf("\n");

  test_getStudentByMatrikelNr_IfStudentNotFound_ShouldReturnNULL();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);
}


/*//test_printStudentByMatrikelNr_IfListIsNULL_ShouldReturn_Minus1//

*/
void test_printStudentByMatrikelNr_IfListIsNULL_ShouldReturn_Minus1(){
  printf("-->%s::", __func__);

  List *list = NULL;
  char *targetMatrikelNr = "test5";

  int returnValue = printStudentByMatrikelNr(list, targetMatrikelNr);

  assert(returnValue == -1);

  printf("success");
  fflush(stdout);
}

/*//test_printStudentByMatrikelNr_IfMatrikelNrIsNULL_ShouldReturn_Minus1//

*/
void test_printStudentByMatrikelNr_IfMatrikelNrIsNULL_ShouldReturn_Minus1(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  char *targetMatrikelNr = NULL;

  int returnValue = printStudentByMatrikelNr(list, targetMatrikelNr);

  assert(returnValue == -1);

  free(list);

  printf("success");
  fflush(stdout);
}


/*//test_printStudentByMatrikelNr_IfStudentIsNULL_ShouldReturn_Minus2//

*/
void test_printStudentByMatrikelNr_IfStudentIsNULL_ShouldReturn_Minus2(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  char *targetMatrikelNr = "test5";
  
  int returnValue = printStudentByMatrikelNr(list, targetMatrikelNr);

  assert(returnValue == -2);

  free(list);

  printf("success");
  fflush(stdout);
}

/*//test_printStudentByMatrikelNr_IfStudentDataIsInvalid_ShouldReturn_0//

*/
void test_printStudentByMatrikelNr_IfStudentDataIsInvalid_ShouldReturn_0(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  initStudent(s0);
  strcpy(s0->matrikelNr, "test5");

  addStudent(list, s0);

  char *targetMatrikelNr = "test5";

  int returnValue = printStudentByMatrikelNr(list, targetMatrikelNr);

  assert(returnValue == 0);

  free(s0);
  free(list);

  printf("success");
  fflush(stdout);
}

/*//test_printStudentByMatrikelNr_IfStudentExists_ShouldReturn_1//

*/
void test_printStudentByMatrikelNr_IfStudentExists_ShouldReturn_1(){
  printf("-->%s::", __func__);
  printf("\n");

  List *list = (List *) malloc(sizeof(Student));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  Student *node = NULL;
  int printSuccessfull = 0;

  initStudent(s0);
  initStudent(s1);

  strcpy(s0->matrikelNr, "test2");

  strcpy(s1->matrikelNr, "test5");
  strcpy(s1->lastname, "mustermann");
  setDate(&s1->birthday, 10, 11, 2000);
  setDate(&s1->start, 1, 10, 2021);
  setDate(&s1->end, 30, 9, 2024);

  addStudent(list, s0);
  addStudent(list, s1);

  char *targetMatrikelNr = "test5";

  node = getStudentByMatrikelNr(list, targetMatrikelNr);

  printSuccessfull = printStudentByMatrikelNr(list, targetMatrikelNr);

  assert(node == s1);
  assert(printSuccessfull == 1);

  free(s0);
  free(s1);
  free(list);

  printf("\n");
  printf("success");
  fflush(stdout);
}

/*//test_printStudent//

*/
void test_printStudent(){
  printf("TEST::%s\n", __func__);

  test_printStudentByMatrikelNr_IfStudentExists_ShouldReturn_1();
  printf("\n");

  test_printStudentByMatrikelNr_IfStudentDataIsInvalid_ShouldReturn_0();
  printf("\n");

  test_printStudentByMatrikelNr_IfListIsNULL_ShouldReturn_Minus1();
  printf("\n");

  test_printStudentByMatrikelNr_IfMatrikelNrIsNULL_ShouldReturn_Minus1();
  printf("\n");

  test_printStudentByMatrikelNr_IfStudentIsNULL_ShouldReturn_Minus2();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);
}

/*//test_printAllStudents_IfSuccessfullyPrinted0_ShouldReturn_0()
TODO:
*/
void test_printAllStudents_IfSuccessfullyPrinted0_ShouldReturn_0(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  Student *s2 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);
  initStudent(s2);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s1->matrikelNr, "test3");
  strcpy(s2->matrikelNr, "test5");
 
  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

  int returnValue = printAllStudents(list);

  assert(returnValue == 0);
  //TODO: assert vlaues not changed

  free(s0);
  free(s1);
  free(s2);
  free(list);

  printf("success");
  fflush(stdout);
}

/*//test_printAllStudents_IfSuccessfull_ShouldReturn_NumberSuccessfullyPrinted()
TODO:
*/
void test_printAllStudents_IfSuccessfull_ShouldReturn_NumberSuccessfullyPrinted(){
  printf("-->%s::", __func__);
  printf("\n");

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  Student *s2 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);
  initStudent(s2);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s0->lastname, "otto");
  setDate(&s0->birthday, 11, 11, 1998);
  setDate(&s0->start, 1, 10, 2021);
  setDate(&s0->end, 30, 9, 2024);

  strcpy(s1->matrikelNr, "test3");
  strcpy(s1->lastname, "mueller");
  setDate(&s1->birthday, 21, 8, 1997);
  setDate(&s1->start, 1, 10, 2021);
  setDate(&s1->end, 30, 9, 2024);
  
  strcpy(s2->matrikelNr, "test5");
  strcpy(s2->lastname, "mustermann");
  setDate(&s2->birthday, 10, 11, 2000);
  setDate(&s2->start, 1, 10, 2021);
  setDate(&s2->end, 30, 9, 2024);

  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

  int returnValue = printAllStudents(list);

  assert(returnValue == 3);
  assert(list->length == 3);
  //TODO: assert values not changed

  free(s0);
  free(s1);
  free(s2);
  free(list);

  printf("success");
  fflush(stdout);
}

/*//test_printAllStudents_IfListIsNULL_ShouldReturn_Minus1()
TODO:
*/
void test_printAllStudents_IfListIsNULL_ShouldReturn_Minus1(){
  printf("-->%s::", __func__);
  
  List *list = NULL;
  int returnValue = printAllStudents(list);

  assert(returnValue == -1);
  
  printf("success");
  fflush(stdout);
}

/*//test_printAllStudents()
TODO:
*/
void test_printAllStudents(){
  printf("TEST::%s\n", __func__);

  test_printAllStudents_IfListIsNULL_ShouldReturn_Minus1();
  printf("\n");

  test_printAllStudents_IfSuccessfullyPrinted0_ShouldReturn_0();
  printf("\n");

  test_printAllStudents_IfSuccessfull_ShouldReturn_NumberSuccessfullyPrinted();
  printf("\n");

  printf("END_TEST::%s::succes\n\n", __func__);
}


/*//test_printNumberStudents_IfListIsNULL_ShouldReturn_Minus1()//

*/
void test_printNumberStudents_IfListIsNULL_ShouldReturn_Minus1(){
  printf("-->%s::", __func__);
  
  List *list = NULL;

  int returnValue = printNumberStudents(list);

  assert(returnValue == -1);

  printf("success");
  fflush(stdout);
}

/*//test_printNumberStudents_IfSuccessfull_ShouldReturn_1()//

*/
void test_printNumberStudents_IfSuccessfull_ShouldReturn_1(){
  printf("-->%s::", __func__);
  printf("\n");
  
  List *list = (List *) malloc(sizeof(List));
  initList(list);

  int returnValue = printNumberStudents(list);

  assert(returnValue == 1);

  free(list);

  printf("success");
  fflush(stdout);
}

/*//test_printNumberStudents()//

*/
void test_printNumberStudents(){
  printf("TEST::%s\n", __func__);
  
  test_printNumberStudents_IfListIsNULL_ShouldReturn_Minus1();
  printf("\n");

  test_printNumberStudents_IfSuccessfull_ShouldReturn_1();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);
}


/*//test_dateIsLeapYear_IfIsLeapYear_ShouldReturn_1

*/
void test_dateIsLeapYear_IfIsLeapYear_ShouldReturn_1(){
  printf("-->%s::", __func__);

  int years[16] = {1904, 1920, 1984, 1992, 1996, 2000, 2004, 2012, 2016, 2020, 2024, 2028, 2032, 2036, 2040, 2044};
  int yearsLen = sizeof(years) / sizeof(int);
  int isLeapYear;

  for(int i = 0; i< yearsLen; i++){

    isLeapYear = dateIsLeapYear(years[i]);
    assert(isLeapYear == 1);
  }
  
  printf("success");
  fflush(stdout);
}


/*//test_dateIsLeapYear_IfIsNotLeapYear_ShouldReturn_0

*/
void test_dateIsLeapYear_IfIsNotLeapYear_ShouldReturn_0(){
  printf("-->%s::", __func__);

  int years[16] = {1909, 1910, 1950, 1966, 1997, 1999, 2010, 2021, 2022, 2023, 2025, 2026, 2027, 2029, 2043, 2045};
  int yearsLen = sizeof(years) / sizeof(int);
  int isLeapYear;

  for(int i = 0; i< yearsLen; i++){

    isLeapYear = dateIsLeapYear(years[i]);
    assert(isLeapYear == 0);
  }

  printf("success");
  fflush(stdout);
}

/*//test_dateIsLeapYear_IfYearIsNegative_ShouldReturn_0

*/
void test_dateIsLeapYear_IfYearIsNegative_ShouldReturn_0(){
  printf("-->%s::", __func__);

  int year = -1904;
  int isLeapYear = dateIsLeapYear(year);

  assert(isLeapYear == 0);

  printf("success");
  fflush(stdout);
}

/*//test_dateIsLeapYear

*/
void test_dateIsLeapYear(){
  printf("TEST::%s\n", __func__);

  
  test_dateIsLeapYear_IfIsNotLeapYear_ShouldReturn_0();
  printf("\n");

  test_dateIsLeapYear_IfIsLeapYear_ShouldReturn_1();
  printf("\n");

  test_dateIsLeapYear_IfYearIsNegative_ShouldReturn_0();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);  
}


/*//test_dateIsValid_IfDayNotInRange_ShouldReturn_0//

*/
void test_dateIsValid_IfDayNotInRange_ShouldReturn_0(){
  printf("-->%s::", __func__);
  
  
  Date dates[8] = {
    {0,10,2021},
    {31,4,2021},
    {31,6,2021},
    {31,9,2021},
    {31,11,2021},
    {0,2,2021},
    {29,2,2021},
    {45,3,2021}
  };

  int datesLen = sizeof(dates) / sizeof(Date);
  int isValid;

  for(int i = 0; i< datesLen; i++){

    isValid = dateIsValid(&dates[i]);
    assert(isValid == 0);
  }

  printf("success");
  fflush(stdout);
}

/*//test_dateIsValid_IfDayNotInRange_ShouldReturn_0//

*/
void test_dateIsValid_IfMonthNotInRange_ShouldReturn_0(){
   printf("-->%s::", __func__);
  
  
  Date dates[2] = {
    {1,0,2021},
    {1,13,2021}
  };

  int datesLen = sizeof(dates) / sizeof(Date);
  int isValid;

  for(int i = 0; i< datesLen; i++){

    isValid = dateIsValid(&dates[i]);
    assert(isValid == 0);
  }

  printf("success");
  fflush(stdout);
}

/*//test_dateIsValid_IfDayNotInRange_ShouldReturn_0//

*/
void test_dateIsValid_IfYearNotInRange_ShouldReturn_0(){
   printf("-->%s::", __func__);
  
  
  Date dates[2] = {
    {1,0, MIN_YEAR - 1},
    {1,13,MAX_YEAR + 1}
  };

  int datesLen = sizeof(dates) / sizeof(Date);
  int isValid;

  for(int i = 0; i< datesLen; i++){

    isValid = dateIsValid(&dates[i]);
    assert(isValid == 0);
  }

  printf("success");
  fflush(stdout);
}


/*//test_dateIsValid//

*/
void test_dateIsValid(){
  printf("TEST::%s\n", __func__);

  test_dateIsValid_IfDayNotInRange_ShouldReturn_0();
  printf("\n");

  test_dateIsValid_IfMonthNotInRange_ShouldReturn_0();
  printf("\n");

  test_dateIsValid_IfYearNotInRange_ShouldReturn_0();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);  
}


/*//test_setDate_IfDateIsNULL_ShouldReturn_Minus1//

*/
void test_setDate_IfDateIsNULL_ShouldReturn_Minus1(){
  printf("-->%s::", __func__);

  Date *date = NULL;
  int returnValue = setDate(date, 23, 11, 2021);

  assert(returnValue == -1);
  assert(date == NULL);
  
  printf("success");
  fflush(stdout);
}

/*//test_setDate_IfDateInvalid_ShouldReturn_0//

*/
void test_setDate_IfDateInvalid_ShouldReturn_0(){
  printf("-->%s::", __func__);

  Date date;
  int returnValue = setDate(&date, 0, 11, 2021);

  assert(returnValue == 0);
  
  printf("success");
  fflush(stdout);
}

/*//test_setDate_IfDateValid_ShouldReturn_1//

*/
void test_setDate_IfDateValid_ShouldReturn_1(){
  printf("-->%s::", __func__);

  Date date = {};
  int returnValue = setDate(&date, 23, 11, 2021);
  
  assert(returnValue == 1);
  assert(date.day == 23 && date.month == 11 && date.year == 2021);
  
  printf("success");
  fflush(stdout);
}


/*//test_setDate//

*/
void test_setDate(){
  printf("TEST::%s\n", __func__);

  test_setDate_IfDateIsNULL_ShouldReturn_Minus1();
  printf("\n");

  test_setDate_IfDateInvalid_ShouldReturn_0();
  printf("\n");

  test_setDate_IfDateValid_ShouldReturn_1();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);  
}



/*//test_studentIsValid_IfValid_ShouldRerurn_1//

*/
void test_studentIsValid_IfValid_ShouldRerurn_1(){
  printf("-->%s::", __func__);
  Student s0;
  initStudent(&s0);

  strcpy(s0.lastname, "mustermann");
  strcpy(s0.matrikelNr, "test5");

  setDate(&s0.birthday, 10, 2, 1999);
  setDate(&s0.start, 1, 10, 2021);
  setDate(&s0.end, 30, 9, 2024);

  assert(dateIsValid(&s0.birthday));
  assert(dateIsValid(&s0.start));
  assert(dateIsValid(&s0.end));

  int returnValue = studentIsValid(&s0);

  assert(returnValue == 1);
  assert(strcmp(s0.lastname, "mustermann") == 0);
  assert(strcmp(s0.matrikelNr, "test5") == 0);
  assert(s0.birthday.day == 10 && s0.birthday.month == 2 && s0.birthday.year == 1999);
  assert(s0.start.day == 1 && s0.start.month == 10 && s0.start.year == 2021);
  assert(s0.end.day == 30 && s0.end.month == 9 && s0.end.year == 2024);

  printf("success");
  fflush(stdout);
}

/*//test_studentIsValid_IfLastnameIsInvalid_ShouldReturn_0//

*/
void test_studentIsValid_IfLastnameIsInvalid_ShouldReturn_0(){
  printf("-->%s::", __func__);
  Student s0;
  initStudent(&s0);

  strcpy(s0.matrikelNr, "test5");

  setDate(&s0.birthday, 10, 2, 1999);
  setDate(&s0.start, 1, 10, 2021);
  setDate(&s0.end, 30, 9, 2024);

  assert(dateIsValid(&s0.birthday));
  assert(dateIsValid(&s0.start));
  assert(dateIsValid(&s0.end));

  int returnValue = studentIsValid(&s0);

  assert(returnValue == 0);
  assert(strcmp(s0.lastname, "") == 0);
  assert(strcmp(s0.matrikelNr, "test5") == 0);
  assert(s0.birthday.day == 10 && s0.birthday.month == 2 && s0.birthday.year == 1999);
  assert(s0.start.day == 1 && s0.start.month == 10 && s0.start.year == 2021);
  assert(s0.end.day == 30 && s0.end.month == 9 && s0.end.year == 2024);

  printf("success");
  fflush(stdout);
}

/*//test_studentIsValid_IfMatrikelNrIsInvalid_ShouldReturn_0//

*/
void test_studentIsValid_IfMatrikelNrIsInvalid_ShouldReturn_0(){
  printf("-->%s::", __func__);
  Student s0;
  initStudent(&s0);

  strcpy(s0.lastname, "mustermann");

  setDate(&s0.birthday, 10, 2, 1999);
  setDate(&s0.start, 1, 10, 2021);
  setDate(&s0.end, 30, 9, 2024);

  assert(dateIsValid(&s0.birthday));
  assert(dateIsValid(&s0.start));
  assert(dateIsValid(&s0.end));

  int returnValue = studentIsValid(&s0);

  assert(returnValue == 0);
  assert(strcmp(s0.lastname, "mustermann") == 0);
  assert(strcmp(s0.matrikelNr, "") == 0);
  assert(s0.birthday.day == 10 && s0.birthday.month == 2 && s0.birthday.year == 1999);
  assert(s0.start.day == 1 && s0.start.month == 10 && s0.start.year == 2021);
  assert(s0.end.day == 30 && s0.end.month == 9 && s0.end.year == 2024);

  printf("success");
  fflush(stdout);
}

/*//test_studentIsValid_IfBirthdayIsInvalid_ShouldReturn_Munus1//

*/
void test_studentIsValid_IfBirthdayIsInvalid_ShouldReturn_Munus1(){
  printf("-->%s::", __func__);
  Student s0;
  initStudent(&s0);

  strcpy(s0.lastname, "mustermann");
  strcpy(s0.matrikelNr, "test5");

  setDate(&s0.birthday, 0, 0, 0);
  setDate(&s0.start, 1, 10, 2021);
  setDate(&s0.end, 30, 9, 2024);

  assert(dateIsValid(&s0.birthday) == -1);
  assert(dateIsValid(&s0.start));
  assert(dateIsValid(&s0.end));

  int returnValue = studentIsValid(&s0);

  assert(returnValue == 0);
  assert(strcmp(s0.lastname, "mustermann") == 0);
  assert(strcmp(s0.matrikelNr, "test5") == 0);
  assert(s0.birthday.day == 0 && s0.birthday.month == 0 && s0.birthday.year == 0);
  assert(s0.start.day == 1 && s0.start.month == 10 && s0.start.year == 2021);
  assert(s0.end.day == 30 && s0.end.month == 9 && s0.end.year == 2024);

  printf("success");
  fflush(stdout);
}

/*//test_studentIsValid_IfStartIsInvalid_ShouldReturn_0//

*/
void test_studentIsValid_IfStartIsInvalid_ShouldReturn_0(){
 printf("-->%s::", __func__);
  Student s0;
  initStudent(&s0);

  strcpy(s0.lastname, "mustermann");
  strcpy(s0.matrikelNr, "test5");

  setDate(&s0.birthday, 10, 2, 1999);
  setDate(&s0.start, 0, 0, 0);
  setDate(&s0.end, 30, 9, 2024);

  assert(dateIsValid(&s0.birthday));
  assert(dateIsValid(&s0.start) == -1);
  assert(dateIsValid(&s0.end));

  int returnValue = studentIsValid(&s0);

  assert(returnValue == 0);
  assert(strcmp(s0.lastname, "mustermann") == 0);
  assert(strcmp(s0.matrikelNr, "test5") == 0);
  assert(s0.birthday.day == 10 && s0.birthday.month == 2 && s0.birthday.year == 1999);
  assert(s0.start.day == 0 && s0.start.month == 0 && s0.start.year == 0);
  assert(s0.end.day == 30 && s0.end.month == 9 && s0.end.year == 2024);

  printf("success");
  fflush(stdout);
}

/*//test_studentIsValid_IfEndIsInvalid_ShouldReturn_0//

*/
void test_studentIsValid_IfEndIsInvalid_ShouldReturn_0(){
  printf("-->%s::", __func__);
  Student s0;
  initStudent(&s0);

  strcpy(s0.lastname, "mustermann");
  strcpy(s0.matrikelNr, "test5");

  setDate(&s0.birthday, 10, 2, 1999);
  setDate(&s0.start, 1, 10, 2021);
  setDate(&s0.end, 0, 0, 0);

  assert(dateIsValid(&s0.birthday));
  assert(dateIsValid(&s0.start));
  assert(dateIsValid(&s0.end) == -1);

  int returnValue = studentIsValid(&s0);

  assert(returnValue == 0);
  assert(strcmp(s0.lastname, "mustermann") == 0);
  assert(strcmp(s0.matrikelNr, "test5") == 0);
  assert(s0.birthday.day == 10 && s0.birthday.month == 2 && s0.birthday.year == 1999);
  assert(s0.start.day == 1 && s0.start.month == 10 && s0.start.year == 2021);
  assert(s0.end.day == 0 && s0.end.month == 0 && s0.end.year == 0);

  printf("success");
  fflush(stdout);
}

/*//test_studentIsValid_IfStartGreaterThenEnd_ShouldReturn_0//

*/
void test_studentIsValid_IfStartGreaterThenEnd_ShouldReturn_0(){
  printf("-->%s::", __func__);
  Student s0;
  initStudent(&s0);

  strcpy(s0.lastname, "mustermann");
  strcpy(s0.matrikelNr, "test5");

  setDate(&s0.birthday, 10, 2, 1999);
  setDate(&s0.start, 30, 9, 2024);
  setDate(&s0.end, 1, 10, 2021);

  assert(dateIsValid(&s0.birthday));
  assert(dateIsValid(&s0.start));
  assert(dateIsValid(&s0.end));

  int returnValue = studentIsValid(&s0);

  assert(returnValue == 0);
  assert(strcmp(s0.lastname, "mustermann") == 0);
  assert(strcmp(s0.matrikelNr, "test5") == 0);
  assert(s0.birthday.day == 10 && s0.birthday.month == 2 && s0.birthday.year == 1999);
  assert(s0.start.day == 30 && s0.start.month == 9 && s0.start.year == 2024);
  assert(s0.end.day == 1 && s0.end.month == 10 && s0.end.year == 2021);

  printf("success");
  fflush(stdout);
}

/*//test_studentIsValid_IfStartEqualsEnd_ShouldReturn_0//

*/
void test_studentIsValid_IfStartEqualsEnd_ShouldReturn_0(){
  printf("-->%s::", __func__);
  Student s0;
  initStudent(&s0);

  strcpy(s0.lastname, "mustermann");
  strcpy(s0.matrikelNr, "test5");

  setDate(&s0.birthday, 10, 2, 1999);
  setDate(&s0.start, 1, 10, 2021);
  setDate(&s0.end, 1, 10, 2021);

  assert(dateIsValid(&s0.birthday));
  assert(dateIsValid(&s0.start));
  assert(dateIsValid(&s0.end));

  int returnValue = studentIsValid(&s0);

  assert(returnValue == 0);
  assert(strcmp(s0.lastname, "mustermann") == 0);
  assert(strcmp(s0.matrikelNr, "test5") == 0);
  assert(s0.birthday.day == 10 && s0.birthday.month == 2 && s0.birthday.year == 1999);
  assert(s0.start.day == 1 && s0.start.month == 10 && s0.start.year == 2021);
  assert(s0.end.day == 1 && s0.end.month == 10 && s0.end.year == 2021);

  printf("success");
  fflush(stdout);
}


/*//test_studentIsValid_IfStudentIsNULL_ShouldReturn_Minus1//

*/
void test_studentIsValid_IfStudentIsNULL_ShouldReturn_Minus1(){
  printf("-->%s::", __func__);
  Student *s0 = NULL;

  int returnValue = studentIsValid(s0);

  assert(returnValue == -1);
  assert(s0 == NULL);

  printf("success");
  fflush(stdout);
}


/*//test_studentIsValid//

*/
void test_studentIsValid(){
  printf("TEST::%s\n", __func__);

  test_studentIsValid_IfValid_ShouldRerurn_1();
  printf("\n");

  test_studentIsValid_IfLastnameIsInvalid_ShouldReturn_0();
  printf("\n");

  test_studentIsValid_IfMatrikelNrIsInvalid_ShouldReturn_0();
  printf("\n");

  test_studentIsValid_IfBirthdayIsInvalid_ShouldReturn_Munus1();
  printf("\n");

  test_studentIsValid_IfStartIsInvalid_ShouldReturn_0();
  printf("\n");

  test_studentIsValid_IfEndIsInvalid_ShouldReturn_0();
  printf("\n");

  test_studentIsValid_IfStudentIsNULL_ShouldReturn_Minus1();
  printf("\n");

  test_studentIsValid_IfStartGreaterThenEnd_ShouldReturn_0();
  printf("\n");

  test_studentIsValid_IfStartEqualsEnd_ShouldReturn_0();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);  
}


/*//test_compareDates_IfDate1GreaterDate2_ShouldReturn_1()//

*/
void test_compareDates_IfDate1GreaterDate2_ShouldReturn_1(){
  printf("-->%s::", __func__);

  Date dates[5] = {
    {31,8,2021},
    {31,8,2020},
    {31,7,2020},
    {30,7,2020}
  };

  int datesLen = sizeof(dates) / sizeof(Date);
  int returnValue;

  for(int i = 0; i < datesLen - 1; i++){
    
    returnValue = compareDates(&dates[i], &dates[i+1]);  
    assert(returnValue == 1);
  }

  printf("success");
  fflush(stdout);
}

/*//test_compareDates_IfDate1EqualsDate2_ShouldReturn_0()//

*/
void test_compareDates_IfDate1EqualsDate2_ShouldReturn_0(){
  printf("-->%s::", __func__);

  Date d1 = {24,11,2021};
  Date d2 = {24,11,2021};

  int returnValue = compareDates(&d1, &d2); 
    
  assert(returnValue == 0);

  printf("success");
  fflush(stdout);
}

/*//test_compareDates_IfDate1LessDate2_ShouldReturn_Minus1()//

*/
void test_compareDates_IfDate1LessDate2_ShouldReturn_Minus1(){
 printf("-->%s::", __func__);

  Date dates[4] = {
    {30,7,2020},
    {31,7,2020},
    {31,8,2020},
    {31,8,2021}
  };

  int datesLen = sizeof(dates) / sizeof(Date);
  int returnValue;

  for(int i = 0; i < datesLen - 1; i++){
    
    returnValue = compareDates(&dates[i], &dates[i+1]);  
    assert(returnValue == -1);
  }

  printf("success");
  fflush(stdout);
}

/*//test_compareDates_IfDate1IsNULL_ShouldReturn_Minus2()//

*/
void test_compareDates_IfDate1IsNULL_ShouldReturn_Minus2(){
  printf("-->%s::", __func__);

  Date *d1 = NULL;
  Date d2;

  int returnValue = compareDates(d1, &d2);
  
  assert(returnValue == -2);

  printf("success");
  fflush(stdout);
}

/*//test_compareDates_IfDate2IsNULL_ShouldReturn_Minus2()//

*/
void test_compareDates_IfDate2IsNULL_ShouldReturn_Minus2(){
 printf("-->%s::", __func__);

  Date d1;
  Date *d2 = NULL;

  int returnValue = compareDates(&d1, d2);
  
  assert(returnValue == -2);
  
  printf("success");
  fflush(stdout);
}

/*//test_compareDates()//

*/
void test_compareDates(){
  printf("TEST::%s\n", __func__);
  
  test_compareDates_IfDate1IsNULL_ShouldReturn_Minus2();
  printf("\n");

  test_compareDates_IfDate2IsNULL_ShouldReturn_Minus2();
  printf("\n");

  test_compareDates_IfDate1GreaterDate2_ShouldReturn_1();
  printf("\n");

  test_compareDates_IfDate1EqualsDate2_ShouldReturn_0();
  printf("\n");

  test_compareDates_IfDate1LessDate2_ShouldReturn_Minus1();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);
}


/*//test_save//

*/
void test_save(){
  printf("TEST::%s\n", __func__);
  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0 = (Student *) malloc(sizeof(Student));
  Student *s1 = (Student *) malloc(sizeof(Student));
  Student *s2 = (Student *) malloc(sizeof(Student));

  initStudent(s0);
  initStudent(s1);
  initStudent(s2);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s0->lastname, "otto");
  setDate(&s0->birthday, 11, 11, 1998);
  setDate(&s0->start, 1, 10, 2021);
  setDate(&s0->end, 30, 9, 2024);

  strcpy(s1->matrikelNr, "test3");
  strcpy(s1->lastname, "mueller");
  setDate(&s1->birthday, 21, 8, 1997);
  setDate(&s1->start, 1, 10, 2021);
  setDate(&s1->end, 30, 9, 2024);
  
  strcpy(s2->matrikelNr, "test5");
  strcpy(s2->lastname, "mustermann");
  setDate(&s2->birthday, 10, 11, 2000);
  setDate(&s2->start, 1, 10, 2021);
  setDate(&s2->end, 30, 9, 2024);

  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

  save(list, TEST_SAVE_FILE);

  free(s0);
  free(s1);
  free(s2);
  free(list);

  printf("END_TEST::%s::success\n\n", __func__);
}


/*//test_readStudentFile_IfListIsNULL_ShouldReturn_Minus1//

*/
void test_readStudentFile_IfListIsNULL_ShouldReturn_Minus1(){
  printf("-->%s::", __func__);
  
  List *list = NULL;

  int resultValue = readStudentFile(list, TEST_SAVE_FILE);

  assert(resultValue == -1);

  printf("success");
  fflush(stdout);
}

/*//test_readStudentFile_IfFileNameIsNULL_ShouldReturn_Minus1//

*/
void test_readStudentFile_IfFileNameIsNULL_ShouldReturn_Minus1(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  char *fileName = NULL;

  int resultValue = readStudentFile(list, fileName);

  assert(resultValue == -1);
  assert(list->length == 0);
  assert(list->first_node == NULL);
  assert(list->last_node == NULL);

  printf("success");
  fflush(stdout);
}

/*//test_read_IfFileNotFound_ShouldReturn_0//

*/
void test_readStudentFile_IfFileNotFound_ShouldReturn_0(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  char *fileName = "fileDoesNotExist.csv";

  int resultValue = readStudentFile(list, fileName);

  assert(resultValue == 0);
  assert(list->length == 0);
  assert(list->first_node == NULL);
  assert(list->last_node == NULL);

  printf("success");
  fflush(stdout);
}

/*//test_read_IfSuccess_ShouldReturn_1//

*/
void test_readStudentFile_IfSuccess_ShouldReturn_1(){
  printf("-->%s::", __func__);

  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0;
  Student *s1;
  Student *s2;

  int resultValue = readStudentFile(list, TEST_SAVE_FILE);

  assert(resultValue == 1);
  assert(list->length == 3);
  assert(list->first_node != NULL);
  assert(list->last_node != NULL);

  s0 = list->first_node;
  s1 = s0->next_node;
  s2 = s1->next_node;

  assert(s0->isSaved == 1);
  assert(s1->isSaved == 1);
  assert(s2->isSaved == 1);

  free(s0);
  free(s1);
  free(s2);
  free(list);

  printf("success");
  fflush(stdout);
}


/*//test_readStudentFile//

*/
void test_readStudentFile(){
  printf("TEST::%s\n", __func__);

  test_readStudentFile_IfListIsNULL_ShouldReturn_Minus1();
  printf("\n");

  test_readStudentFile_IfFileNameIsNULL_ShouldReturn_Minus1();
  printf("\n");

  test_readStudentFile_IfFileNotFound_ShouldReturn_0();
  printf("\n");

  test_readStudentFile_IfSuccess_ShouldReturn_1();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);
}


/*//test_createStudent_IfMatrikelNrIsNULL_ShouldReturn_NULL//

*/
void test_createStudent_IfMatrikelNrIsNULL_ShouldReturn_NULL(){
  printf("-->%s::", __func__);

  char *matrikelNr = NULL;
  char *lastname = "mustermann";
  Date birthday = {18,11,1998};
  Date start = {1,10,2021};
  Date end = {30,9,2024};
  int isSaved = 0;

  Student *returnValue = createStudent(matrikelNr, lastname, birthday, start, end, isSaved);

  assert(returnValue == NULL);

  printf("success");
  fflush(stdout);
}

/*//test_createStudent_IfLastnameIsNULL_ShouldReturn_NULL//

*/
void test_createStudent_IfLastnameIsNULL_ShouldReturn_NULL(){
  printf("-->%s::", __func__);

  char *matrikelNr = "test2";
  char *lastname = NULL;
  Date birthday = {18,11,1998};
  Date start = {1,10,2021};
  Date end = {30,9,2024};
  int isSaved = 0;

  Student *returnValue = createStudent(matrikelNr, lastname, birthday, start, end, isSaved);

  assert(returnValue == NULL);

  printf("success");
  fflush(stdout);
}


/*//test_createStudent_IfStudentIsInvalid_ShouldReturn_NULL//

*/
void test_createStudent_IfStudentIsInvalid_ShouldReturn_NULL(){
  printf("-->%s::", __func__);

  char *matrikelNr = "test2";
  char *lastname = "mustermann";
  Date birthday = {45,11,1998};
  Date start = {0,10,2021};
  Date end = {31,9,2024};
  int isSaved = 0;

  Student *returnValue = createStudent(matrikelNr, lastname, birthday, start, end, isSaved);

  assert(returnValue == NULL);

  printf("success");
  fflush(stdout);
}

/*//test_createStudent_If_ShouldReturn_Pointer//

*/
void test_createStudent_IfStudentIsValid_ShouldReturn_Pointer(){
  printf("-->%s::", __func__);

  char *matrikelNr = "mustermann";
  char *lastname = "test2";
  Date birthday = {18,11,1998};
  Date start = {1,10,2021};
  Date end = {30,9,2024};
  int isSaved = 0;

  Student *returnValue = createStudent(matrikelNr, lastname, birthday, start, end, isSaved);

  assert(returnValue != NULL);
  assert(strcmp(returnValue->matrikelNr, matrikelNr) == 0);
  assert(strcmp(returnValue->lastname, lastname) == 0);
  assert(compareDates(&returnValue->birthday, &birthday) == 0);
  assert(compareDates(&returnValue->start, &start) == 0);
  assert(compareDates(&returnValue->end, &end) == 0);

  printf("success");
  fflush(stdout);
}

/*//test_createStudent//

*/
void test_createStudent(){
  printf("TEST::%s\n", __func__);

  test_createStudent_IfMatrikelNrIsNULL_ShouldReturn_NULL();
  printf("\n");

  test_createStudent_IfLastnameIsNULL_ShouldReturn_NULL();
  printf("\n");

  test_createStudent_IfStudentIsInvalid_ShouldReturn_NULL();
  printf("\n");

  test_createStudent_IfStudentIsValid_ShouldReturn_Pointer();
  printf("\n");

  printf("END_TEST::%s::success\n", __func__);
}

/*//runAllTests//

*/
void menuOperationRunTests(){
  isTestMode = 1;

  test_dateIsLeapYear();
  test_dateIsValid();
  test_setDate();
  test_compareDates();

  test_studentIsValid();
  test_createStudent();

  test_addStudent();
  test_getStudentByMatrikelNr();
  test_deleteStudent();
  test_printStudent();
  test_printAllStudents();
  test_printNumberStudents();
  test_save();
  test_readStudentFile();

  isTestMode = 0;

  fflush(stdin);

  printf("|\n");
  printf("|\n");
  printf("|Press ANY Key to Continue\n");
  getchar();
  printf("|\n");

  clearConsole();
}


void menuOperationAddStudent(){

  Student *newStudent = (Student *) malloc(sizeof(Student));
  initStudent(newStudent);

  char tryAgainChar[1];
  int isTryAgainCharValid = 0;
  int addResult = 0;
  
  printf("|\n");
  printf("|Operation::Add Student:\n");
  printf("|\n");
  printf("|Please Enter Values:\n");
  printf("|\n");
  printf("|'Matrikel-Nr': ");
  scanf(" %s", newStudent->matrikelNr);

  if(matrikelNrExists(StudentList, newStudent->matrikelNr) == 1){
    printf("|\n");
    printf("|Matrikel-Nr: '%s' Already Exists -->Operation Aborted\n", newStudent->matrikelNr);
    printf("|\n");

    fflush(stdin);

    printf("|\n");
    printf("|\n");
    printf("|Press ANY Key to Continue\n");
    getchar();
    printf("|\n");

    clearConsole();

    fflush(stdin);

    return;
  }

  printf("|'Lastname': ");
  scanf(" %s", newStudent->lastname);

  do{
    printf("|'Birthday (mm/dd/YYYY)':");
    scanf("%2d/%2d/%d", &newStudent->birthday.month, &newStudent->birthday.day, &newStudent->birthday.year);
    
    if(dateIsValid(&newStudent->birthday) == 1){
      break;
    }
    printf("|\n");
    
    do{
      fflush(stdin);
      printf("|Do You Want to Try Again? [Y/n]: ");
      scanf(" %c", tryAgainChar);
      printf("|\n");
      fflush(stdin);

      if(strcmp(tryAgainChar, "Y") == 0){
        isTryAgainCharValid = 1;
      }
      if(strcmp(tryAgainChar, "n") == 0){
        clearConsole();
        return;
      } 
    }while(isTryAgainCharValid != 1);
  
    if(isTryAgainCharValid == 0) break;
  }while(1);

  isTryAgainCharValid = 0;

  do{
    printf("|'Start of Studies' (mm/dd/YYYY)':");
    scanf("%2d/%2d/%d", &newStudent->start.month, &newStudent->start.day, &newStudent->start.year);
    
    if(dateIsValid(&newStudent->start) == 1){
      int cmpResultBirthdayStart = compareDates(&newStudent->birthday, &newStudent->start);
<<<<<<< HEAD
      
      if(cmpResultBirthdayStart == 1)  printf("|\n|Invalid Date --> 'Birthday' > 'Start of Studies'\n");
      if(cmpResultBirthdayStart == 0)  printf("|\n|Invalid Date --> 'Birthday' = 'Start of Studies'\n");
      if(cmpResultBirthdayStart == -1) break;
      if(cmpResultBirthdayStart == -2) printf("|\n|Internal Error --> Faild Date Validation\n");
=======
      if(cmpResultBirthdayStart < 0 && cmpResultBirthdayStart > -2){
        break;
      }
>>>>>>> parent of ff644a3 (update menuOperationAddStudent)
    }
    printf("|\n");

    do{
      fflush(stdin);
      printf("|Do You Want to Try Again? [Y/n]: ");
      scanf(" %c", tryAgainChar);
      printf("|\n");
      fflush(stdin);

      if(strcmp(tryAgainChar, "Y") == 0){
        isTryAgainCharValid = 1;
      }
      if(strcmp(tryAgainChar, "n") == 0){
        clearConsole();
        return;
      }
    }while(isTryAgainCharValid != 1);
  
    if(isTryAgainCharValid == 0) break;
  }while(1);

  isTryAgainCharValid = 0;

  do{
    printf("|'End of Studies (mm/dd/YYYY)':");
    scanf("%2d/%2d/%d", &newStudent->end.month, &newStudent->end.day, &newStudent->end.year);
    
    if(dateIsValid(&newStudent->end) == 1){
      int cmpResultStartEnd = compareDates(&newStudent->start, &newStudent->end);
<<<<<<< HEAD
      if(cmpResultStartEnd == 1)  printf("|\n|Invalid Date --> 'Start of Studies' > 'End of Studies'\n");
      if(cmpResultStartEnd == 0)  printf("|\n|Invalid Date --> 'Start of Studies' = 'End of Studies'\n");
      if(cmpResultStartEnd == -1) break;
      if(cmpResultStartEnd == -2) printf("|\n|Internal Error --> Faild Date Validation\n");
=======
      if(cmpResultStartEnd < 0 && cmpResultStartEnd > -2){ 
        break;
      }
>>>>>>> parent of ff644a3 (update menuOperationAddStudent)
    }
    printf("|\n");

    do{
      fflush(stdin);
      printf("|Do You Want to Try Again? [Y/n]: ");
      scanf(" %c", tryAgainChar);
      printf("|\n");
      fflush(stdin);

      if(strcmp(tryAgainChar, "Y") == 0){
        isTryAgainCharValid = 1;
      }
      if(strcmp(tryAgainChar, "n") == 0){
        clearConsole();
        return;
      } 
    }while(isTryAgainCharValid != 1);
  
    if(isTryAgainCharValid == 0) break;
  }while(1);
  
  
  addResult = addStudent(StudentList, newStudent);

  printf("|\n");
  printf("|\n");

  switch (addResult)
  {
  case 1: printf("|Student: '%s' --> ADDED\n", newStudent->matrikelNr); break;
  case 0: printf("|Student: '%s' --> Student Data NOT Valid\n", newStudent->matrikelNr); break;
  case -1: printf("|Internal ERROR Operation Aborted\n"); break;
  default: return;
  }
  
  fflush(stdin);

  printf("|\n");
  printf("|\n");
  printf("|Press ANY Key to Continue\n");
  getchar();
  printf("|\n");

  clearConsole();

  fflush(stdin);
}

/*//menuOperationDeleteStudent//

*/
void menuOperationDeleteStudent(){
  char confirmChar[1];
  char targetMatrikelNr[50];
  int isConfirmCharValid = 0;
  int deleteResult = 0;

  printf("|\n");
  printf("|Operation::Delete Student:\n");
  printf("|\n");
  printf("|Please enter the 'Matrikel-Nr': ");
  scanf(" %s", targetMatrikelNr);
  printf("|\n");
  fflush(stdin);
  
  do{
    
    printf("|Delete Student Permanently? [Y/n]: ");
    scanf(" %c",confirmChar);

    if(strcmp(confirmChar, "Y") == 0){
      isConfirmCharValid = 1;
    }
    if(strcmp(confirmChar, "n") == 0){
      clearConsole();
      return;
    } 

  }while(isConfirmCharValid != 1);

  printf("|\n");
  fflush(stdin);

  deleteResult = deleteStudent(StudentList, targetMatrikelNr);

  switch (deleteResult)
  {
    case 1: printf("|Student: '%s' --> DELETED\n", targetMatrikelNr); break;
    case 0: printf("|Student: '%s' --> NOT FOUND\n", targetMatrikelNr); break;
    case -1: printf("|Internal ERROR Deletion Aborted\n", targetMatrikelNr); break;
    default: break;
  }

  printf("|\n");
  printf("|\n");
  printf("|Press ANY Key to Continue\n");
  getchar();
  printf("|\n");

  clearConsole();

  fflush(stdin);
}

void menuOperationPrintStudent(){

  char targetMatrikelNr[50];
  int printResult = 0;

  printf("|\n");
  printf("|Operation::Print Student:\n");
  printf("|\n");
  printf("|Please enter the 'Matrikel-Nr': ");
  scanf("%s", targetMatrikelNr);
  printf("|\n");
  fflush(stdin);

  printResult = printStudentByMatrikelNr(StudentList, targetMatrikelNr);
 
  switch (printResult)
  {
    case 0: printf("|Student: '%s' --> Can't be Printed\n", targetMatrikelNr); break;
    case -1: printf("|Internal ERROR Operation Aborted\n", targetMatrikelNr); break;
    case -2: printf("|Student: '%s' --> NOT FOUND\n", targetMatrikelNr); break;
    default: break;
  }

  printf("|\n");
  printf("|\n");
  printf("|Press ANY Key to Continue\n");
  getchar();
  printf("|\n");

  fflush(stdin);
}

void menuOperationPrintAllStudents(){
  printf("|\n");
  printf("|Operation::Print All Students:\n");
  printf("|\n");

  int count = printAllStudents(StudentList);

  if(count == 0)
    printf("|No Students to print.\n");

  printf("|\n");
  printf("|\n");
  printf("|Press ANY Key to Continue\n");
  getchar();
  printf("|\n");

  clearConsole();

  fflush(stdin);
}

void menuOperationPrintNumberStudents(){
  printf("|\n");
  printf("|Operation::Print Number Students:\n");
  printf("|\n");

  printNumberStudents(StudentList);

  printf("|\n");
  printf("|\n");
  printf("|Press ANY Key to Continue\n");
  getchar();
  printf("|\n");

  clearConsole();

  fflush(stdin);
}

void menuOperationClearConsole(){
  clearConsole();
}

void printMenu(){
  printf("+-------------------------------------+\n");
  printf("|menu\n");
  printf("+-------------------------------------+\n");
  printf("|   (1)   add student\n");
  printf("|   (2)   delete student\n");
  printf("|\n");
  printf("|   (3)   print student\n");
  printf("|   (4)   print student::all\n");
  printf("|   (5)   print number students\n");
  printf("|\n");
  printf("|   (c)   clear console\n");
  printf("|   (t)   run tests::all\n");
  printf("|   (x)   exit\n");
  printf("+-------------------------------------+\n");
  printf("|choose operation:: ");

}


/*//menu//

*/
void menu(){

  char mode;
  int isRunning = 1;

  do{
    
    printMenu();

    scanf(" %c", &mode);
    fflush(stdin);

    switch (mode)
    {
    case '1': menuOperationAddStudent(); break;
    case '2': menuOperationDeleteStudent(); break;

    case '3': menuOperationPrintStudent(); break;
    case '4': menuOperationPrintAllStudents(); break;
    case '5': menuOperationPrintNumberStudents(); break;
    
    case 'c': menuOperationClearConsole(); break;
    case 't': menuOperationRunTests(); break;
    case 'x': isRunning = 0; break;

    default: 
      printf("|\n");
      printf("|Invalid Menu-Operation::Press ANY Key to Continue\n"); 
      getchar();
      break;

    }

  }while(isRunning);
}


void exitHandler_saveOnExit(void){
  save(StudentList, SAVE_FILE);

  fflush(stdin);

    printf("|\n");
    printf("|\n");
    printf("|Press ANY Key to Exit\n");
    getchar();
    printf("|\n");

    clearConsole();
}



void abortHandler(){
  fflush(stdin);

    printf("|\n");
    printf("|\n");
    printf("|Press ANY Key to Continue\n");
    getchar();
    printf("|\n");
}

int main(){
  signal(SIGABRT, &abortHandler);

  StudentList = (List *) malloc(sizeof(List));
  initList(StudentList);

  readStudentFile(StudentList, SAVE_FILE);

  atexit(exitHandler_saveOnExit);

  menu();

  return 0;
}