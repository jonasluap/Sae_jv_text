# Jeu textuel narratif en C

## Présentation
Ce projet est un **jeu narratif textuel en langage C**, développé par **LUAP Jonas** et **EL BAZTAMI Manal**.  
Le joueur incarne un personnage confronté à une situation critique et doit faire des choix qui influencent le déroulement de l’histoire.

Le jeu est jouable en **français** et en **anglais**, propose **plusieurs embranchements**, une **carte évolutive**, ainsi qu’un **système de sauvegarde automatique**.

---

## Fonctionnalités principales

###  Gameplay
- Jeu **100 % textuel** dans le terminal
- Choix multiples influençant l’histoire
- Plusieurs fins possibles selon les décisions du joueur
- Messages personnalisés avec le nom du joueur

###  Langues
- Français (`fr`)
- Anglais (`gb`)

La langue est sélectionnée au lancement du jeu.

---

##  Système de sauvegarde

Le jeu intègre une **sauvegarde automatique** :
- La progression est enregistrée dans le fichier `sauvegarde.txt`
- Au lancement du programme, une sauvegarde existante est détectée
- Le joueur peut :
  - Continuer la partie sauvegardée
  - Ou recommencer une nouvelle partie

### Données sauvegardées
- Nom du joueur
- Langue choisie
- Branche narrative actuelle
- Sous-branche
- Dernier choix effectué
- Étape actuelle de la carte

La sauvegarde est **supprimée automatiquement à la fin du jeu**.

---

##  Carte progressive

Le jeu dispose d’un **système de carte ASCII évolutive** :
- Les cartes sont stockées dans le dossier `cartes/`
- Une carte différente est affichée selon l’avancement du joueur
- Les cartes sont affichées :
  - Avant un choix important
  - Après certains événements clés


##  Aléatoire et événements imprévus

Le jeu utilise la génération aléatoire (`rand()`) pour :
- Déclencher des **pièges aléatoires** (alarme, blessure, poison…)
- Ajouter de l’imprévisibilité lors de certains mauvais choix

Ces événements n’arrêtent pas immédiatement la partie mais renforcent la tension narrative.

---

##  Organisation des fichiers

```
.
├── main.c              # Code source principal
├── README.md           # Documentation
├── sauvegarde.txt      # Fichier de sauvegarde (créé automatiquement)
├── histoire/           # Fichiers texte de l’histoire
│   ├── 1intro_fr.txt
│   ├── 1intro_gb.txt
│   └── ...
├── cartes/             # Cartes ASCII évolutives
│   ├── carte_etape1_fr.txt
│   ├── carte_etape2_fr.txt
│   └── ...
```

---

##  Compilation avec GCC

###  Prérequis
- Un compilateur **GCC**
- Un terminal (CMD, PowerShell, Terminal Linux)

---

###  Compilation et exécution sous Linux

1. Ouvrir un terminal dans le dossier du projet
2. Compiler le programme :

```bash
gcc main.c -o jeu
```

3. Lancer le jeu :

```bash
./jeu
```

---

###  Compilation et exécution sous Windows

#### Option 1 : avec MinGW

1. Installer **MinGW** ou **MSYS2** (GCC pour Windows)
2. Ouvrir le terminal MinGW
3. Se placer dans le dossier du projet
4. Compiler :

```bash
gcc main.c -o jeu.exe
```

5. Exécuter :

```bash
jeu.exe
```

#### Option 2 : avec WSL (Windows Subsystem for Linux)

- Utiliser exactement les mêmes commandes que sous Linux

---

##  Problèmes courants

- **Le jeu ne trouve pas les fichiers texte** :
  - Vérifiez que les dossiers `histoire/` et `cartes/` sont bien présents
  - Respectez les noms de fichiers

- **Problème d’accents** :
  - Utilisez un terminal compatible UTF-8

---

##  Améliorations possibles

- Ajout de nouvelles branches narratives
- Plus de langues
- Sauvegardes multiples
- Interface graphique

---

##  Auteurs

- **LUAP Jonas**
- **EL BAZTAMI Manal**

---

##  Licence

Projet pédagogique – usage scolaire / académique.

