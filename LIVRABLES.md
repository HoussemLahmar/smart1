# 7. Livrables Attendus

## Pour Chaque Bloc

- ✅ **Code source complet**
  - Tous les fichiers `.c`, `.cpp`, `.py`, `.js`, `.jsx`, `.sh`, etc. nécessaires au fonctionnement du bloc.

- ✅ **Fichiers de configuration**
  - Exemples : `config.h`, `config.json`, `config.yaml`, `.env`, `settings.py`, etc.
  - Fournis avec des valeurs d’exemple et commentaires si possible.

- ✅ **Instructions de compilation et de déploiement**
  - Fichier `INSTALL.md` ou section dédiée dans le `README.md` pour chaque bloc.
  - Précise :
    - Prérequis (outils, dépendances, versions)
    - Commandes pour compiler (ex : `make`, `gcc`, `npm install`, `pip install`)
    - Commandes pour lancer/déployer (ex : `python app.py`, `./flash.sh`, `docker-compose up`, etc.)
    - Pour le front : build et lancement (`npm run build`, `npm start`, ...)

- ✅ **Documentation technique des fonctions critiques**
  - Fichier `README.md` ou `docs/` dédié à chaque bloc.
  - Doit contenir :
    - Description générale de l’architecture du bloc.
    - Explication des fonctions/modules critiques :
      - Leur rôle
      - Points d’entrée/sortie
      - Algorithme principal (schéma/bref pseudo-code si besoin)
      - Pièges ou points clés (ex : gestion erreurs, sécurité, persistance, perf)

---

## Exemple de structure d’un bloc (Bloc 3 : Infrastructure Serveur)

```
bloc3_infra_serveur/
├── app.py
├── mqtt_broker.py
├── database.py
├── config.yaml
├── requirements.txt
├── README.md
└── INSTALL.md
```

- **README.md** : documentation technique sur l’API, la base, les fonctions MQTT/etc.
- **INSTALL.md** : instructions d’installation, lancement, variables d’environnement, exemples de commandes.

---

## Exemple de structure d’un bloc (Bloc 1 : Firmware ESP)

```
bloc1_esp_firmware/
├── main.cpp
├── config.h
├── platformio.ini
├── README.md
└── INSTALL.md
```

- **README.md** : schéma de l’architecture, explication du flux capteur→MQTT, points critiques (interruptions, timers, etc.)
- **INSTALL.md** : comment flasher l’ESP, choix de la carte, dépendances PlatformIO/Arduino.

---

## Astuces

- Si plusieurs blocs partagent des dépendances ou des configs, bien séparer les fichiers ou utiliser des sous-dossiers.
- Pour la documentation technique : commenter les fonctions directement dans le code ET résumer dans le `README.md` les plus critiques.
- Fournir un `Makefile` ou script d’automatisation si pertinent.

---

## Pour aller plus loin

- Ajouter des exemples d’utilisation (appels API, captures d’écrans dashboard, logs de tests…)
- Versionner les fichiers de config (`config.example.yaml`, etc.)
- Si besoin : fournir des dumps de base ou datasets d’exemple dans un dossier `datasets/`

---