Ceci est une illustration de l'utilisation d'Intel Advisor pour l'exercices A dans la feuille `exercices_roofline.md`, dont les programmes sont dans les dossiers `optim/` et `solutions_optim`.

## A. Analyse et optimisation d'un produit matriciel
Pour rappel, on compare ici les performances de l'algoritme
```math
C_{ij} = C_{ij} + A_{ik} B_{kj} \qquad \forall i,j,k = 1,\ldots,n
```

Pour des boucles imbriquées selon les indices $`(i,j,k)`$ et $`(i,k,j)`$. On indique les deux versions des codes avec `loop.c` et `loop_ikj.c`.

On a vu que `icc`, avec l'option `-O3`, arrive à échanger l'ordre des boucles (j,k) dans sa phase d'optimisation. Pour contrôler ça, c'est utile d'ajouter l'option `-qopt-report` à la compilation de `loop.c` pour imprimer un rapport de vectorisation dans un fichier `loop.optrpt` (un rapport similaire peut être obtenu par `gcc` en terminal avec l'option `-fopt-info` et ses spécialisations).

Dans la suite, c'est recomanndé de comparer les programme issus des codes `loop.c` et `loop_ikj` en compilant avec gcc, ou, comme c'est fait dans la suite, en ajoutant l'option `-fno-inline` à icc (ce qui dans notre cas empêche au compilateur d'échanger les boucles j et k).

La comparaison suivante est donc faite avec les lignes de compilation:
```
icc -O3 -g loop.c -o program_loop -fno-inline -qopt-report -xCORE-AVX2
icc -O3 -g loop_ikj.c -o program_loop_ikj -fno-inline -qopt-report -xCORE-AVX2
```

### Ordre $`(i,j,k)`$
Cette version de l'algorithme demande, pour la boucle interne, d'effectuer 2 opérations (la multiplication de l'élément $` A_{ik} `$ avec l'élément $` B_{kj} `$, et l'addition de ce résultat au valeur courant de $` C_{ij} `$) avec la lecture des 2 données _double_ $` A_{ik} `$ et $` B_{kj} `$ (l'élément $` C_{ij} `$ étant réutilisé pour toute la boucle interne, il peut être stoqué dans un régistre du processeur). L'intensité algorithmique de notre noyau de calcul est donc:
```math
I = \frac{ 2\ \mathrm{FLOPS} }{ 2 \times 8\ \mathrm{bytes} } = \frac{1}{8}\ \mathrm{FLOPS/byte} = 0.125\ \mathrm{FLOPS/byte}
```

Voici la visualisation _Survey_ pour le programme original:
![survey_ijk](solutions_roofline/images/survey_ijk.png)
On note déjà que la vectorisation de la boucle interne est inefficiente, signe d'un possible accès à des données pas alignées en mémoire.

La visualisation _Roofline_ nous montre que la performance est bien dans la région _memory bound_ du diagramme, limitée par la bande de la mémoire DRAM: ![roofline_ijk](solutions_roofline/images/roofline_ijk.png)

Ceci est cohérent avec le taux élévé de _cache misses_ qu'on avait déjà mésuré avec Callgrind.
Une analyse roofline pour tous les niveaux de mémoire nous montre plus en détail l'utilisation de la cache:
![roofline_mem_ijk](solutions_roofline/images/roofline_mem_ijk.png)
L'impact de l'accès à la mémoire DRAM est significatif, compte tenu du fait qu'il s'agit du niveau de mémoire le plus lent.

### Ordre $`(i,k,j)`$
Cette version de l'algorithme demande, pour la boucle interne, d'effectuer toujours 2 opérations (la multiplication de l'élément $` A_{ik} `$ avec l'élément $` B_{kj} `$, et l'addition de ce résultat au valeur courant de $` C_{ij} `$) avec 3 transfers de données en mémoire: la lecture des 2 données _double_ $` C_{ij} `$ et $` B_{kj} `$ (l'élément $` A_{ik} `$ étant réutilisé pour toute la boucle interne, il peut être stoqué dans un régistre du processeur), et l'écriture du résultat dans l'élément $` C_{ij} `$. L'intensité algorithmique de notre noyau de calcul est donc:
```math
I = \frac{ 2\ \mathrm{FLOPS} }{ 3 \times 8\ \mathrm{bytes} } = \frac{1}{12}\ \mathrm{FLOPS/byte} = 0.0833\ \mathrm{FLOPS/byte}
```
L'intensité algorithmique est donc légèrement inférieure à l'intensité du programme précedent. Même si on cherche généralement à incrémenter l'intensité algorithmique d'un noyau, l'analyse _roofline_ nous montrera finalement si cette baisse pose des problèmes ou pas.

La visualisation _Survey_ nous montre que cette fois la boucle est bien vectorisée:
![survey_ikj](solutions_roofline/images/survey_ikj.png)

La visualisation _Roofline_ nous montre que le programme est encore _memory bound_, mais la performance atteinte est supérieure à celle de la boucle (i,j,k) (8.3 GFLOPS vs. 1.104 GFLOPS). et elle dépasse la limite posée par la bande de la mémoire L2:
![roofline_ikj](solutions_roofline/images/roofline_ikj.png)
Cela même si l'intensité algorithmique est inférieure (0.083 vs 0.125). En effet, la baisse d'inténsité algorithmique ferait aussi baisser les performances si le noyau était encore limité par la bande de la DRAM. mais dans notre cas le nouveau noyau est limité par la bande de la mémoire L2 (qui est bien plus élévée).

Une analyse pour tous les niveaux de mémoire nous confirme que dans cette version les performances sont limité par l'utilisation de la mémoire cache (de L1 à L3) plutôt que de la DRAM:
![roofline_mem_ikj](solutions_roofline/images/roofline_mem_ikj.png)

### Conclusion
Le but de ces exercices et de leur analyse est de montrer comme l'optimisation de code est souvent un processus itératif, effectué à l'aide des mésures des performances (spécifiques à l'ordinateur utilisé) avec des outils d'analyse variés et complémentaires.
