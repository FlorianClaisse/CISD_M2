# Les liste peuvent contenir tous types de données et peuvent etre accedé par [[]] ou par myList$'name'
x = list("toto", 1:8) # Creation d'une liste
x
y = list(matrice=matrix(1:15, ncol=5),
         vecteur=seq(1, 20, by=5),
         texte="toto",
         scalaire=8) # Creation d'une liste 
y
x[[1]] # Recuperation de la valeur a la position 1
x[[2]] + 1 # Addtion de tous les elements a la position 2
y$matrice # Recupere la valeur du champ nommé "matrice"
y$vecteur
y[c("texte", "scalaire")] # Affichage des champs "texte" et "scalaire"
names(y) # Afficher les noms des champs
length(y) #Le nombre d'element dans la "liste"
length(y$vecteur)
cos(y$scalaire) + y[[2]][1]
summary(y) # Information sur les elements qui compose la liste
