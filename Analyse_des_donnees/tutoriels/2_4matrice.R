# creation d'un matrice
A = matrix(1:15, ncol=5)
# Afficher A
A
A[1, 1] = "Test"
# changer le nom des lignes
rownames(A) = c(paste("ligne", 1:3, sep=""))
A[1, 3] # Extraire à une position precise
A[, 2] # Extraire une colonne entiere precise
A[2, ] # Extraire une ligne entiere precise
A[1:3, c(2, 5)] # Extraire des ligne et des colonnes precise
A[1:3, -c(2, 5)] # Extraire ligne et toutes les autres colonnes sans les (-)
cbind(A, A) # Concatener deux matrice en ajoutant les colonnes
rbind(A, A) # Concatener deux matrice en ajoutant les lignes
dim(A) # dimension d'une matrice
nrow(A) # Nombre de lignes
ncol(A) # Nombre de colonnes
t(A) # Transposer de A
det(A[, 3:5]) # determinant de la matrice
solve(A[1:2, 2:3]) # inversion d'une matrice
diag(A) # Diagonal de la matrice
diag(1:5) # Creation d'une matrice diagonal de taille 1:5
A > 5 # Matrice de booléens
A[A < 5] = 0 # Toute les valeurs < prennent la valeur 0
colSums(A) # Somme colonne par colonne
rowSums(A) # Somme ligne par ligne
rowMeans(A) # Moyenne ligne par ligne de A
apply(A, 1, max)
A + A # Somme de deux matrice
A * B # Multiplication de deux matrice
t(B) %% A 
5 * A # Multiplication par un scalaire












