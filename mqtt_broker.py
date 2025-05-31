import paho.mqtt.client as mqtt
import json
from database import insert_measure

MQTT_BROKER = "localhost"
MQTT_PORT = 1883
ZONES = ["zone1", "zone2", "zone3", "zone4", "zone5", "zone6", "zone7", "zone8"]

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker")
    # S'abonner à chaque topic de capteurs
    for i, zone in enumerate(ZONES):
        capteur = f"lse01_{i+1}"
        client.subscribe(f"oliveraie/capteurs/{capteur}/#")

def on_message(client, userdata, msg):
    topic = msg.topic
    try:
        payload = msg.payload.decode()
        value = float(payload)
    except Exception:
        print(f"[MQTT] Payload non numérique: {msg.payload}")
        return

    # Ex : topic = oliveraie/capteurs/lse01_1/temperature ou .../ldr1
    parts = topic.split("/")
    if len(parts) == 4:
        zone_num = int(parts[2][6:])  # lse01_1 → 1
        zone = f"zone{zone_num}"
        capteur_type = parts[3]  # e.g. temperature, ldr1, ldr2
        # On stocke la mesure
        insert_measure(zone, capteur_type, value)

def main():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_forever()

if __name__ == "__main__":
    main()