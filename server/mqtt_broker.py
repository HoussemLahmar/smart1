import yaml
import paho.mqtt.client as mqtt
import threading
from database import db

with open("config.yaml") as f:
    cfg = yaml.safe_load(f)

def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode()
    print(f"[MQTT] {topic}: {payload}")

    if topic.startswith("oliveraie/capteurs/lse01_"):
        parts = topic.split('/')
        capteur = parts[2]
        zone = "zone" + capteur.split('_')[1]  # lse01_1 -> zone1
        typ = parts[3]
        try:
            val = float(payload)
            db.insert_measurement(zone, {"capteur": capteur, typ: val})
        except ValueError:
            print("[WARN] Payload non numérique ignoré.")

def mqtt_loop():
    client = mqtt.Client()
    client.username_pw_set(cfg['mqtt']['user'], cfg['mqtt']['password'])
    client.connect(cfg['mqtt']['broker'], cfg['mqtt']['port'], 60)
    client.subscribe(cfg['mqtt']['topics']['capteurs'])
    client.subscribe(cfg['mqtt']['topics']['zones'])
    client.on_message = on_message
    print("[MQTT] Démarrage boucle MQTT...")

    threading.Thread(target=client.loop_forever, daemon=True).start()