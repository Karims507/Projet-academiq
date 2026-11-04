# Gestion d'un lieu culturel (C++17)

## Prérequis

- CMake 3.16 ou plus récent
- Un compilateur C++17 (par exemple GCC)

## Compiler

Depuis la racine du projet, configurez puis compilez le projet :

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++                                                      
cmake --build build
```

Avec Visual Studio, vous pouvez préciser la configuration :

```powershell
cmake --build build --config Release
```

## Lancer

Sous Windows (configuration par défaut) :

```powershell
Copy-Item data\test_data.txt lieu_data.txt
.\build\Projet-academiq.exe lieu_data.txt
```

Après une compilation Release avec Visual Studio :

```powershell
.\build\Release\Projet-academiq.exe lieu_data.txt
```

Sous Linux ou macOS :

```sh
cp data/test_data.txt lieu_data.txt
./build/Projet-academiq lieu_data.txt
```

L'argument est le chemin du fichier de données. S'il n'est pas fourni,
l'application utilise `lieu_data.txt`. Les modifications sont sauvegardées dans
ce fichier. La copie de l'exemple évite de modifier les fichiers de test suivis
par Git.

Pour vérifier le traitement d'un fichier incohérent, lancez l'application avec
`data/test_data_corrompu.txt` comme argument. Elle affiche l'erreur et se termine
avec le code de sortie 2.

## Configuration et atelier

Le code administrateur par défaut est `Karm` ; il est défini dans
`src/Config.hpp`.

Les fichiers de `ajout_atelier/` ne font pas partie de la cible actuelle.
Pour les compiler avec l'application, ajoutez `ajout_atelier/Workshop.cpp`
à la liste des sources de `add_executable` dans `CMakeLists.txt` et intégrez
le nouveau type au code de l'application.

Un terminal configuré en UTF-8 est recommandé pour afficher correctement les
accents.
Executer ce commande pour configure en UTF-8:
`chcp 65001`
 
