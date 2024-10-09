- Ces scripts sont des propositions de solution pour l'exercice B4 dans la feuille `exercices_slurm-debug.md`.
- Pour pouvoir les utiliser, Ces fichiers sont à copier dans le dossier `slurm/`.

Il est raccomandé de lire les scripts pour comprendre leur fonctionnement.

Sur PlaFRIM, vous pouvez utiliser ces scripts dans l'ordre suivant.

1. Pour lancer les tests de scalabilité:
    ```
    ./run_scalability.sh
    ```
    (modifiez le suffix et le calcul du nombre de points si vous voulez passer de scalabilité forte à faible, et viceversa)

2. Pour extraire les statistiques sur le temps de calcul:
    ```
    ./get_stats out_ring _strong
    ```

3. Pour visualiser la scalabilité, en utilisant Guix (à la place des modules) pour charger les programmes nécessaires:
    ```
    guix shell --pure which python python-matplotlib -- python3 plot.py
    ```
