#ifndef GRADE_H
#define GRADE_H

#include <sqlite3.h>
#include "database.h"

/*
 * Vérifie si l'étudiant existe et s'il est inscrit au cours
 * Retour :
 * 1 → existe
 * 0 → n'existe pas
 */
int verifier_etud_existe(sqlite3 *db,int student_id,char *course_id,char *semester);

/*
 * Insère une ligne vide dans la table grade
 */
int insert_into_grade_table(sqlite3 *db,int student_id,char *course_id,char *semester);

/*
 * Ajouter une note (projet + examen)
 */
int addGrade(sqlite3 *db,int student_id,char *course_id,char *semester,float project,float exam);

/*
 * Mettre à jour une note existante
 */
int updateGrade(sqlite3 *db,int student_id,char *course_id,char *semester,float project,float exam);

/*
 * Affiche les notes d’un étudiant pour un major et un semestre
 */
int affichage_grade(sqlite3 *db,Major_List *major_list,int student_id,char *major_id,char *semester);

#endif
