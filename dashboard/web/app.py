from flask import Response

@app.route("/export_csv_history_zone/<zone>")
def export_csv_history_zone(zone):
    def generate():
        with open("logs.csv", "r") as f:
            header = f.readline()
            yield header  # toujours la ligne d'en-tête
            for line in f:
                # On suppose que la colonne 2 est le nom de la zone
                # Format: timestamp;zone;temperature;humidite;ldr1;ldr2;pompe
                if f";{zone};" in line:
                    yield line
    return Response(generate(), mimetype="text/csv",
                    headers={"Content-Disposition": f"attachment;filename=historique_{zone}.csv"})