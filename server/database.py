import yaml
import os
from influxdb_client import InfluxDBClient, Point
import sqlite3

with open("config.yaml") as f:
    cfg = yaml.safe_load(f)
db_type = cfg['database']['type']

class InfluxDBHandler:
    def __init__(self, url, token, org, bucket):
        self.client = InfluxDBClient(url=url, token=token, org=org)
        self.write_api = self.client.write_api()
        self.bucket = bucket

    def insert_measurement(self, zone, data: dict):
        point = Point(zone)
        for k, v in data.items():
            point.field(k, v)
        self.write_api.write(bucket=self.bucket, record=point)

    def query(self, flux):
        return self.client.query_api().query(flux)

class SQLiteHandler:
    def __init__(self, db_path):
        self.conn = sqlite3.connect(db_path, check_same_thread=False)
        self.create_table()

    def create_table(self):
        c = self.conn.cursor()
        c.execute("""CREATE TABLE IF NOT EXISTS mesures (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            zone TEXT,
            capteur TEXT,
            type TEXT,
            valeur REAL
        )""")
        self.conn.commit()

    def insert_measurement(self, zone, data: dict):
        c = self.conn.cursor()
        for k, v in data.items():
            if k == 'capteur': continue
            c.execute("INSERT INTO mesures (zone, capteur, type, valeur) VALUES (?, ?, ?, ?)",
                      (zone, data.get('capteur', ''), k, v))
        self.conn.commit()

    def query(self, sql):
        c = self.conn.cursor()
        c.execute(sql)
        return c.fetchall()

if db_type == "influxdb":
    db = InfluxDBHandler(
        url=cfg['database']['influxdb_url'],
        token=cfg['database']['influxdb_token'],
        org=cfg['database']['influxdb_org'],
        bucket=cfg['database']['influxdb_bucket']
    )
else:
    db = SQLiteHandler(cfg['database']['sqlite_path'])