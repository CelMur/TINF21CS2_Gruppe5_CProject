
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
*/
void deleteStudent(char *matrikelNr){

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


/*//getStudentByMatrikelNr//
  returns a Student from 'list' by comparing 'matrikelNr'

*/
Student *getStudentByMatrikelNr(List *list, char *matrikelNr){

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

}

/*//test_deleteStudent//

*/
void test_deleteStudent(){

}


int main(){

  
  test_inputStudent();
  

 
  return 0;
}