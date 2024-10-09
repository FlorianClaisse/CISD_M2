Tous ces exercices sont à essayer sur PlaFRIM, en utilisant Slurm pour l'allocation des ressources, selon les méthodes utilisées dans les précedentes exercices (feuille `exercices_1_slurm.md`). Une réservation interactives des ressources avec la commande `salloc` peut être convenable.

Les fichiers pour les exercices de cette séance sont les mêmes contenus dans le dossier `codes_2b_optim/`. Le but de la séance est d'analyser les performances des programmes (avant, pendant et après l'optimisation) avec l'outil Intel Advisor.


### Préparation de l'environnement de travail
Pour commencer:

1. Réservez des ressources interactives avec Slurm, par exemple:
    ```
    salloc -C miriel --time=01:00:00
    ```
    Une fois que Slurm vous aura réservé un nœud de calcul (`miriel<XX>` par exemple), connectez-vous avec:
    ```
    ssh -Y miriel<XX>
    ```
    > :warning: L'option `-Y` est importante, pour cette séance, pour pouvoir exporter l'interface graphique vers votre postation de travail.
    > :warning: L'erreur:
    > ```
    > X11 connection rejected because of wrong authentication. [...] Error: Unable to initialize GTK+, is DISPLAY set properly?
    > ```
    > indique que probablement l'option `-Y` n'a pas été passée dans une étape de connexion avec ssh.

2. Chargez les compilateurs GNU gcc et Intel avec les modules d'environnement:
    ```
    module load compiler/gcc/9.3.0
    module load compiler/intel/2020_update4
    ```
3. Ce module ne définie pas de raccourcies pour les éxécutables d'Intel Advisor, mais il définie la variable `CMPL_DIR` avec le parcours vers l'installation de la suite du compilateur Intel. Vous pouvez alors définir les alias suivants pour les éxécutables d'Advisor:
    ```
    alias advixe-cl="$CMPL_DIR/advisor/bin64/advixe-cl"
    alias advixe-gui="$CMPL_DIR/advisor/bin64/advixe-gui"
    ```
    Vous pouvez vérifier l'exactitude de vos parcours avec:
    ```
    echo $CMPL_DIR
    which advixe-cl
    which advixe-gui
    ```

### Analyse _roofline_ par ligne de commande
Chaque analyse se fait essentiellement en trois étapes:
1. Si nécessaire, recompilez le programme (par exemple `loop.c`) avec le compilateur Intel `icc` (ou `gcc`, mais Advisor vous donnera moins d'informations), optimisé avec les options de debug:
    ```
    icc -O3 -g loop.c -o program_loop
    ```

2. La commande `advixe-cl` permet de lancer des analyses avec Advisor. Une analyse _roofline_ demande deux analyses d'affilée:
    ```
    advixe-cl --collect survey --ignore-checksums --project-dir ./results_loop0 --no-trip-counts -- ./program_loop
    advixe-cl --collect tripcounts --ignore-checksums --project-dir ./results_loop0 --flop -- ./program_loop
    ```
    Glossaire minimale des options:
    - `--collect` est l'option principale qui permet de définir le type d'analyse, ici `survey` (analyse sommaire, principalement profilage) et `tripcounts` (racolte détaillée du nombre d'appels et d'instructions dans les différentes fonctions).
    - `--project-dir` permet de définir le dossier où sauvegarder les résultats.
    - `--ignore-checksums` permet de lancer des nouvelles analyses dans un dossier utilisé précedemment.
    - `--flop` permet de récolter des informations sur le nombre d'opérations en virgule flottante (FLOP).

3. Pour ouvrir les résultats dans l'interface graphique:
    ```
    advixe-gui results_loop0/results_loop0.advixeproj &
    ```
    et cliquez sur `Show My Result`. Naviguez les onglets `Summary` et `Survey & Roofline`. Dans ce dernier, la barre verticale gauche vous permet de passer de la visualisation _Survey_ à la visualisation _Roofline_.

    Pour plus d'informations: https://www.intel.com/content/www/us/en/develop/documentation/advisor-user-guide/top/analyze-cpu-roofline/explore-cpu-memory-roofline-results/cpu-roofline-report-overview.html

> :warning: Sur miriel078, la visualisation de l'analyse roofline pourrait ne pas être affichée correctement.

### Analyse par interface graphique
La commande `advixe-gui` permet de configurer une nouvelle analyse de zéro, ou de relancer une analyse avec des nouveaux paramètres.
1. Pour une nouvelle analyse, lancez `advixe-gui` sans arguments, et cliquez sur `Configure Project`. Cela vous permet de choisir l'éxécutable à analyser, le type et les paramètres de l'analyse.
2. Pour visualiser les résultats d'une analyse (par exemple dans le dossier `mesResultats`), lancez
    ```
    advixe-gui mesResultats/mesResultats.advixeproj
    ```
    et cliquez `Show My Result`. Là, utilisez le panneau gauche `Vectorization Workflow` pour changer les options de l'analyse et la relancer sur le même éxécutable déjà enregistré dans le projet.


## A. Analyse et optimisation d'un produit matriciel
1. Lancez des analyses pour les programmes obtenus avec les compilateurs `gcc` et `icc`. Pourquoi les résultats sont différents entre gcc et icc?
    > :warning: Conseil: nommez les deux éxécutables avec deux noms différents (par exemple `program_loop_icc` et `program_loop_gcc` et utilisez deux dossiers différents pour les résultats (par exemple `results_loop_gcc` et `results_loop_icc`).
2. Advisor vous conseillera de recompiler pour le bonne architecture du processeur (AVX2 sur les nœuds miriel). Pour faire cela, utilisez l'option `-mavx2` pour gcc, et `-xCORE-AVX2` pour icc; recompilez les programmes et relancez les analyses.
3. Ajoutez des options d'analyse par l'interface graphique (par exemple, cochez des boucles à étudier et sélectionnez la case "_For All Memory Levels_" dans l'analyse roofline), et relancez l'analyse par l'interface graphique.
4. Relancez l'analyse sur la version de `loop.c` optimisée par vous.
5. Vous pouvez comparer les résultats de deux analyses dans une même fenêtre dans la visualisation _roofline_.


### Références
- S. Williams et al., "Roofline: An Insightful Visual Performance Model for Multicore Architectures", https://zenodo.org/record/1236156
- https://www.intel.com/content/www/us/en/develop/documentation/advisor-user-guide/top/analyze-cpu-roofline/explore-cpu-memory-roofline-results/cpu-roofline-report-overview.html
- https://www.intel.com/content/www/us/en/developer/articles/technical/memory-level-roofline-model-with-advisor.html
- (Pour aller plus loins) STREAM benchmark for measuring sustainable memory bandwidth https://www.cs.virginia.edu/stream/ref.html

### Remerciements
François Rué et Florent Pruvost (INRIA) pour la version initiale des diapositives.
