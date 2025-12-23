// #include <stdio.h>
// #include <sqlite3.h>
// #include <stdlib.h>
// #include <string.h>



//     int rc;
//     char *err_msg = 0;
//     sqlite3 *db;

    

//     // // Activer les clés étrangères
//     // sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);

//     // Création de la table grades
//     const char *sql =
//         "CREATE TABLE IF NOT EXISTS grade ("
//         "grade_id INTEGER PRIMARY KEY AUTOINCREMENT,"
//         "student_id INTEGER NOT NULL,"
//         "course_id text NOT NULL,"
//         "semester TEXT NOT NULL,"
//         "CC1 REAL NOT NULL,"
//         "CC2 REAL NOT NULL,"
//         "NOTE REAL NOT NULL,"
//         "FOREIGN KEY (student_id) REFERENCES student(student_id),"
//         "FOREIGN KEY (course_id) REFERENCES courses(course_id)"
//         ");";

        
//     int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
//     if (rc != SQLITE_OK) {
//         printf("Erreur SQL (create table): %s\n", err_msg);
//         sqlite3_free(err_msg);
//         sqlite3_close(db);
//         return 1;
//     }

//     printf("Table grades créée avec succès.\n");

//     // 🔹 Forcer le début de l'AUTOINCREMENT à 1000
//     // (à exécuter UNE SEULE FOIS)
//     const char *sql_seq =
//         "INSERT OR IGNORE INTO sqlite_sequence (name, seq) "
//         "VALUES ('grade', 999);";

//     rc = sqlite3_exec(db, sql_seq, 0, 0, &err_msg);
//     if (rc != SQLITE_OK) {
//         printf("Erreur sqlite_sequence: %s\n", err_msg);
//         sqlite3_free(err_msg);
//     } 

//     sqlite3_close(db);
   
