Tous ces exercices sont à essayer sur PlaFRIM, en utilisant Slurm pour l'allocation des ressources, selon les méthodes utilisées dans les précedentes exercices (feuille `exercices_1_slurm.md`). Une réservation interactives des ressources avec la commande `salloc` peut être convenable.

Les fichiers pour les exercices de cette séance sont dans le dossier `codes_2a_debug/`. Les programmes _gprof_ et _Valgrind_ sont déjà disponibles dans l'environnement de travail de PlaFRIM.


## A. Déverminage
Rappel:
- le déverminage a besoin d'un programme compilé avec les symboles de debug (option `-g`), et sans optimisation (`-O0`) pour retrouver la ligne source exacte du problème. Par exemple:
   ```
   gcc -O0 -g -std=c99 bug1.c -o program1
   ```
- `gdb` est un outil interactif, utilisez-le avec une allocation interactive des ressources (`salloc`).

1. Utilisez `gdb` pour corriger le programme `bug1.c` (ajoutez l'option de compilation `-std=c99`).
2. Utilisez `valgrind` pour corriger le programme `bug2.c` (ajoutez l'option de compilation `-std=c99`).
3. Utilisez `gdb` et `valgrind` pour corriger le programme `bug3.c` (ajoutez l'option de compilation `-std=c99`).
    - Les deux outils trouvent la position de l'erreur, mais quel outil est le plus utile pour trouver sa cause?
    - Quelle fonction on peut utiliser, dans un programme C compilé avec les options de debug, pour contrôler la valeur d'une variable ou d'une expression?
    - Des pointeurs sont passés comme arguments des fonctions. Pourquoi? En aurait-il besoin en Fortran? Et en C++?


### Références
- https://modules.readthedocs.io/en/latest/index.html
- https://docs.open-mpi.org/en/main/running-apps/slurm.html
- https://slurm.schedmd.com/documentation.html
- https://www.sourceware.org/gdb/documentation/
- https://valgrind.org/docs/manual/quick-start.html
- https://prace-ri.eu/
- https://www.genci.fr/fr
- https://www.plafrim.fr/
- https://plafrim-users.gitlabpages.inria.fr/doc/
- https://www.top500.org/

### Remerciements
François Rué et Florent Pruvost (INRIA) pour la version initiale des diapositives.
