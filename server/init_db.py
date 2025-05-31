"""
Script pour initialiser la base de données du serveur d'irrigation intelligente.
- Crée la table SQLite si besoin
- Vérifie la connexion InfluxDB et la création du bucket
Usage :
    python init_db.py
"""

import yaml
import sys

with open("config.yaml") as f:
    cfg = yaml.safe_load(f)

db_type = cfg['database']['type']

if db_type == "sqlite":
    import sqlite3
    db_path = cfg['database']['sqlite_path']
    conn = sqlite3.connect(db_path)
    c = conn.cursor()
    c.execute("""CREATE TABLE IF NOT EXISTS mesures (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
        zone TEXT,
        capteur TEXT,
        type TEXT,
        valeur REAL
    )""")
    conn.commit()
    conn.close()
    print(f"[OK] Base SQLite initialisée : {db_path}")

elif db_type == "influxdb":
    from influxdb_client import InfluxDBClient
    from influxdb_client.client.buckets_api import BucketsApi

    url = cfg['database']['influxdb_url']
    token = cfg['database']['influxdb_token']
    org = cfg['database']['influxdb_org']
    bucket = cfg['database']['influxdb_bucket']

    client = InfluxDBClient(url=url, token=token, org=org)
    buckets_api = client.buckets_api()

    buckets = [b.name for b in buckets_api.find_buckets().buckets]
    if bucket not in buckets:
        buckets_api.create_bucket(bucket_name=bucket, org=org)
        print(f"[OK] Bucket InfluxDB créé : {bucket}")
    else:
        print(f"[INFO] Bucket InfluxDB déjà présent : {bucket}")

    client.close()
else:
    print("Type de base de données non supporté dans config.yaml :", db_type)
    sys.exit(1)