Ces sont des propositions pour la solution des exercices B et C dans la feuille `exercices_optim.md`, dont les programmes sont dans le dossier `optim/`.

:warning: Pour ces analyse, ça peut convenir de passer à gcc les options `-fopt-info`, qui retourne les informations sur l'optimisation faite par le compilateur, et `-fno-inline`, qui empêche l'_inlining_ des fonctions (qui n'apporte pas des grandes différences de performances dans ces exercices, mais qui rend plus difficile de lire la sortie de gprof)

## B. Profilage et optimisation d'opérations matricielles
Le programme `loop.c` effectue des moyennes de matrices et des produits matriciels. Le profilage montre que c'est le produit matriciel qui consomme le plus de temps, même si la moyenne de matrices est calculée bien plus souvent.

Pour raisonner sur ses possibles optimisations, on reprend la définition du produit matriciel.

Selon la formule scolaire:
```math
C_{ij} = \sum_{k = 1}^n A_{ik} B_{kj} \qquad \forall i,j = 1,\ldots,n
```

Du point de vue de l'implémentation, on a en effet à gérer trois boucles (sur i, j, et k) après avoir proprement initialisé C:
```math
C_{ij} = C_{ij} + A_{ik} B_{kj} \qquad \forall i,j,k = 1,\ldots,n
```
et on peut se demander quel ordre est le plus efficient pour le calcul, en se souvenant qu'en C ces matrices sont sauvegardées en mémoire par lignes.


### Ordre $` (i,j,k) `$
À l'itération interne $` k `$:
```math
C_{ij} = C_{ij} + A_{i*} B_{*j} \qquad \forall i,j = 1,\ldots,n
```
<div align="center">
<i> On calcule chaque entrée de C comme un produit ligne-colonne, ce qui correspond à la formule scolaire du produit matriciel. </i>
</div>

Graphiquement:
```
   j                 j
 [    ]    [    ]  [ |  ]
 [    ]    [    ]  [ |  ]
i[ *  ] = i[----]  [ |  ]
 [    ]    [    ]  [ |  ]
```

Du point de vue de la mémoire, si on assume que la cache peut bien contenir une ligne de A, B et C:
- L'élément $` C_{i,j} `$ est déjà dans la cache (la ligne $` C_{i*} `$ a été lue pour l'itération externe $` i `$).
- L'élément $` A_{i,k} `$ est déjà dans la cache (la ligne $` A_{i*} `$ a été lue pour l'itération externe $` i `$).
- L'élément $` B_{k,j} `$ demande une ligne différente $` B_{k*} `$ (qui n'est probablement pas dans la cache) pour chaque itération $` k `$.


Au final: chaque ligne de A et C est lue une fois (pour chaque itération externe $` i `$), chaque ligne de B est possiblement lue $` n^2 `$ fois (pour chaque paire $` (i,j) `$).

Comment lire le moins de lignes de matrices possible?
- L'indice le plus interne de la boucle, idéalement, ne devrait pas être un ligne => l'indice j est un bon candidat.
- L'indice le plus externe devrait contenir, idéalement, le plus de lignes possible => l'indice i est un bon candidat.

Cela nous amène à analyser l'ordre $` (i,k,j) `$.

### Ordre $` (i,k,j) `$
À l'itération interne $` j `$:
```math
C_{i*} = C_{i*} + A_{ik} B_{k*} \qquad \forall i,k = 1,\ldots,n
```
<div align="center">
<i> On incrémente chaque ligne i de C avec une ligne k de B, amplifiée par l'entrée (i,k) de A. </i>
</div>

L'opération vectorielle dans l'itération interne est aussi connue avec l'acronyme _DAXPY_ (**d**ouble precision **a**lpha by **x** **p**lus **y**, car elle résout une équation du type $` \vec y = \alpha \vec x + \vec y `$).

Graphiquement:
```
             k
 [    ]    [    ]  [    ]
 [    ]    [    ] k[----]
i[----] = i[ *  ]  [    ]
 [    ]    [    ]  [    ]
```

Du point de vue de la mémoire, si on assume que la cache peut bien contenir une ligne de A, B et C:
- L'élément $` C_{i,j} `$ est déjà dans la cache (la ligne $` C_{i*} `$ a été lue pour l'itération externe $` i `$).
- L'élément $` A_{i,k} `$ est déjà dans la cache (la ligne $` A_{i*} `$ a été lue pour l'itération externe $` i `$), et il est réutilisé pour toutes les itérations $` j `$.
- L'élément $` B_{k,j} `$ est déjà dans la cache (la ligne $` B_{k*} `$ a été lue pour l'itération intermédiaire $` k `$).

