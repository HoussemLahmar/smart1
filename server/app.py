from flask import Flask, request, jsonify
from database import db
from weather_api import get_forecast
import yaml

with open("config.yaml") as f:
    cfg = yaml.safe_load(f)

app = Flask(__name__)

@app.route("/")
def index():
    return "Smart Irrigation Server API"

@app.route("/api/zones/<zone>/status")
def zone_status(zone):
    if cfg['database']['type'] == "influxdb":
        flux = f'from(bucket: "{cfg["database"]["influxdb_bucket"]}") |> range(start: -1h) |> filter(fn: (r) => r._measurement == "{zone}") |> last()'
        tables = db.query(flux)
        res = []
        for table in tables:
            for record in table.records:
                res.append({"field": record.get_field(), "value": record.get_value(), "time": str(record.get_time())})
        return jsonify(res)
    else:
        res = db.query(f'SELECT type, valeur, timestamp FROM mesures WHERE zone="{zone}" ORDER BY timestamp DESC LIMIT 5')
        return jsonify([{"type": r[0], "valeur": r[1], "timestamp": r[2]} for r in res])

@app.route("/api/forecast")
def forecast():
    return jsonify(get_forecast())

@app.route("/api/irrigate", methods=["POST"])
def irrigate():
    data = request.json
    import paho.mqtt.publish as publish
    zone = data.get("zone")
    duration = data.get("minutes")
    topic = f"oliveraie/zones/{zone}/irrigation/start"
    publish.single(topic, payload=str(duration), hostname=cfg['mqtt']['broker'],
                   auth={"username": cfg['mqtt']['user'], "password": cfg['mqtt']['password']})
    return jsonify({"ok": True, "msg": f"Irrigation {zone} {duration}min"})

if __name__ == "__main__":
    from mqtt_broker import mqtt_loop
    mqtt_loop()
    app.run(host="0.0.0.0", port=5000, debug=True)