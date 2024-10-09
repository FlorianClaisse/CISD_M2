# concater une chaine 
paste("Bonjour", "a", "tous", sep="_")
# Connaitre la taille d'une chaine
nchar("Coucou")
# Recuperer une sous chaine ou modifier a un endroit precis
test = "Bonjour"
substr(test, start=2, stop=4)
substr(test, start=2, stop=4) = "xxxxxx"
test
# Spliter une chaine de caractere
strsplit("test", split="s")
# Rechercher un element dans une liste
grep(pattern="o", c("bonjour", "toto"), value=FALSE)
grep(pattern="o", c("bonjour", "toto"), value=TRUE)
# Remplacer un pattern dans un chaine 
gsub(pattern="o", replacement="@", "bonjour", fixed=TRUE)