Au final: chaque ligne de A et C est lue une fois (pour chaque itération externe $` i `$), chaque ligne de B est possiblement lue $` n `$ fois (pour chaque itération intermédiaire $` k `$).

### Performances
Sortie de `gprof` pour la boucle $`( i,j,k )`$:
```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
 91.89     21.25    21.25                             multiply
  9.23     23.38     2.13                             average

```

Regardons les performances en mémoire avec l'aide de Callgrind (avec l'option `--simulate-cache=yes`):
```
==54670== Callgrind, a call-graph generating cache profiler
==54670== Copyright (C) 2002-2017, and GNU GPL'd, by Josef Weidendorfer et al.
==54670== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==54670== Command: ./program_loop
==54670== 
--54670-- warning: L3 cache found, using its data for the LL simulation.
--54670-- warning: specified LL cache: line_size 64  assoc 20  total_size 15,728,640
--54670-- warning: simulated LL cache: line_size 64  assoc 30  total_size 15,728,640
==54670== For interactive control, run 'callgrind_control -h'.
==54670== 
==54670== Events    : Ir Dr Dw I1mr D1mr D1mw ILmr DLmr DLmw
==54670== Collected : 77576391587 19700690100 9850338583 796 8924472315 157811617 788 8909808201 157811569
==54670== 
==54670== I   refs:      77,576,391,587
==54670== I1  misses:               796
==54670== LLi misses:               788
==54670== I1  miss rate:           0.00%
==54670== LLi miss rate:           0.00%
==54670== 
==54670== D   refs:      29,551,028,683  (19,700,690,100 rd + 9,850,338,583 wr)
==54670== D1  misses:     9,082,283,932  ( 8,924,472,315 rd +   157,811,617 wr)
==54670== LLd misses:     9,067,619,770  ( 8,909,808,201 rd +   157,811,569 wr)
==54670== D1  miss rate:           30.7% (          45.3%   +           1.6%  )
==54670== LLd miss rate:           30.7% (          45.2%   +           1.6%  )
==54670== 
==54670== LL refs:        9,082,284,728  ( 8,924,473,111 rd +   157,811,617 wr)
==54670== LL misses:      9,067,620,558  ( 8,909,808,989 rd +   157,811,569 wr)
==54670== LL miss rate:             8.5% (           9.2%   +           1.6%  )
```
Les lignes `I` concernent les instructions, les lignes `D` concernent les données. Le `1` indique la mémoire cache L1, le "LL" indique le dernier niveau (_last level_) de mémoire cache (voir https://valgrind.org/docs/manual/cg-manual.html pour plus d'informations). On note des taux élévé de _cache misses_ quand il s'agit de lire des données en mémoire.


Ici par contre la sortie de `gprof` pour la boucle $`(i,k,j)`$:
```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
 73.98      5.77     5.77                             multiply
 27.08      7.88     2.11                             average
```
et la sortie de Callgrind (avec `--simulate-cache=yes`):
```
==53179== Callgrind, a call-graph generating cache profiler
==53179== Copyright (C) 2002-2017, and GNU GPL'd, by Josef Weidendorfer et al.
==53179== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==53179== Command: ./program_loop
==53179== 
--53179-- warning: L3 cache found, using its data for the LL simulation.
--53179-- warning: specified LL cache: line_size 64  assoc 20  total_size 15,728,640
--53179-- warning: simulated LL cache: line_size 64  assoc 30  total_size 15,728,640
==53179== For interactive control, run 'callgrind_control -h'.
==53179== 
==53179== Events    : Ir Dr Dw I1mr D1mr D1mw ILmr DLmr DLmw
==53179== Collected : 77568009124 28286430389 9850338584 797 1397967867 157811617 789 1389367881 157811569
==53179== 
==53179== I   refs:      77,568,009,124
==53179== I1  misses:               797
==53179== LLi misses:               789
==53179== I1  miss rate:           0.00%
==53179== LLi miss rate:           0.00%
==53179== 
==53179== D   refs:      38,136,768,973  (28,286,430,389 rd + 9,850,338,584 wr)
==53179== D1  misses:     1,555,779,484  ( 1,397,967,867 rd +   157,811,617 wr)
==53179== LLd misses:     1,547,179,450  ( 1,389,367,881 rd +   157,811,569 wr)
==53179== D1  miss rate:            4.1% (           4.9%   +           1.6%  )
==53179== LLd miss rate:            4.1% (           4.9%   +           1.6%  )
==53179== 
==53179== LL refs:        1,555,780,281  ( 1,397,968,664 rd +   157,811,617 wr)
==53179== LL misses:      1,547,180,239  ( 1,389,368,670 rd +   157,811,569 wr)
==53179== LL miss rate:             1.3% (           1.3%   +           1.6%  )
```
On note que le nombre de _cache misses_ a été considerablement reduit par l'échange d'indices de la boucle, ce qui explique les meilleures performances en termes de temps de calcul.


### Notes
- Cette solution permet de raisonner sur les accès en mémoire de l'algoritme, mais celui ci pourrait être encore plus optimisé à l'aide d'un outil qui analyse l'utilisation de la cache par le programme.
- Celle solution est un exemple d'optimisation du produit matriciel, mais elle n'est absolumment pas la seule optimisation possible ou la meilleure. Le sujet est vaste (voir https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm) et des optimisations plus avancées sont possibles (par exemple https://en.wikipedia.org/wiki/Loop_nest_optimization#Example:_matrix_multiplication).


## C. Profilage et optimisation de l'allocation de mémoire
Le programme `coords.c` veut calculer des coordonnées (fictives) d'un ensemble de points en 3D.

### Version originale
Le programme alloue 3 _double_ pour chaque point dans un vecteur de taille $` n `$. La sortie de `time` returne le temps de calcul:
```
real	0m2.443s
user	0m1.616s
sys	0m0.826s
```

La sortie de Callgrind montre en effet que la plupart des instructions (`Ir`) est dans les fonctions de bibliothèque `malloc` et `free` pour allouer et désallouer les coordonnées de chaque point:
```
--------------------------------------------------------------------------------
Profile data file 'callgrind.out.36318' (creator: callgrind-3.13.0)
--------------------------------------------------------------------------------
I1 cache: 
D1 cache: 
LL cache: 
Timerange: Basic block 0 - 3250125159
Trigger: Program termination
Profiled target:  ./program_coords (PID 36318, part 1)
Events recorded:  Ir
Events shown:     Ir
Event sort order: Ir
Thresholds:       99
Include dirs:     
User annotated:   
Auto-annotation:  on

--------------------------------------------------------------------------------
            Ir 
--------------------------------------------------------------------------------
15,150,560,202  PROGRAM TOTALS

--------------------------------------------------------------------------------
           Ir  file:function
--------------------------------------------------------------------------------
7,499,992,249  ???:_int_malloc [/usr/lib64/libc-2.17.so]
3,649,999,996  ???:_int_free [/usr/lib64/libc-2.17.so]
2,000,000,097  ???:malloc [/usr/lib64/libc-2.17.so]
1,200,000,028  coords.c:main [/home/lcirrott/tp_enseirb-matmeca_2022/optim/program_coords]
  800,000,038  ???:free [/usr/lib64/libc-2.17.so]

--------------------------------------------------------------------------------
-- Auto-annotated source: coords.c
--------------------------------------------------------------------------------
         Ir 

          .  #include <stdlib.h>
          .  #include <string.h>
          .  
          .  #define n 50000000
          .  //#define n 100000000
          .  
          .  void point_allocate( double **mycoord, int i, int size ) {
200,000,000    mycoord[i] = (double*) malloc( size * sizeof(double) );
9,500,386,523  => ???:malloc (50000000x)
          .  }
          .  
          .  void assign( double **mycoord, int i ) {
150,000,000    mycoord[i][0] = (double)i;
200,000,000    mycoord[i][1] = (double)(2*i);
250,000,000    mycoord[i][2] = (double)(3*i);
          .  }
          .  
          3  int main( int argc, char* argv[] ) {
          .    double **coords;
          .    int i;
          .  
          .    /* Allocate pointers to coordinates */
          9    coords = (double**) malloc( n * sizeof(double*) );
     58,911  => ???:_dl_runtime_resolve_xsave'2 (1x)
          .  
100,000,004    for ( i = 0; i < n; i++ ) {
          .      /* For each point, allocate an array of 3 coordinates */
          .      point_allocate( coords, i, 3 );
          .      /* ... Dummy assignement of point coordinates... */
          .      assign( coords, i );
          .    }
          .  
          .    /* ... Imagine there is a computational mechanics code here ... */
          .  
          .    /* Free memory */
100,000,000    for ( i = 0; i < n; i++ ) {
200,000,004      free( coords[i] );
        938  => ???:_dl_runtime_resolve_xsave'2 (1x)
4,449,999,911  => ???:free (49999999x)
          .    }
          3    free( coords );
         37  => ???:free (1x)
          .  
          .    return EXIT_SUCCESS;
          5  }

--------------------------------------------------------------------------------
Ir 
--------------------------------------------------------------------------------
 8  percentage of events annotated

```

La sortie de Massif nous montre le profil d'utilisation de la mémoire par le programme:
```
--------------------------------------------------------------------------------
Command:            ./program_coords
Massif arguments:   --time-unit=ms
ms_print arguments: massif.out.35966
--------------------------------------------------------------------------------


    GB
2.235^                                   ##                                   
     |                                  :# :                                  
     |                                :::# :::                                
     |                               :: :# :: :::                             
     |                            ::::: :# :: : :::                           
     |                          :::: :: :# :: : :::@::                        
     |                        ::: :: :: :# :: : :::@: :                       
     |                     :::: : :: :: :# :: : :::@: :::                     
     |                    :: :: : :: :: :# :: : :::@: :::@::                  
     |                  :::: :: : :: :: :# :: : :::@: :::@: ::                
     |               :::: :: :: : :: :: :# :: : :::@: :::@: :::@              
     |            ::@:  : :: :: : :: :: :# :: : :::@: :::@: :::@:::           
     |           :: @:  : :: :: : :: :: :# :: : :::@: :::@: :::@: ::          
     |        ::::: @:  : :: :: : :: :: :# :: : :::@: :::@: :::@: :::@        
     |       :: ::: @:  : :: :: : :: :: :# :: : :::@: :::@: :::@: :::@:::     
     |     @@:: ::: @:  : :: :: : :: :: :# :: : :::@: :::@: :::@: :::@: :::   
     |   ::@ :: ::: @:  : :: :: : :: :: :# :: : :::@: :::@: :::@: :::@: :::@: 
     | ::: @ :: ::: @:  : :: :: : :: :: :# :: : :::@: :::@: :::@: :::@: :::@::
     | : : @ :: ::: @:  : :: :: : :: :: :# :: : :::@: :::@: :::@: :::@: :::@::
     | : : @ :: ::: @:  : :: :: : :: :: :# :: : :::@: :::@: :::@: :::@: :::@::
   0 +----------------------------------------------------------------------->s
     0                                                                   90.92
```
le profil est triangulaire car les coordonnées sont allouées itérativement dans une boucle sur les points, pour après être désallouées encore itérativement.
Le temps de calcul est beaucoup plus long car Massif ralentit l'éxécution du programme.

### Version améliorée
Comme la taille des coordonnées est connue (le programme veut pouvoir travailler en 3D), on pourrait plutôt penser d'allouer toutes les coordonnées en une seule fois, dans un vecteur de taille $` 3n `$. Le résultat, dans le programme `coords_better.c`, est bien plus rapide:
```
real	0m0.002s
user	0m0.000s
sys	0m0.002s
```

La sortie de Callgrind nous montre que le processeur éxécute effectivement beaucoup moins d'instruction:
```
--------------------------------------------------------------------------------
Profile data file 'callgrind.out.37617' (creator: callgrind-3.13.0)
--------------------------------------------------------------------------------
I1 cache: 
D1 cache: 
LL cache: 
Timerange: Basic block 0 - 23631
Trigger: Program termination
Profiled target:  ./program_coords_better (PID 37617, part 1)
Events recorded:  Ir
Events shown:     Ir
Event sort order: Ir
Thresholds:       99
Include dirs:     
User annotated:   
Auto-annotation:  on

--------------------------------------------------------------------------------
     Ir 
--------------------------------------------------------------------------------
116,398  PROGRAM TOTALS
```

Le programme est tellement rapide que Massif n'arrive pas a recueillir suffisamment de données. On active alors dans le programme `coords_better.c` la fonction `check()` (qui contrôle le résultat sans modifier l'utilisation de la mémoire), pour avoir une durée d'éxécution significative pour le profilage.
La sortie de Massif nous montre le nouveau profil, rectangulaire car toute la mémoire est alloué et désalloué d'un seul coup:
```
--------------------------------------------------------------------------------
Command:            ./program_coords_better
Massif arguments:   --time-unit=ms
ms_print arguments: massif.out.45286
--------------------------------------------------------------------------------


    GB
1.118^    :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
     |    :                                                                  #
   0 +----------------------------------------------------------------------->s
     0                                                                   1.853
```
