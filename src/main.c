
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

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

  TESTS: 
    test_getStudentByMatrikelNr_ExpectMatchingStudentFound
    test_getStudentByMatrikelNr_ExpectMatchingStudentNotFound
*/
Student *getStudentByMatrikelNr(List *list, char *matrikelNr){
/*Case1: matrikelNr not found*/
  /*Case2: list == NULL*/

  Student *currentStudent = list->first_node;
  int cmpResult;
  int hasNext = 0;

  do{
    cmpResult = strcmp(currentStudent->matrikelNr, matrikelNr);
  
    if(cmpResult == 0) {
      return currentStudent;
    }

    if(currentStudent->next_node == NULL){
      hasNext = 0;
    }else{
      hasNext = 1;
    }

    currentStudent = currentStudent->next_node;

  }while(hasNext);

  return NULL;
}

/*//addStudent//
  inserts 'node' at the alphabetically destined position

  TESTS:
    test_addStudent
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

  TESTS: 
    NONE
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

  TESTS:
    test_inputStudent
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
void printStudent(char *matrikelNr){

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

/*//test_addStudent//

*/
void test_addStudent(){
  List *list;
  initList(list);

  Student *s0, *s1, *s2, *s3;

  initStudent(s0);
  strcpy(s0->matrikelNr, "test2");

  initStudent(s1);
  strcpy(s1->matrikelNr, "test5");

  initStudent(s2);
  strcpy(s2->matrikelNr, "test3");

  initStudent(s3);
  strcpy(s3->matrikelNr, "test6");

  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

  free(s0);
  free(s1);
  free(s2);
  free(s3);
  free(list);
}

/*//test_addStudent_isLastNode//

*/
void test_addStudent_isLastNode(){
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
}


/*//test_addStudent_isNotFirstNode_isNotLastNode//

*/
void test_addStudent_isNotFirstNode_isNotLastNode(){
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

  assert(s0->next_node == s1);
  assert(s0->prev_node == NULL);

  assert(s1->next_node == NULL);
  assert(s1->prev_node == s1);

  assert(s2->next_node == s1);
  assert(s2->prev_node == s0);
}


/*//test_addStudent_isFirstNode//

*/
void test_addStudent_isFirstNode(){

  test_addStudent_isFirstNode();
  test_addStudent_isLastNode();
  test_addStudent_isNotFirstNode_isNotLastNode();
  
}



/*//test_deleteStudent//

*/
void test_deleteStudent(){
  
  test_deleteStudent_isFirstNode();
  test_deleteStudent_isLastNode();
  test_deleteStudent_isNotFirst_isNotLast();
}

/*//test_deleteStudent_isFirstNode//

*/
void test_deleteStudent_isFirstNode(){
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

}


/*//test_deleteStudent_isLastNode//

*/
void test_deleteStudent_isLastNode(){
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
}


/*//test_deleteStudent_isNotFirst_isNotLast//

*/
void test_deleteStudent_isNotFirst_isNotLast(){
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
}

void test_getStudentByMatrikelNr_ExpectMatchingStudentFound(){
  List *list = (List *) malloc(sizeof(List));
  initList(list);

  Student *s0;
  Student *s1;

  initStudent(s0);
  initStudent(s1);

  strcpy(s0->matrikelNr, "test2");
  strcpy(s1->matrikelNr, "test5");

  char *targetMatrikelNr = "test5";
  Student *node = getStudentByMatrikelNr(list, targetMatrikelNr);

  assert(node != NULL);
  assert(node == s1);
}

void test_getStudentByMatrikelNr_ExpectMatchingStudentNotFound(List *list){
  char *matrikelNr = "";
  Student *st = getStudentByMatrikelNr(list, matrikelNr);

  assert(st == NULL);
}


void test_getStudentByMatrikelNr(){
  List *list = (List *) malloc(sizeof(List));

  test_getStudentByMatrikelNr_ExpectMatchingStudentFound();

  test_getStudentByMatrikelNr_ExpectMatchingStudentNotFound(list);
 
}



int main(){
 
  return 0;
}