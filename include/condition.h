#ifndef CONDITION_H
#define CONDITION_H

typedef struct cond {
    char cond_type[20];
    char ref_id[30];
    struct cond *suivant;
} cond;

extern cond *Debut;
extern cond *Fin;
extern int nbre_condition;

void initialisation();
void ajouter_tete1(char *cond_type, char *ref_id);
void liberer_cond();

#endif
