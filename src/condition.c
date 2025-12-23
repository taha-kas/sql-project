#include "condition.h"
#include <stdlib.h>
#include <string.h>

cond *Debut,*Fin;
int nbre_condition;

void initialisation(){
    Debut = NULL;
    nbre_condition=0;
}

void ajouter_tete1(char *cond_type,char *ref_id){
    cond *element=(cond*)malloc(sizeof(cond));

    strcpy(element->cond_type,cond_type);
    strcpy(element->ref_id , ref_id);
    element->suivant = Debut;
    if (Debut==NULL)
    {
        Debut= element;
        Fin = element;
    }else
    {
        Debut = element;
    }
    nbre_condition++;
}

void liberer_cond() {
    cond *tmp;
    while (Debut != NULL) {
        tmp = Debut;
        Debut = Debut->suivant;
        free(tmp);
    }
    Fin = NULL;
    nbre_condition = 0;
}