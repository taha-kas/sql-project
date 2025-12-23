#ifndef COURSE_LIST_H
#define COURSE_LIST_H

typedef struct course {
    char course_id[25];
    float grade;
    struct course *suivant;
} course;

/* Variables globales */
extern course *Debut_course;
extern course *Fin_course;
extern int nbre_course;

/* Fonctions */
void initialisation_course();
void ajouter_tete(char *course_id, float grade);
void affichage();
void liberer_course();

#endif
