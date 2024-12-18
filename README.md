TP de Synthèse - Micro-Shell ENSEASH

Introduction

Dans le cadre de notre TP de synthèse intitulé "ENSEA in the Shell", nous avons développé un micro-shell en C. Ce shell simplifié, que nous avons nommé enseash, est capable d'exécuter des commandes simples, d'afficher les codes de retour des programmes lancés, et de gérer proprement leur terminaison, qu'elle soit normale ou due à un signal.

1. Message d'accueil et affichage du prompt

Pour commencer, nous avons ajouté un message d'accueil qui apparaît lorsque le shell est lancé, suivi du prompt enseash% permettant à l'utilisateur de saisir des commandes.
L'affichage est réalisé avec la fonction write, conformément aux consignes, et le prompt est réaffiché à chaque itération.

