import sqlite3
from datetime import datetime

DB_PATH = "irrigation.db"

def get_db():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn

def init_db():
    conn = get_db()
    cur = conn.cursor()
    cur.execute("""
        CREATE TABLE IF NOT EXISTS mesures (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT,
            zone TEXT,
            capteur TEXT,
            valeur REAL
        )
    """)
    conn.commit()
    conn.close()

def insert_measure(zone, capteur, valeur):
    conn = get_db()
    cur = conn.cursor()
    cur.execute(
        "INSERT INTO mesures (timestamp, zone, capteur, valeur) VALUES (?, ?, ?, ?)",
        (datetime.now().isoformat(), zone, capteur, valeur)
    )
    conn.commit()
    conn.close()

def get_last_measures():
    """Retourne la dernière mesure de chaque type/capteur par zone."""
    conn = get_db()
    cur = conn.cursor()
    result = {}
    for zone in [f"zone{i}" for i in range(1,9)]:
        result[zone] = {}
        for capteur in ["temperature", "humidite", "ldr1", "ldr2", "pump"]:
            cur.execute("""
                SELECT valeur, timestamp FROM mesures
                WHERE zone=? AND capteur=?
                ORDER BY timestamp DESC LIMIT 1
            """, (zone, capteur))
            row = cur.fetchone()
            if row:
                result[zone][capteur] = {"valeur": row["valeur"], "timestamp": row["timestamp"]}
    conn.close()
    return result

def get_zone_history(zone):
    conn = get_db()
    cur = conn.cursor()
    cur.execute(
        "SELECT * FROM mesures WHERE zone=? ORDER BY timestamp ASC", (zone,)
    )
    rows = cur.fetchall()
    conn.close()
    return rows

def get_all_history():
    conn = get_db()
    cur = conn.cursor()
    cur.execute(
        "SELECT * FROM mesures ORDER BY timestamp ASC"
    )
    rows = cur.fetchall()
    conn.close()
    return rows

# À l'init
if __name__ == "__main__":
    init_db()