import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import train_test_split
import joblib
import os

# Chargement du jeu de données
DATASET_PATH = os.getenv("DATASET_PATH", "datasets/zones_mesures.csv")
df = pd.read_csv(DATASET_PATH, sep=";")

# Suppose que les colonnes sont : timestamp,zone,temperature,humidite,ldr1,ldr2,pump
# ! Adapter cette partie à la structure réelle de vos données !
feature_cols = ["temperature", "humidite", "ldr1", "ldr2"]
target_col = "pump"

# Ne garder que les colonnes utiles et dropper les na
df = df[feature_cols + [target_col]].dropna()

# Split jeu de données
X = df[feature_cols]
y = df[target_col]

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Apprentissage du modèle
model = RandomForestRegressor(n_estimators=100, random_state=42)
model.fit(X_train, y_train)

# Évaluation rapide
score = model.score(X_test, y_test)
print(f"Score R2 sur test : {score:.3f}")

# Sauvegarde du modèle
joblib.dump(model, "zone_pump_model.pkl")
print("Modèle entraîné et sauvegardé sous zone_pump_model.pkl")