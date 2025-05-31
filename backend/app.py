from flask import Flask, request, jsonify, send_file
import yaml
import os

app = Flask(__name__)
SCHEDULE_PATH = "irrigation_schedule.yaml"

@app.route("/api/schedule/<zone>", methods=["GET", "POST"])
def schedule(zone):
    with open(SCHEDULE_PATH, "r") as f:
        sched = yaml.safe_load(f)
    if request.method == "GET":
        return jsonify(sched.get(zone, []))
    sched[zone] = request.json
    with open(SCHEDULE_PATH, "w") as f:
        yaml.dump(sched, f)
    return jsonify({"ok": True})

@app.route("/api/health")
def health():
    return jsonify({"status": "ok"})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)