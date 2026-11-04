# Gestion d'un lieu culturel (C++17)

**Compiler** (g++ ≥ 9 ou clang, make ; sous Windows : WSL ou MSYS2) :

    make

**Lancer** (le fichier est relu au démarrage et réécrit après chaque modification) :

    cp data/test_data.txt ma_prog.txt
    ./lieu ma_prog.txt          # sans argument : lieu_data.txt (créé s'il n'existe pas)

**Code administrateur** : `SCEN` (modifiable dans `src/Config.hpp`).

**Démonstration automatique** des comportements demandés : `bash tests/demo.sh`
(conflit de salle, réservation excessive, mauvais code, sauvegarde + redémarrage, fichier incohérent).

**Fichier incohérent** : `./lieu data/test_data_corrompu.txt` → message `fichier:ligne: problème`, code de sortie 2.

**Nouveau type (atelier)** : `cp ajout_atelier/Workshop.* src/ && make` (aucun fichier existant à modifier).
Jeu de test : `ajout_atelier/test_data_atelier.txt`.

Terminal en UTF-8 recommandé (accents).
