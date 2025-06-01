import csv
def export_zone_to_csv(zone_data, filename):
    with open(filename, "w", newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["Capteur", "Valeur", "Horodatage"])
        for row in zone_data:
            writer.writerow(row)