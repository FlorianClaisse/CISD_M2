# Pour voir ou nos données seront sauvegardé
getwd()
# Pour mettre a jour le repertoire de travail
setwd("/Users/florianclaisse/Documents/Enseirb/Analyse_des_données")
# Afficher l'aide sur une fonction
help(plot)
?plot
# Pour installer un nouveau package, on peut aller dans Tools->Install Pachages
# ou dans l'onglet package en bas à droite
# ou utiliser la fonction install.packages()
# ou telecharger l'archive .zip et aller dans Tools
# ou installer directement avec le terminal R avec "R CMD INSTALL package-name.tar.gz"
# Pour installer un librairies dispo sur github, il faut installer "devtools" la charger avec "library(devtools)" puis utiliser la commande "install_github()

# Pour charger une librairies on utilise la commande "library()" et "search()" ou "searchpaths()" pour
# connaitre la liste des librairies qui sont chargées.

# Pour mettre à jour les paquets il faut aller dans la fenetre en bas a droite et cliquer sur Update.

# Exercice

install.packages("tidyverse")
install.packages("FactoMineR")
install.packages("BiocManager")
BiocManager::install()
BiocManager::install("mixOmics")
install.packages("corrplot")
install.packages("gridExtra")
install.packages("plotly")
install.packages("questionr")

library("tidyverse")
?rnorm





