# Système d'Irrigation Automatisée Multi-Zones

---

## Sommaire

- [Description générale](#description-générale)
- [Architecture](#architecture)
- [Blocs du projet](#blocs-du-projet)
- [Installation & Déploiement](#installation--déploiement)
- [Configuration](#configuration)
- [Utilisation du système](#utilisation-du-système)
- [Documentation technique](#documentation-technique)
    - [Firmware ESP](#firmware-esp)
    - [Infrastructure Serveur](#infrastructure-serveur)
    - [Frontend Dashboard React](#frontend-dashboard-react)
    - [Bloc Intelligence Artificielle](#bloc-intelligence-artificielle)
- [Export CSV](#export-csv)
- [FAQ & Dépannage](#faq--dépannage)
- [Annexes](#annexes)

---

## Description générale

Ce projet a pour but de piloter et surveiller à distance l’irrigation de plusieurs zones agricoles à l’aide de modules ESP32, d’un serveur central (Flask/Python), d’un dashboard web React et d’une intelligence artificielle pour l’aide à la décision.

---

## Architecture

```
[Capteurs/LDR/DHT22] <--> [ESP32] <==WiFi==> [Serveur MQTT/Flask/DB] <==REST==> [Dashboard React]
                                                             |
                                                         [ML]
```

- **ESP32** : Mesure capteurs, commande pompes, expose API REST locale, publie MQTT.
- **Serveur** : Reçoit MQTT, stocke mesures, expose API Flask, export CSV, IA.
- **Dashboard React** : Affiche états, permet irrigation manuelle/automatique, exporte CSV.
- **IA** : Modèle ML, prédiction ouverture pompe à partir des mesures.

---

## Blocs du projet

1. **Firmware ESP** : `main.cpp`, `config.h`
2. **Configuration** : `config.h`, `config.yaml`, `.env`
3. **Infrastructure Serveur** : `mqtt_broker.py`, `database.py`, `app.py`
4. **Dashboard Frontend** : `src/` (ReactJS)
5. **Intelligence Artificielle** : `train_model.py`, `model_converter.py`, `datasets/`

---

## Installation & Déploiement

### 1. Prérequis

- **Backend** : Python 3.9+, `pip install -r requirements.txt`
- **MQTT Broker** : Mosquitto ou équivalent (`sudo apt install mosquitto`)
- **Frontend** : Node.js 18+, npm
- **Firmware** : [PlatformIO](https://platformio.org/) ou Arduino IDE
- **Base de données** : SQLite (par défaut, incluse)
- **(Optionnel) IA** : scikit-learn, joblib, skl2onnx

### 2. ESP32 Firmware

- Cloner le dépôt, ouvrir `main.cpp` dans PlatformIO/VSCode.
- Adapter `config.h` (SSID WiFi, mot de passe, topics MQTT, adresses IP).
- Flasher chaque ESP32 :
    ```
    platformio run -t upload
    ```
- Redémarrer, vérifier le port série ou la page `/api/status` locale.

### 3. Serveur (MQTT + Flask)

- Installer les dépendances Python :
    ```
    pip install -r requirements.txt
    ```
- Lancer le broker MQTT (exemple avec Mosquitto) :
    ```
    mosquitto -c mosquitto.conf
    ```
- Démarrer le backend :
    ```
    python mqtt_broker.py  # Ecoute et stocke les mesures
    python app.py          # Expose l’API Flask (par défaut sur :5000)
    ```
- Les variables de config sont dans `config.yaml` ou `.env` (voir section Configuration).

### 4. Frontend React

- Dans `dashboard/web/` ou `frontend/` :
    ```
    npm install
    npm run build   # pour production
    npm start       # pour développement (proxy sur API Flask)
    ```
- Accéder à `http://localhost:3000/` (ou l’URL déployée).

### 5. Bloc IA

- Placer vos jeux de données dans `datasets/`.
- Entraîner le modèle :
    ```
    python train_model.py
    ```
- (Optionnel) Convertir le modèle :
    ```
    python model_converter.py
    ```

---

## Configuration

- **Firmware ESP** : `config.h` (SSID, mot de passe, topics MQTT, endpoints REST)
- **Serveur** : `config.yaml`, `.env` (chemin DB, port MQTT, port Flask, etc.)
- **Frontend** : `.env` pour la variable d’URL de l’API centrale (`REACT_APP_API_URL`)

---

## Utilisation du système

### Dashboard

- **Vue globale** : Statut de chaque zone (température, humidité, lumière, pompe)
- **Irrigation manuelle** : Bouton "Irriguer" par zone ou globalement (central/local)
- **Export CSV** : Télécharger les mesures en temps réel ou l’historique (par zone)
- **Historique** : Accès aux mesures passées, visualisation graphique (optionnel)
- **Aide IA** : Affichage de la prédiction ML (optionnel)

### API REST

- `/zones/<zone>/status` : Statut d’une zone (temp/humidité/ldr/pump)
- `/zones/all_status` : Statut de toutes les zones
- `/export_csv` : Export CSV des mesures actuelles
- `/export_csv_history` : Historique complet (CSV)
- `/export_csv_history_zone/<zone>` : Historique d’une zone (CSV)

### Commandes depuis le dashboard

- Irrigation centrale : `/api/irrigate`
- Irrigation locale directe : `/api/direct_irrigate`

---

## Documentation technique

### Firmware ESP

- **main.cpp** :
    - Boucle de mesure toutes les N secondes, stockage en RAM ou SD, publication MQTT
    - Serveur HTTP REST sur `/api/status`, `/api/irrigate`, `/api/history_csv`
    - Gestion du buffer d’historique (voir Bloc 1 & 2)
- **config.h** :
    - Paramètres réseau, topics MQTT, pins, durée irrigation

### Infrastructure Serveur

- **mqtt_broker.py** :
    - Abonnement aux topics MQTT
    - Décodage, validation, insertion en base de données (SQLite)
- **database.py** :
    - Fonctions d’insertion, de récupération temps réel et historique
    - Tables : `mesures(timestamp, zone, capteur, valeur)`
- **app.py** :
    - Endpoints Flask REST
    - Génération CSV (temps réel, historique global ou par zone)
    - Sécurisation possible (authentification, CORS)
    - Voir docstring dans chaque fichier pour la description détaillée des fonctions

### Frontend Dashboard React

- **src/App.jsx** : Routing principal, intégration composants
- **src/components/GlobalIrrigation.jsx** : Commande globale (central/local)
- **src/components/ZoneTable.jsx** : Tableau multi-zones, Refresh auto
- **src/components/ZoneDetail.jsx** : Détail zone, historique, export
- **src/components/ExportCSVButton.jsx** : Boutons de téléchargement CSV

### Bloc Intelligence Artificielle

- **train_model.py** : Entraîne un modèle scikit-learn sur les historiques (RandomForest par défaut)
- **model_converter.py** : Conversion du modèle vers ONNX (pour embarqué ou microservices)
- **datasets/** : Jeus de données historiques (CSV), à placer ici

---

## Export CSV

- **Toutes zones, temps réel** : `/export_csv` (1 ligne/zone, dernier état)
- **Historique complet** : `/export_csv_history` (toutes mesures)
- **Historique d’une zone** : `/export_csv_history_zone/<zone>`
- **Historique local (ESP)** : `http://IP_ESP/api/history_csv?zone=zone7` (par zone, buffer RAM ou SD)

---

## FAQ & Dépannage

- **Problème connexion MQTT** : Vérifier broker en marche, adresses IP, ports, firewall
- **Pas de mesures en base** : Vérifier topics MQTT, logs du broker et du script
- **Dashboard React ne s’affiche pas** : Vérifier `npm start`, port 3000, config proxy
- **Export CSV vide** : Vérifier que des données existent en base (`sqlite3 irrigation.db`)

---

## Annexes

- **Schémas de câblage** : Voir dossier `docs/` ou README Bloc 1
- **Exemples d’API** : Utiliser `curl` ou Postman pour tester les endpoints
- **Datasets** : Format CSV, séparateur `;`

---

## Contact & Contributions

Pour toute question, bug ou amélioration, merci d’ouvrir une issue sur le dépôt GitHub ou de contacter l’équipe projet.