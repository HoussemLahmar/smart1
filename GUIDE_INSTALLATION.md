# Guide d’installation et d’exécution – Système d’Irrigation Automatisée Desktop

---

## 1. Prérequis

### Système d’exploitation
- **Windows 10/11**, **Linux** ou **macOS**

### Outils nécessaires
- **Node.js** (v18 ou +)
- **Python** (3.9 ou +)
- **Git** (optionnel)

---

## 2. Récupération du projet

```sh
git clone https://github.com/[TON_ORG_OU_USER]/[TON_REPO].git
cd [TON_REPO]
```
ou Télécharger ZIP et extraire.

---

## 3. Installation backend

```sh
cd backend
python -m venv venv
venv\Scripts\activate       # (Windows) OU source venv/bin/activate (Linux/macOS)
pip install -r requirements.txt
```

---

## 4. Installation frontend

```sh
cd ../frontend
npm install
npm run build
```

---

## 5. Packager le backend Flask

```sh
pip install pyinstaller
pyinstaller --onefile app.py
# Place dist/app.exe à la racine /dist
```

---

## 6. Installer Electron

```sh
cd ..
npm install --save-dev electron electron-builder
```

---

## 7. Lancer en dev

```sh
npm start
```

## 8. Générer l’exécutable desktop

```sh
npx electron-builder
```

---

## 9. Utiliser l’application

- Double-cliquer sur l’exécutable généré dans `dist/`
- L’interface s’ouvre, tout fonctionne en local.

---

## 10. Maintenance

- Modifier les horaires via le dashboard ou `backend/irrigation_schedule.yaml`
- Pour plus de détails, voir README.md