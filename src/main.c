
#include <stdlib.h>


typedef struct Date{
  int day;
  int month;
  int year;
}Date;

typedef struct Student{
  char *lastname;
  char *matrikelNr;

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


void addStudent(List *list, Student *node){

  if(list->first_node == NULL){
    list->first_node = node;
    list->last_node = node;
    list->length++;
    return;
  }


  Student *tempNode = &(*list->last_node);
  tempNode->next_node = node;
  list->last_node = node;
  node->prev_node = tempNode;

  list->length++;
}



int main(){

}