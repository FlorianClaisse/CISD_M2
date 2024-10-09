## A. Connexion à PlaFRIM et transfert de fichiers

### Connexion
Le programme `ssh` (Secure SHell) vous permet de vous connecter à une machine distante par ligne de commande.
Une connexion ssh démarrée dans un terminal de _votre ordinateur local_ vous ouvrira un terminal _sur la machine distante_.

Pour faire cela avec PlaFRIM, vous avez deux options:

1. Par mot de passe, en deux étapes, dépuis un terminal de votre ordinateur:
    ```
    ssh -Y <LOGIN>@formation.plafrim.fr
    ```

    > :warning: Remplacez bien `<LOGIN>` avec votre nom utilisateur sur PlaFRIM, _sans chevrons_.

    et (depuis le nouveau terminal)
    ```
    ssh -Y plafrim
    ```
    Vous serez maintenant sur le nœud miriel045 de PlaFRIM.

2. **En alternative**, vous pouvez vous connecter avec une seule commande ssh, en ajoutant d'abord dans le fichier `.ssh/config` _de votre ordinateur_:
    ```
    Host formation
      ForwardAgent yes
      ForwardX11 yes
      User <LOGIN>
      ProxyCommand ssh -T -q -o "ForwardAgent Yes" -l <LOGIN> formation.plafrim.fr 'ssh-add -t 1 && nc plafrim 22'
    ```
    et en exécutant la commande (depuis votre ordinateur):
    ```
    ssh -Y formation
    ```
    Vous serez maintenant sur le nœud miriel045 de PlaFRIM.

    > :warning: Si le fichier `.ssh/config` n'existe pas dans votre ordinateur, vous pouvez le créer dans le dossier `.ssh/` qui devrait normalement déjà exister.


### Transfert de fichiers
Pour copier des fichiers de votre ordinateur à Plafrim (et viceversa), vous avez encore plusieurs options. Pour facilité, on assume ici d'avoir utilisé l'option de connexion 2 (vous avez donc dans le fichier `.ssh/config` un host "formation" qui identifie votre compte sur PlaFRIM).

> :warning: Testez une fois la commande `ssh formation` avant d'essayer le transfert de fichiers pour éviter des refus de connexion.

> :warning: Votre dossier home sur PlaFRIM sera initialement vide.

1. Pour copier des fichiers de votre ordinateur à PlaFRIM, vous pouvez utiliser:
   ```
   scp -r -p <MYFILES> formation:/home/<LOGIN>/<REMOTEFOLDER>/
   ```
   ou
   ```
   rsync -av -e ssh <MYFILES> formation:/home/<LOGIN>/<REMOTEFOLDER>/
   ```
   Enversez les parcours d'origine et de destination pour copier de PlaFRIM à votre ordinateur.

   > :warning: Les commandes `cp`, `scp`, `rsync` suivent toujours un schéma `<COMMANDE> <SOURCE> <DESTINATION>`. Le parcours d'origine précède donc toujours le parcours de destination.

   > :warning: Comme pour toute commande Linux, n'hésitez pas à taper `man scp` et `man rsync` pour plus informations sur les commandes et leurs options.

   > En supposant que je suis dans la home de mon ordinateur, dans laquelle j'ai un fichier "testfile" que je veux copier dans mon dossier "Programming" sur PlaFRIM (que j'ai précédemment crée), ma commande serait donc:
   > ```
   > scp -p testfile formation:/home/lcirrott/Programming/
   > ```

2. **En alternative**, vous pouvez monter dans le système de fichiers local des dossiers remotes avec:
    ```
    sshfs formation:/home/<LOGIN>/<REMOTEDIR> <MOUNTPOINT>/
    ```
    et le démonter avec:
    ```
    fusermount -u <MOUNTPOINT>
    ```

    > :warning: Il vous faudra remplacer `<LOGIN>` par votre nom utilisateur sur PlaFRIM (toujours sans chevrons), `<REMOTEDIR>` par le nom d'un dossier distant _qui existe déjà_ sur PlaFRIM (vous pouvez le laisser vide pour monter toute votre home), et <MOUNTPOINT> par le parcours local d'un dossier (vide!) dans lequel vous souhaitez monter votre dossier distant.

    La dernière commande vous permet de _monter_ localement le dossier distant, et de l'utiliser pour lire, modifier, copier des fichiers en vous cacheant que tout cela est en train de passer par une connexion ssh. L'expérience d'utilisation sera similaire à quand vous montez une clef USB sur votre ordinateur (sauf qu'il s'agira d'une grappe de calcul loin quelque part).

    > En supposant que je suis dans la home de mon ordinateur local, et que je veux voir mon dossier distant "Programming" dans mon dossier local (vide) "mirror", ma commande serait donc:
    > ```
    > sshfs formation:/home/lcirrott/Programming mirror
    > ```
    > suivie par (une fois terminé mon travail)
    > ```
    > fusermount -u mirror
    > ```

