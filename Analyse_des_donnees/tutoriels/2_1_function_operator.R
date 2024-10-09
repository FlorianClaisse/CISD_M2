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





