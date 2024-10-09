Tous ces exercices sont à essayer sur PlaFRIM, en utilisant Slurm pour l'allocation des ressources, selon les méthodes utilisées dans les précedentes exercices (feuille `exercices_slurm.md`). Une réservation interactives des ressources avec la commande `salloc` peut être convenable.

Les fichiers pour les exercices de cette séance sont dans le dossier `optim/`. Les programmes _gprof_ et _Valgrind_ sont déjà disponibles dans l'environnement de travail de PlaFRIM.


## A. Profilage d'un logiciel de calcul professionnel

_Mmg_  est un logiciel libre qui permet d'adapter des maillages, principalement utilisé pour adapter un maillage de calcul à la solution de la simulation numérique ou à des interfaces dans le domain de calcul.

1. Téléchangez le dépôt Git:
    ```
    git clone https://github.com/MmgTools/mmg.git
    ```

2. Compilez le programme en version optimisée avec informations de debug et profilage. Utilisez _CMake_ pour cela:
    ```
    mkdir mmg/build && cd mmg/build
    module load build/cmake/3.15.3
    cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXE_LINKER_FLAGS=-pg -DCMAKE_C_FLAGS=-pg -DUSE_VTK=OFF
    cmake --build . -j 8 -v
    ```
    La variable de CMake `CMAKE_BUILD_TYPE` s'occupe de selectionner un type de _release_ qui passe au compilateur l'option de debug `-g`, les variables `CMAKE_EXE_LINKER_FLAGS` et `CMAKE_C_FLAGS` imposent l'option de profilage `-pg`.

3. Téléchargez un maillage pour essayer le programme:
    ```
    cd .. && mkdir mytest && cd mytest
    wget https://gitlab.inria.fr/ParMmg/testparmmg/-/raw/master/A319_gmsh/A319_in_a_box.mesh
    ```

4. Profilez le programme lancé sur ce dernier maillage. Évaluez d'abord le temps de calcul avec la commande `time`:
    ```
    time ../build/bin/mmg3d_O3d A319_in_a_box.mesh -hmin 6 -hausd 5 -hgrad 1.7
    ```
    Puisqu'on a compilé le programme avec l'option de profilage `-pg`, un fichier `gmon.out` avec les information de profilage sera généré à chaque éxécution du programme.

    Ensuite, profilez le programme avec `gprof` en spécifiant le parcours de l'éxécutable et le fichier de profilage:
    ```
    gprof ../build/bin/mmg3d_O3d gmon.out
    ```
    Cette dernière programme montrera le _flat profile_ et le _call graph_ du programme. Vous pouvez voir les informations de profilage directement dans le code source avec les options suivantes:
    ```
    gprof -A -l ../build/bin/mmg3d_O3d gmon.out
    ```

5. Profilez l'utilisation de la mémoire par le programme avec l'outil _Massif_:
    ```
    valgrind --tool=massif --time-unit=ms ../build/bin/mmg3d_O3d A319_in_a_box.mesh -hmin 6 -hausd 5 -hgrad 1.7
    ```
    Pour chaque éxécution, Massif générera un fichier `massif.out.<PID>` où `<PID>` sera le numéro du processus associé à l'éxécution du programme.

    En sélectionnant le `<PID>` associé à votre dernière éxécution, vous pouvez imprimer les résultats du profilage avec:
    ```
    ms_print massif.out.<PID>
    ```

## B. Profilage et optimisation d'opérations matricielles

On essaie ici de profiler et optimiser le programme de test `loop.c`, qui utilise des simples boucles pour calculer des moyennes et produits de matrices.

1. Compilez le programme avec l'option `-pg` pour pouvoir le profiler
    ```
    gcc -O2 -pg loop.c -o program_loop
    ```
2. Lancez la commande `time` pour une première évaluation du temps de calcul. Puisqu'on a compilé le programme avec l'option `-pg`, un fichier `gmon.out` avec les informations de profilage détaillées sera généré à chaque éxécution:
    ```
    time ./program_loop
    ```
