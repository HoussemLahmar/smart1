import requests
import csv

API_URL = "http://localhost:5000/api"

def get_zones():
    return requests.get(f"{API_URL}/zones").json()

def get_history(zone):
    return requests.get(f"{API_URL}/zone/{zone}").json()

def export_zone_csv(zone, filename):
    data = get_history(zone)
    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Capteur","Valeur","Horodatage"])
        for row in data:
            writer.writerow([row["capteur"], row["valeur"], row["timestamp"]])

def set_mode(zone, mode):
    requests.post(f"{API_URL}/zone/{zone}/mode", json={"mode":mode})

def get_plan(zone):
    return requests.get(f"{API_URL}/zone/{zone}/plan").json()

def add_plan(zone, plan):
    requests.post(f"{API_URL}/zone/{zone}/plan", json=plan)

def delete_plan(zone, plan_id):
    requests.delete(f"{API_URL}/zone/{zone}/plan/{plan_id}")