library(GGally)
library("FactoMineR")

# Chargement des données
load("données/eaux.rda")

# Creation de trois objet
X = data
n = nrow(X) # Nombres d'observation
p = ncol(X) # Nombres de variables

####################### Manipuler les données ###############################

# Manipulation des données
X[, 1:3] # les 3 premières colonnes
X[, c(1,5)] # la première et la 5ème colonne
X[, -c(1,5)] # toutes les colonnes sauf la première et la 5ème
X[1:3, ] # les 3 premières lignes
class(X) # Un data.frame est une liste dont les éléments sont les colonnes
X$saveur.amère # On récupère les élements d’une liste avec le $
X[ ,1]

####################### Moyenne et ecart type #################################

# Moyenne de la variable saveur amère
sum(X$saveur.amère)/n
mean(X$saveur.amère)
# Ecart-type de la variable saveur amère
x <- X$saveur.amère
sqrt(1/(n-1)*sum((x-mean(x))^2))
sd(x)
# Moyenne et écart-type de toutes les variables
moy <- apply(X, 2, mean) # 1=lignes, 2=colonnes
sigma <- apply(X,2, sd) # standard deviation

####################### Centrer Reduire #######################################
# Création des données centrées ...
Y <- sweep(X, 2, moy, "-")
apply(Y, 2, mean) # les colonnes sont bien centrées
Z <- sweep(Y, 2, sigma, "/")
apply(Z, 2, sd) # les colonnes sont bien de variance 1
# ou de manière équivalente
Z <- scale(X)
# ou avec l’écart-type non corrigé (comme en ACP)
Z <- scale(X)*sqrt(n/(n-1))

######################## Description bivariée #################################

# Avec la fonction ggpairs du package GGally
ggpairs(X[,1:5])
# Cacluler et visualiser la matrice de corrélation
z1 <- Z[, 1] # variable saveur amère standardisée
z2 <- Z[, 2] # variable saveur sucrée standardisée
sum(z1*z2)/n # corrélation entre les deux variables
cor(X$saveur.amère, X$saveur.sucrée)
cor(X[,1:5]) # matrice des corrélations entre les 5 première variables
ggcorr(X[,1:5])

################# Matrice de distance et correlations #########################

# Matrice des distances entre les individus
dist(X) # données brutes
dist(Y) # données centrées
dist(Z) # données centrées-réduites
# Corrélation entre les variables
cor(X)
# ou encore
t(Z) %*% Z/n # %*% est le produit matriciel


################## ACP sur matrice de covariance ##############################
# Fonction PCA du package FactoMineR
# (scale.unit=FALSE)
res <- PCA(X, graph = FALSE, scale.unit = FALSE)
# Figure individus
plot(res,choix = "ind", cex = 1.5, title = "")
# Figure variables
plot(res, choix = "var", cex = 1.5, title = "")
# Equivalent à la décomposition en valeurs propres de la matrice des covariances
Y <- as.matrix(Y)
C <- (t(Y) %*% Y)/n # matrice des covariances
eigen(C)$values
res$eig[, 1]

#################### ACP sur matrice de correlation ###########################
# Analyse en composantes principales normalisée (sur matrice des corrélations)
# (par défaut: scale.unit=TRUE)
res <- PCA(X, graph=FALSE)
# Figure individus
plot(res, choix = "ind", cex = 1.5, title = "") # plan 1-2
plot(res, choix = "ind", axes=c(2,3), cex = 1.5, title = "") # plan 2-3
# Figure variables
plot(res, choix = "var", cex = 1.5, title = "") # plan 1-2
plot(res, choix = "var", axes=c(2,3), cex = 1.5, title = "") # plan 2-3
# Equivalent à la décomposition en valeurs propres de la matrice des corrélations
R <- (t(Z) %*% Z)/n # matrice des corrélations
eigen(R)$values
res$eig[, 1]
# Récuperer les 2 premières compostantes principales
F <- res$ind$coord[, 1:2]
plot(F, pch = 16)
text(F, rownames(X), pos = 3) # on retrouve la figure des individus
# Récuperer les loadings (corrélations aux deux premières CP)
A <- res$var$coord[, 1:2]
plot(A, pch=16)
text(A, colnames(X), pos = 3) # on retrouve la figure des variables
A[1, , drop=FALSE] # corrélations entre saveur amère et les 2 premières CP
cor(F, X$saveur.amère)
# Interprétation du premier plan des individus en fonction des variables ?

############################# Interpretation ##################################

############################# Kaiser, regle du coude ##########################

# Inertie (variance) des composantes principales
apply(F, 2, var)*(n-1)/n # variances des 2 premières CP
res$eig[, 1]
sum(res$eig[, 1])
res$eig

############ Qualite de la projection des individus ##########################

# Qualité de la projection des individus sur les axes
res$ind$cos2
# Qualité de la projection des individus sur le premier plan
apply(res$ind$cos2, 1, sum)
# Interprétation du premier plan factoriel des individus ?

################### Qualite de projection des variables ######################

# Qualité de la projection des variables sur les axes
res$var$cos2
# Qualité de la projection des variables sur le premier plan
apply(res$var$cos2, 1, sum) # ou regarder la longeur des flèches !
# Interprétation du premier plan factoriel des variables ?


















