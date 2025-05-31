import yaml

@app.route("/api/schedule/<zone>", methods=["GET", "POST"])
def schedule(zone):
    path = "irrigation_schedule.yaml"
    with open(path) as f:
        sched = yaml.safe_load(f)
    if request.method == "GET":
        return jsonify(sched.get(zone, []))
    else:
        sched[zone] = request.json  # tableau des créneaux
        with open(path, "w") as f:
            yaml.dump(sched, f)
        return jsonify({"ok": True})