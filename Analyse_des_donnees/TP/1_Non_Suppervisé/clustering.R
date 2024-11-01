library("FactoMineR")

# Données sur les fromages
X <- read.table("../Data/fromage.txt", sep="", header=TRUE, row.names=1)
n <- nrow(X)
p <- ncol(X)