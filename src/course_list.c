#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "course_list.h"

course *Debut_course = NULL;
course *Fin_course = NULL;
int nbre_course = 0;

void initialisation_course(){
    Debut_course = NULL;
    Fin_course = NULL;
    nbre_course =0;
}

void ajouter_tete(char *course_id,float grade){
    course *element = (course*)malloc(sizeof(course));

    strcpy(element->course_id,course_id);
    element->grade = grade;
    element->suivant = Debut_course;
    if (Debut_course == NULL)
    {
        Debut_course = element;
        Fin_course = element;
    }else
    {
        Debut_course = element;
    }
    nbre_course++;    
}

void affichage(){
    course *element = Debut_course;
    if (element==NULL)
    {
        return;
    }
    
    float Note_total = 0,Note_Major;
    printf("---------------------------\n");
    printf("| %15s | %5s |\n","Course_id", "Grade");
    printf("---------------------------\n");
    while (element!=NULL)
    {
        printf("| %15s | %5f |\n",element->course_id, element->grade);
        Note_total += element->grade;
        element = element->suivant;
    }
    printf("---------------------------\n");
    Note_Major = Note_total/nbre_course;
    printf("| %15s | %5f |\n","Note General", Note_Major);
   
}

void liberer_course() {
    course *tmp;
    while (Debut_course != NULL) {
        tmp = Debut_course;
        Debut_course = Debut_course->suivant;
        free(tmp);
    }
    Fin_course = NULL;
    nbre_course = 0;
}

