from database import db

def compute_recommendation(zone):
    res = db.query(f'SELECT valeur FROM mesures WHERE zone="{zone}" AND type="humidite" ORDER BY timestamp DESC LIMIT 1')
    humidite = res[0][0] if res else None
    if humidite is None:
        return {"irriguer": False, "duree": 0, "motif": "Pas de mesure"}
    if humidite < 30:
        return {"irriguer": True, "duree": 30, "motif": "Humidité basse"}
    return {"irriguer": False, "duree": 0, "motif": "OK"}