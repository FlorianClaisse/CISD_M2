# Les arrays sont des matrices de dimensions supérieur à 2.
E = array(c(1:8, rep(1, 8), seq(0, 1, len=8)), dim=c(2, 4, 3))
E
E[, , 1] # Recupere la matrice a la position 1
dim(E) # Dimension de l'array
length(E) # Nombre d'elements total rows * cols * nb_matrix
nrow(E) # Nombre de lignes de chaque sous matrice
ncol(E) # Nombre de colonnes de chaque sous matrice
aperm(E, resize = FALSE)
E + 10 # Ajouter 10 a tous les elements
H = array(1:12, c(2, 3, 2))
H
apply(H, 1, sum) # Somme de chaque 1er puis 2eme... lignes entre chaque sous matrice
apply(H, 2, sum) # Somme de chaque 1er puis 2eme... colonnes entre chaque sous matrice
apply(H, 3, sum) # Somme de chaque sous matrice
