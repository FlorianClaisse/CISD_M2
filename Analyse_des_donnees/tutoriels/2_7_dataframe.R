# un data frame est une matrice mais ou chaque colonne peuvent être hétérogene.
# on peut changer une matrice en data frame grace a "as.data.frame(mat)
# Creation d'un data frame
taille = runif(12, 150, 180)
masse = runif(12, 50, 90)
sexe = rep(c("M", "F", "F", "M"), 3)
H = data.frame(taille, masse, sexe)
H
summary(H) # Afficher les informations global sur le data frame
head(H) # Afficher les premiere ligne du data frame
H[1,] # Recuperer la premiere ligne du data frame
H$taille # Recuperer toutes la colonne qui se nomme "taille"
MH = as.matrix(H) # Transformer le data frame en matrice
summary(MH) # Grosse perte d'information
as.list(H) # Tranformation du data frame en list
rm(taille)
H
H$taille
attach(H)
taille
detach(H)
