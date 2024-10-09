# x11() Unix, windows() windows, quartz() Mac OS
quartz() # Afficher ma fenetre graphique
dev.list() # Afficher les fenetres graphique d'ouverte
# split.screen(c(1, 2)) # Spliter une fenetre en sous fenetre que l'on selectionne avec "screen(1)" et screen(2)
# plot(x=c(1:20)) # Affiche un graphique avec des valeurs x sur y ordonnées sur l'axe des x
# plot(x=c(1:20), y=c(11:30)) # Affiche un graphique de sur l'axe x et de y sur l'axe y
# pie(x=) # Affiche un graphique en camenbert
# boxplot(x) # Affiche un graphique en boite a moustache
# pairs(x) # Si x est une matrice ou un tableau de dibbéesn dessube tous les graphes bivariés entre les colonnes de x
# hist(x) # Histogramme des frequences de x
# barplot(x) # Histogramme des valeurs de x
# contour(x, y, z) # Courbes de niveau (les données sont interpolées pour tracer les courbes), x et y doivent être des vecteurs et z une matrice telle que dim(z) = c(length(x), length(y))
# image(x, y, z) # Idem mais les données sont représentées avec des couleurs
#persp(x, y, z) # Idem mais en perspective
dev.off() # Fermer une fenetre graphique
