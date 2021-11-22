
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


/*//addStudent//
  inserts 'node' at the alphabetically destined position

  TESTS:
    test_addStudent
*/
void addStudent(List *list, Student *node){

  Student *currentNode = list->first_node;
  Student *prevNode;
  Student *nextNode;

  int currentCmpResult = 0;
  int prevCmpResult = 0;

  if(list->first_node == NULL){
      list->first_node = node;
      list->last_node = node;
      list->length++;
      return;
  }

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
void deleteStudent(List *list, char *matrikelNr){
  Student *node = getStudentByMatrikelNr(list, matrikelNr);
  Student *prevNode;
  Student *nextNode;

  if(node == NULL){
    return;
  }

  prevNode = node->prev_node;
  nextNode = node->next_node;

  prevNode->next_node = nextNode;
  nextNode->prev_node = prevNode;

  list->length--;

  free(node);
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
void test_addStudent(List *list){
  

  Student *s0 = (Student *) malloc(sizeof(Student));
  strcpy(s0->matrikelNr, "test2");
  s0->prev_node = NULL;
  s0->next_node = NULL;

  Student *s1 = (Student *) malloc(sizeof(Student));
  strcpy(s1->matrikelNr, "test5");
  s1->prev_node = NULL;
  s1->next_node = NULL;

  Student *s2 = (Student *) malloc(sizeof(Student));
  strcpy(s2->matrikelNr, "test3");
  s2->prev_node = NULL;
  s2->next_node = NULL;

  Student *s3 = (Student *) malloc(sizeof(Student));
  strcpy(s3->matrikelNr, "test6");
  s3->prev_node = NULL;
  s3->next_node = NULL;


  addStudent(list, s0);
  addStudent(list, s1);
  addStudent(list, s2);

}

/*//test_deleteStudent//

*/
void test_deleteStudent(){
 
}

void test_getStudentByMatrikelNr_ExpectMatchingStudentFound(List *list){
  char *matrikelNr = "test5";
  Student *st = getStudentByMatrikelNr(list, matrikelNr);

  assert(st != NULL);
  assert(st->matrikelNr == matrikelNr);
}

void test_getStudentByMatrikelNr_ExpectMatchingStudentNotFound(List *list){
  char *matrikelNr = "";
  Student *st = getStudentByMatrikelNr(list, matrikelNr);

  assert(st == NULL);
}


void test_getStudentByMatrikelNr(){

  List *list;
  list = (List *) malloc(sizeof(List));
  //TODO: catch out list == NULL 

  test_addStudent(list);

  test_getStudentByMatrikelNr_ExpectMatchingStudentFound(list);

  test_getStudentByMatrikelNr_ExpectMatchingStudentNotFound(list);
 
}



int main(){
  List *list = (List *) malloc(sizeof(List));
  list->first_node = NULL;
  list->last_node = NULL;
  list->length = 0;
  
  test_getStudentByMatrikelNr();
 
  return 0;
}