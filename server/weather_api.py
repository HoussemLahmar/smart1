import requests
import yaml

with open("config.yaml") as f:
    cfg = yaml.safe_load(f)

def get_forecast():
    lat = cfg['weather']['lat']
    lon = cfg['weather']['lon']
    url = f"https://api.open-meteo.com/v1/forecast?latitude={lat}&longitude={lon}&hourly=precipitation,temperature_2m&forecast_days=1"
    try:
        resp = requests.get(url, timeout=5)
        data = resp.json()
        precip = data['hourly']['precipitation'][:6]
        return {
            "precip_next_6h": sum(precip),
            "temp_now": data['hourly']['temperature_2m'][0]
        }
    except Exception as e:
        print(f"[Weather] Erreur : {e}")
        return None