> :warning: Dans tous les cas décrits avant, il vous faudra démarrer la commande depuis votre ordinateur local, et pas par PlaFRIM. Cela simplement parce que vous pouvez donner à votre machine locale l'adresse de PlaFRIM dans le réseau, mais l'enverse est plus difficile et pas toujours possible.

## B. Partage des ressources avec SLURM
Les fichiers des exercises suivants sont dans le dossier `codes_1_slurm/` de ce dépôt, que vous pouvez télécharger sur votre machine locale et copier sur PlaFRIM comme vu dans le paragraphe précédent.

Les exercices sont en ordre de compléxité croissante, ne brûlez pas les étapes.

1. Explorez les nœuds avec la commande `sinfo`.
2. Demandez un job intéractif avec `salloc` (lancez la commande `man salloc` pour trouver des informations sur les options).
    - Contrôlez l'état de l'allocation du job avec la commande `squeue`.
    - Connectez-vous au nœud alloué avec `ssh`, compilez le programme `hello.c` avec la librairie OpenMPI 3.1.4-all que vous chargez dans votre environnement avec `module` et lancez-le en parallèle.

    > :warning: Vous pouvez par exemple demander un nœud miriel pour dix minutes avec la commande:
    > ```
    > salloc -C miriel --time=00:10:00
    > ```

    > :warning: Vous pouvez regarder exclusivement les jobs de votre compte avec la commande:
    > ```
    > squeue -u <LOGIN>
    > ```

    > :warning: En supposant que salloc vous a réservé le nœud miriel087, vouz pouvez vous connecter à ce nœud là avec:
    > ```
    > ssh miriel087
    > ```

    > :warning: Le système de fichiers est visible depuis tous les nœuds de calcul, donc vos fichiers seront disponibles et modifiables depuis n'importe quel nœud vous serez.

3. Utilisez la commande `sbatch` pour soumettre à l'ordonnanceur le job `job_hello.slm`. Suivez son exécution avec `squeue`, contrôlez le résultat dans les fichiers en sortie.
4. Le programme `ring.c` résout l'équation de la chaleur en une dimension sur un domaine périodique, en parallèle (avec donc un schéma de communication "en anneau")
    ```math
    \frac{ \partial u }{ \partial t } = \alpha \frac{ \partial^2 u }{ \partial x^2 }
    ```
    avec condition initiale
    ```math
    u^{(0)}(x) = A \sin ( \frac{ 2 \pi }{ L } x )
    ```
    en utilisant simplement une méthode d'Euler explicite et un schéma spatial centré
    ```math
    u^{(n+1)}_i = u^{(n)}_i + \frac{\alpha \Delta t}{\Delta x^2} ( u^{(n)}_{i+1} - 2 u^{(n)}_i + u^{(n)}_{i-1} )
    ```
    et une parallélisation à mémoire distribuée avec nœuds fantômes via le standard MPI. Le programme peut être compilé avec
    ```
    mpicc -O3 ring.c -lm -o program_ring
    ```
    et exécuté avec une commande comme
    ```
    mpirun -np <NP> ./program_ring <NXTOT>
    ```
    où le paramètre `<NXTOT>` contrôle le nombre total de degrées de liberté (et donc la taille du domaine $` L = N_x \Delta x `$, en étant les paramètre $` \Delta x `$ et $` \Delta t `$ constants dans cette simple version).
    - Compilez le programme avec gcc 9.3 et OpenMPI 3.1.4-all, et lancez le programme (via des jobs Slurm) avec un million de degrées de liberté, sur un nombre de procès MPI croissant de 1 à 64 selon une puissance de 2. Regardez le temps d'exécution et évaluez la _scalabilité forte_ du programme.
    - Lancez le programme (via des jobs Slurm) sur un nombre de procès MPI croissant de 1 à 64 avec cent mille degrées de liberté _par procès MPI_. Regardez le temps d'exécution et évaluez la _scalabilité faible_ du programme.
    - Proposez des scripts bash pour automatiser le lancement des jobs pour les études de scalabilité.


### Références
- https://modules.readthedocs.io/en/latest/index.html
- https://docs.open-mpi.org/en/main/running-apps/slurm.html
- https://slurm.schedmd.com/documentation.html
- https://prace-ri.eu/
- https://www.genci.fr/fr
- https://www.plafrim.fr/
- https://plafrim-users.gitlabpages.inria.fr/doc/
- https://www.top500.org/

### Remerciements
François Rué et Florent Pruvost (INRIA) pour la version initiale des diapositives.
