
#include <stdlib.h>
#include <string.h>


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

/*

*/
int compareNodes(Student *node1, Student *node2){
  if (node1 == NULL) return 1;
  if (node2 == NULL) return 1;

  int cmp = strcmp(node1->lastname, node2->lastname);

  return cmp;
}


/*

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


Student *getStudentByMatrikelNr(List *list, char *matrikelNr){
  
}


void test_addStudent(){
  List list;
  list.first_node = NULL;
  list.last_node = NULL;
  list.length = 0;

  Student * s0 = (Student*) malloc(sizeof(Student));
  strcpy(s0->lastname, "testperson1");
  s0->prev_node = NULL;
  s0->next_node = NULL;

  Student * s1 = (Student*) malloc(sizeof(Student));
  strcpy(s1->lastname, "testperson5");
  s1->prev_node = NULL;
  s1->next_node = NULL;

  Student * s2 = (Student*) malloc(sizeof(Student));
  strcpy(s2->lastname, "testperson3");
  s2->prev_node = NULL;
  s2->next_node = NULL;

  Student * s3 = (Student*) malloc(sizeof(Student));
  strcpy(s3->lastname, "testperson6");
  s3->prev_node = NULL;
  s3->next_node = NULL;


  addStudent(&list, s0);
  addStudent(&list, s1);
  addStudent(&list, s2);
  addStudent(&list, s3);
}

int main(){

  
 
  return 0;
}