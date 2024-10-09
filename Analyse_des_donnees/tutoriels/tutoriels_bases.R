# les differents type de données sont null, logical, numeric, complex, character
# les classes d'objets courantes sont: vector, matrix, array, factor, data.frame, list

# Pour connaitre la classe d'un objet on utilise "class()" et pour la nature des elements le composant "str()"

a = 5 # Stocker une valeur
ls() # Lister toutes les variables disponible
rm(a) # supprimer une variable 
rm(list=ls()) # supprimer toutes les variables disponible

# Operations
2 + 2
exp(10)
a = log(2)
b = cos(10)
a - b
a * b
d = 2 < 3
dd = FALSE
dd - d
dd + d
e = "toto"
class(e)
str(e)


# is et as
is.numeric(a)
is.complex(a)
as.character(a)
as.list(e)

# Arrondir
floor(a)
ceiling(a)
round(a, digits=2)
signif(a+1, digits=2)

# Boolean et operateurs logique
# >, >=, <, <=, !=, ==, &, |

# Creer un vecteur
c(1, 2, 3, 4, 5, 6)
# Creer un vecteur avec une range
seq(1, 6, 1)
seq(1, 20, length=5)
# ou plus rapidement avec 
d = 1:6
# Creer un vecteur avec x, repete n fois
rep(2, 15)
rep(c(1, 2), 3)
rep(c(1, 2), each=3)
# Extraire des elements
d[2]
d[2:3]
d[c(1, 3, 5)]
d[-3]
d[-(1:2)]
# Operateur
d + 4
d - 4
d * 4
d / 4
d + c(6, 5, 4, 3, 2, 1)
# Fonction usuelles
length(d)
sum(d)
cumsum(d) # sum cumule
diff(d) # difference successive
t(d) # Transposer
t(d) %% 4 # produit scalaire
d[3] = NA # donnée manquante
is.na(d)
any(is.na(d))
all(is.na(d))
abs(d)
sort(d)
order(d, decreasing=FALSE) # sort les indices pour rearanger a vecteur
d == 4 # Savoir si une valeur est contenue dans le vecteur
encore = c(a=12, b=23, c=24, d=45)
names(encore)
encore["a"]
encore > 12
encore[encore > 12]
which(encore > 12)
cos(encore)
unique(encore)





