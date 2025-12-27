#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include <sqlite3.h>
#include "database.h"


//extraire année academique precedent

void previousAcademicYear(char *current, char *previous);



/*
 * Vérifie si un étudiant interne valide les prérequis d’un major
 * Retourne :
 * 1 → prérequis validés
 * 0 → prérequis non validés
 */
int vérifierPre(sqlite3 *db,Major_List *major_list,int student_id,char *major_id,char *previous_academic_year);

/*
 * Inscrit un étudiant dans un major pour un semestre donné
 * Retourne :
 * 0 → succès
 * 1 → échec
 */
int enrollStudent(sqlite3 *db,Major_List *major_list,int student_id,char *major_id);

/*
 * Désinscrit un étudiant d’un major
 */
int unenrollStduent(sqlite3 *db, int student_id, char *major_id,char *academic_year);


int StudentExistsInDB(sqlite3 *db, int student_id);



 
#endif