3. Visualisez les résultats du profilage (_flat profile_ et _call graph_):
    ```
    gprof program_loop gmon.out
    ```
    Vous pouvez voir ces mêmes résultats comme commentaires aux lignes de code en ajoutant les options `-A -l`.
4. Optimisez le programme:
    - Quelle fonction faudrait-il optimiser d'abord?
    - Pourrait-on améliorer la performance du produit matriciel en changeant l'ordre des indices sur lesquels on boucle? (Considerez que en langage C ces matrices sont stockées par lignes, donc à chaque utilisation des données la mémoire cache va être remplie avec des lignes des matrices)

## C. Profilage et optimisation de l'allocation de mémoire

On essaie ici de profiler et optimiser l'utilisation de la mémoire (avec _Callgrind_ et _Massif_) par le programme de test `coords.c`, qui calcule les coordonnées 3D d'un ensemble de points.

1. Compilez le programme avec l'option de debug `-g` (Callgrind ne veut pas l'option `-pg`):
    ```
    gcc -O2 -g coords.c -o program_coords
    ```
2. Profilez l'utilisation de la mémoire avec Massif:
   ```
   valgrind --tool=massif --time-unit=ms ./program_coords
   ```
   Cela produira un fichier `massif.out.<PID>`.

   Visualisez les résultats avec:
   ```
   ms_print massif.out.<PID>
   ```
3. Profilez le programme avec Callgrind:
    ```
    valgrind --tool=callgrind ./program_coords
    ```
    Cela peut demander quelques minutes. L'éxécution produira un fichier `callgrind.out.<PID>`, où `<PID>` sera le numéro du processus associé à l'éxécution du programme.

   Visualizer les résultats avec:
    ```
    callgrind_annotate --auto=yes callgrind.out.<PID>
    ```
4. Optimisez le programme:
    - Dans quelle fonction on passe le plus de temps?
    - Puisque la taille des coordonnées et le nombre de points sont connus à l'avance, y-aurait-il une façon plus éfficiente pour allouer la mémoire?
    - Essayez cette modification et comparer les résultats de son profilage (Callgrind et Massif) avec ceux de la version originelle du programme.
5. [Optionnel, ça prend beaucoup de temps d'installation] Le programme libre _KCachegrind_ permet une visualisation graphique des résultats de Callgrind. Il n'est pas disponible dans les modules sur PlaFRIM, mais il est disponible dans _Guix_: Guix est un gestionnaire de paquets (vous pouvez donc l'utiliser en alternative aux modules) libre et orienté vers la reproducibilité de l'environnement de travail. Il permet de stocker dans un dépôt Git la procédure de compilation des bibliothèques utilisées, et des créer des environnements de travail où vou avez le contrôles complet des bibliothèques installées et des variables d'environnement.

    - Créez une shell Guix dans laquelle KCachegrind sera disponible:
        ```
        guix shell kcachegrind
        ```
        La première fois que vous lancez cette commande, Guix téléchargera et compilera KCachegrind dans son _store_ avant de vous le mettre à disposition dans une shell.

        Une fois la shell disponible, vous pouvez visualiser vos résultats avec:
        ```
        kcachegrind callgrind.out.<PID>
        ```

        Une fois terminé, vous pouvez normalement quitter la shell Guix avec la commande `ctrl-D`.


### Références
- Modules https://modules.readthedocs.io/en/latest/index.html
- Guix https://guix.gnu.org/en/help/
- MPI via Slurm https://docs.open-mpi.org/en/main/running-apps/slurm.html
- Slurm https://slurm.schedmd.com/documentation.html
- Valgrind https://valgrind.org/docs/manual/quick-start.html
- Callgrind https://valgrind.org/docs/manual/cl-manual.html
- Callgrind https://web.stanford.edu/class/archive/cs/cs107/cs107.1174/guide_callgrind.html
- Massif https://valgrind.org/docs/manual/ms-manual.html
- PlaFRIM https://www.plafrim.fr/
- PlaFRIM https://plafrim-users.gitlabpages.inria.fr/doc/

### Remerciements
François Rué et Florent Pruvost (INRIA) pour la version initiale des diapositives.
