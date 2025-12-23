#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include <sqlite3.h>
#include "database.h"

/*
 * Vérifie si un étudiant interne valide les prérequis d’un major
 * Retourne :
 * 1 → prérequis validés
 * 0 → prérequis non validés
 */
int vérifierPre(sqlite3 *db,Major_List *major_list,int student_id,char *major_id,char *semester);

/*
 * Inscrit un étudiant dans un major pour un semestre donné
 * Retourne :
 * 0 → succès
 * 1 → échec
 */
int enrollStudent(sqlite3 *db,Major_List *major_list,int student_id,char *major_id,char *semester);

/*
 * Désinscrit un étudiant d’un major
 */
int unenrollStduent(sqlite3 *db, int student_id, char *major_id);

int StudentExistsInDB(sqlite3 *db, int student_id);


 
#endif
