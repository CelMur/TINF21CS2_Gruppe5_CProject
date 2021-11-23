
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

const int MIN_YEAR = 1900;
const int MAX_YEAR = 2100;

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

  struct Student *next_node;
  struct Student *prev_node;
}Student;


typedef struct List{
  struct Student *first_node;
  struct Student *last_node;

  int length;
}List;

/*//initList//
  reserves memory and sets pointer to NULL
*/
void initList(List *list){
  list->first_node = NULL;
  list->last_node= NULL;
  list->length = 0;
}


/*//initStudent//
  reserves memory and sets pointer to NULL
*/
int initStudent(Student *student){
  student->next_node = NULL;
  student->prev_node = NULL;
}


/*//DateIsLeapYear//
  return 1 if the date is valid
  returns 0 if the date is invalid

  reference for leapyear-calculation:
    https://docs.microsoft.com/de-de/office/troubleshoot/excel/determine-a-leap-year 11/23/2021 09:27pm
*/
int DateIsLeapYear(int year){
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


/*//DateIsValid//
  return 1 if the date is valid
  returns 0 if the date is invalid
*/
int DateIsValid(int day, int month, int year){

  if(year < MIN_YEAR || year > MAX_YEAR) return 0;   
  if(month < 1 || month > 12) return 0;    
  if(day < 1 || day > 31) return 0;    

  if(month == 2){
    if(DateIsLeapYear(year)){
      if(day <= 29){
        return 1;
      }
      return 0;
    }
    if(day <= 28){
      return 1;
    }
    return 0;
  }

  if(month == 4 || month == 6 || month == 9 || month == 11){
    if(day > 30){
      return 0;
    }
  }
  return 1;
}



/*//setDate//
  returns 1 if successful
  returns 0 if one or more values invalid
  return -1 if date == NULL
*/
int setDate(Date *date, int day, int month, int year){

  if(date == NULL) return -1;
  if(DateIsValid(day, month, year) == 0) return 0;

  date->day = day;
  date->month = month;
  date->year = year;

  return 1;
}

/*//compareNodes//
    returns the matrikelNr of 'node1' with 'node2'

  'node1'  > 'node2' = 1
  'node1'  < 'node2' = -1
  'node1' == 'node2' = 0

  'node1' == NULL = 1
  'node2' == NULL = 1
*/
int compareNodes(Student *node1, Student *node2){
  if (node1 == NULL) return 1;
  if (node2 == NULL) return 1;

  int cmp = strcmp(node1->matrikelNr, node2->matrikelNr);

  return cmp;
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

*/
void addStudent(List *list, Student *node){

  if(list->first_node == NULL){
      list->first_node = node;
      list->last_node = node;
      list->length++;
      return;
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
}

/*//deleteStudent//
  removes a Student from 'list' by comparing its 'matrikelNr'

*/
int deleteStudent(List *list, char *matrikelNr){
  Student *node = getStudentByMatrikelNr(list, matrikelNr);
  Student *prevNode = NULL;
  Student *nextNode = NULL;

  if(node == NULL){
    return 0;
  }

  if(list->length == 1){

    list->first_node == NULL;
    list->last_node == NULL;


  }else if(list->first_node == node){

    nextNode = node->next_node;
    nextNode->prev_node = NULL;
    list->first_node = nextNode;
    

  }else if(list->last_node == node){

    prevNode = node->prev_node;
    prevNode->next_node = NULL;
    list->last_node = prevNode;


  }else{
    prevNode = node->prev_node;
    nextNode = node->next_node;

    prevNode->next_node = nextNode;
    nextNode->prev_node = prevNode;
  }

  list->length--;

  free(node);
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


/*//printStudent//

*/
int printStudent(List *list, char *matrikelNr){

  if(list == NULL) return -1;
  Student *node = getStudentByMatrikelNr(list, matrikelNr);

  if(node == NULL){
    return -2;
  }

  //TODO:validateStudent

  printf("'Student'=\n");
  printf("  {\n");
  printf("    'matrikelNr' = '%s',\n", node->matrikelNr);
  printf("    'lastname' = '%s',\n", node->lastname);
  printf("    'birthday' = '%02i/%02i/%i',\n", node->birthday.month, node->birthday.day, node->birthday.year);
  printf("    'studyStart' = '%02i/%02i/%i',\n", node->start.month, node->start.day, node->start.year);
  printf("    'studyEnd' = '%02i/%02i/%i',\n", node->end.month, node->end.day, node->end.year);
  printf("  }");

  return 1;
}


/*//printAllStudents//

*/
void printAllStudents(char *matrikelNr){

}

/*//menue//

*/
void menue(){

}

/*//save//

*/
void save(List *list){

}


/*//read//

*/
void read(List *List){

}


/*//test_inputStudent//

*/
void test_inputStudent(){
  Student s0 = inputStudent();
  
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
  strcpy(s1->matrikelNr, "test3");
  strcpy(s2->matrikelNr, "test5");

  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

  assert(list->first_node == s0);
  assert(list->last_node == s2);
  assert(list->length == 3);

  assert(s0->next_node == s1);
  assert(s0->prev_node == NULL);

  assert(s1->next_node == s2);
  assert(s1->prev_node == s0);

  assert(s2->next_node == NULL);
  assert(s2->prev_node == s1);

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


/*//test_printStudent_IfListIsNULL_ShouldReturn_Minus1//

*/
void test_printStudent_IfListIsNULL_ShouldReturn_Minus1(){
  printf("-->%s::", __func__);

  printf("not implemented");
  fflush(stdout);
}

/*//test_printStudent_IfStudentIsNULL_ShouldReturn_Minus2//

*/
void test_printStudent_IfStudentIsNULL_ShouldReturn_Minus2(){
  printf("-->%s::", __func__);

  printf("not implemented");
  fflush(stdout);
}

/*//test_printStudent_IfDateIsInvalid_ShouldReturn_0//

*/
void test_printStudent_IfDateIsInvalid_ShouldReturn_0(){
  printf("-->%s::", __func__);

  printf("not implemented");
  fflush(stdout);
}

/*//test_printStudent_IfStudentExists_ShouldReturn_1//

*/
void test_printStudent_IfStudentExists_ShouldReturn_1(){
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
  setDate(&s1->start, 1, 10, 2000);
  setDate(&s1->end, 30, 9, 2000);

  addStudent(list, s0);
  addStudent(list, s1);

  char *targetMatrikelNr = "test5";

  node = getStudentByMatrikelNr(list, targetMatrikelNr);

  printSuccessfull = printStudent(list, targetMatrikelNr);

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

  test_printStudent_IfStudentExists_ShouldReturn_1();
  printf("\n");

  test_printStudent_IfDateIsInvalid_ShouldReturn_0();
  printf("\n");

  test_printStudent_IfListIsNULL_ShouldReturn_Minus1();
  printf("\n");

  test_printStudent_IfStudentIsNULL_ShouldReturn_Minus2();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);
}

void test_printAllStudent(){

}


/*//test_DateIsLeapYear_IfIsLeapYear_ShouldReturn_1

*/
void test_DateIsLeapYear_IfIsLeapYear_ShouldReturn_1(){
  printf("-->%s::", __func__);

  int years[16] = {1904, 1920, 1984, 1992, 1996, 2000, 2004, 2012, 2016, 2020, 2024, 2028, 2032, 2036, 2040, 2044};
  int yearsLen = sizeof(years) / sizeof(int);
  int isLeapYear;

  for(int i = 0; i< yearsLen; i++){

    isLeapYear = DateIsLeapYear(years[i]);
    assert(isLeapYear == 1);
  }
  
  printf("success");
  fflush(stdout);
}


/*//test_DateIsLeapYear_IfIsNotLeapYear_ShouldReturn_0

*/
void test_DateIsLeapYear_IfIsNotLeapYear_ShouldReturn_0(){
  printf("-->%s::", __func__);

  int years[16] = {1909, 1910, 1950, 1966, 1997, 1999, 2010, 2021, 2022, 2023, 2025, 2026, 2027, 2029, 2043, 2045};
  int yearsLen = sizeof(years) / sizeof(int);
  int isLeapYear;

  for(int i = 0; i< yearsLen; i++){

    isLeapYear = DateIsLeapYear(years[i]);
    assert(isLeapYear == 0);
  }

  printf("success");
  fflush(stdout);
}

/*//test_DateIsLeapYear_IfYearIsNegative_ShouldReturn_0

*/
void test_DateIsLeapYear_IfYearIsNegative_ShouldReturn_0(){
  printf("-->%s::", __func__);

  int year = -1904;
  int isLeapYear = DateIsLeapYear(year);

  assert(isLeapYear == 0);

  printf("success");
  fflush(stdout);
}

/*//test_DateIsLeapYear

*/
void test_DateIsLeapYear(){
  printf("TEST::%s\n", __func__);

  
  test_DateIsLeapYear_IfIsNotLeapYear_ShouldReturn_0();
  printf("\n");

  test_DateIsLeapYear_IfIsLeapYear_ShouldReturn_1();
  printf("\n");

  test_DateIsLeapYear_IfYearIsNegative_ShouldReturn_0();
  printf("\n");

  printf("END_TEST::%s::success\n\n", __func__);  
}


/*//test_DateIsValid_IfDayNotInRange_ShouldReturn_0//

*/
void test_DateIsValid_IfDayNotInRange_ShouldReturn_0(){
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

    isValid = DateIsValid(dates[i].day, dates[i].month, dates[i].year);
    assert(isValid == 0);
  }

  printf("success");
  fflush(stdout);
}

/*//test_DateIsValid_IfDayNotInRange_ShouldReturn_0//

*/
void test_DateIsValid_IfMonthNotInRange_ShouldReturn_0(){
   printf("-->%s::", __func__);
  
  
  Date dates[2] = {
    {1,0,2021},
    {1,13,2021}
  };

  int datesLen = sizeof(dates) / sizeof(Date);
  int isValid;

  for(int i = 0; i< datesLen; i++){

    isValid = DateIsValid(dates[i].day, dates[i].month, dates[i].year);
    assert(isValid == 0);
  }

  printf("success");
  fflush(stdout);
}

/*//test_DateIsValid_IfDayNotInRange_ShouldReturn_0//

*/
void test_DateIsValid_IfYearNotInRange_ShouldReturn_0(){
   printf("-->%s::", __func__);
  
  
  Date dates[2] = {
    {1,0, MIN_YEAR - 1},
    {1,13,MAX_YEAR + 1}
  };

  int datesLen = sizeof(dates) / sizeof(Date);
  int isValid;

  for(int i = 0; i< datesLen; i++){

    isValid = DateIsValid(dates[i].day, dates[i].month, dates[i].year);
    assert(isValid == 0);
  }

  printf("success");
  fflush(stdout);
}


/*//test_DateIsValid//

*/
void test_DateIsValid(){
  printf("TEST::%s\n", __func__);

  test_DateIsValid_IfDayNotInRange_ShouldReturn_0();
  printf("\n");

  test_DateIsValid_IfMonthNotInRange_ShouldReturn_0();
  printf("\n");

  test_DateIsValid_IfYearNotInRange_ShouldReturn_0();
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

/*//test_all//

*/
void test_all(){
  test_DateIsLeapYear();
  test_DateIsValid();
  test_setDate();

  test_addStudent();
  test_getStudentByMatrikelNr();
  test_deleteStudent();
  test_printStudent();
}

int main(){
 
  test_all();

  return 0;
}