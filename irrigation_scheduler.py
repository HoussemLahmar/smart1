import yaml
import time
import requests
from datetime import datetime, timedelta

API_URL = "http://localhost:5000/api/irrigate"
SCHEDULE_PATH = "irrigation_schedule.yaml"
CHECK_INTERVAL = 60  # secondes

def load_schedule():
    with open(SCHEDULE_PATH) as f:
        return yaml.safe_load(f)

def in_slot(now, slot):
    day = now.isoweekday()
    if day not in slot["days"]:
        return False
    start = datetime.strptime(slot["start"], "%H:%M").time()
    end = datetime.strptime(slot["end"], "%H:%M").time()
    return start <= now.time() < end

triggered = set()  # (zone, date, start)

while True:
    now = datetime.now()
    today_str = now.strftime("%Y-%m-%d")
    schedule = load_schedule()
    for zone, slots in schedule.items():
        for slot in slots:
            key = (zone, today_str, slot["start"])
            if in_slot(now, slot) and key not in triggered:
                # Calcul durée en minutes
                s = datetime.strptime(slot["start"], "%H:%M")
                e = datetime.strptime(slot["end"], "%H:%M")
                minutes = int((e - s).total_seconds() // 60)
                print(f"[{now}] Irrigation auto {zone} : {slot['start']}-{slot['end']} ({minutes} min)")
                try:
                    requests.post(API_URL, json={"zone": zone, "minutes": minutes})
                except Exception as ex:
                    print(f"Erreur irrigation : {ex}")
                triggered.add(key)
            # Reset pour le prochain jour
            if now.time() < datetime.strptime(slot["start"], "%H:%M").time():
                triggered.discard(key)
    time.sleep(CHECK_INTERVAL)