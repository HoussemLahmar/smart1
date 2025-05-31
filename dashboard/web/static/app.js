const ZONES = ["zone1", "zone2", "zone3", "zone4", "zone5", "zone6", "zone7", "zone8"];
// ZONE_URLS est rendu dans le template Flask (voir ci-dessous)

function renderZones() {
    const zonesList = document.getElementById("zones-list");
    zonesList.innerHTML = "";
    ZONES.forEach(zone => {
        const col = document.createElement("div");
        col.className = "col-md-6";
        col.innerHTML = `
            <div class="card zone-card" id="card-${zone}">
                <div class="card-header d-flex justify-content-between align-items-center">
                    <strong>${zone.toUpperCase()}</strong>
                    <button class="btn btn-sm btn-outline-success" onclick="irrigateZone('${zone}')">Irriguer (central)</button>
                    <button class="btn btn-sm btn-outline-primary" onclick="irrigateZoneDirect('${zone}')">Irriguer (local)</button>
                </div>
                <div class="card-body" id="zone-body-${zone}">
                    <span>Chargement...</span>
                </div>
            </div>
        `;
        zonesList.appendChild(col);
    });
}

function fetchZoneStatus(zone) {
    fetch(`/api/zone_status/${zone}`)
    .then(resp => resp.json())
    .then(data => {
        let html = "";
        if (data.error) {
            html = `<span class="text-danger">${data.error}</span>`;
        } else if (Array.isArray(data) && data.length > 0) {
            let last = {};
            data.forEach(m => last[m.type] = m.valeur);
            html = `
              <span class="badge bg-primary badge-measure">Humidité: ${last.humidite ?? '-'}%</span>
              <span class="badge bg-info badge-measure">EC: ${last.ec ?? '-'}</span>
              <span class="badge bg-success badge-measure">Temp: ${last.temp ?? '-'}°C</span>
              <span class="badge bg-warning badge-measure">LDR1: ${last.ldr1 ?? '-'}</span>
              <span class="badge bg-warning badge-measure">LDR2: ${last.ldr2 ?? '-'}</span>
              <br><small class="text-muted">Dernière mesure: ${data[0].timestamp ?? ''}</small>
            `;
        } else {
            html = `<span class="text-muted">Aucune donnée récente.</span>`;
        }
        document.getElementById(`zone-body-${zone}`).innerHTML = html;
    });
}

function irrigateZone(zone) {
    let minutes = prompt("Durée d'irrigation (minutes) ?", 10);
    if (!minutes) return;
    fetch('/api/irrigate', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({zone, minutes: parseInt(minutes)})
    })
    .then(resp => resp.json())
    .then(res => {
        alert(res.msg || "Commande envoyée via serveur central !");
    });
}

function irrigateZoneDirect(zone) {
    let minutes = prompt("Durée d'irrigation locale (minutes) ?", 10);
    if (!minutes) return;
    fetch('/api/direct_irrigate', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({zone, minutes: parseInt(minutes)})
    })
    .then(resp => resp.json())
    .then(res => {
        alert(res.msg || "Commande envoyée en direct !");
    });
}

function updateAllZones() { ZONES.forEach(fetchZoneStatus); }
function fetchWeather() {
    fetch('/api/forecast')
    .then(resp => resp.json())
    .then(data => {
        let html = "";
        if (data && data.precip_next_6h !== undefined) {
            html = `
                <b>Précipitations 6h :</b> ${data.precip_next_6h} mm<br>
                <b>Temp. actuelle :</b> ${data.temp_now} °C
            `;
        } else {
            html = "Données météo indisponibles.";
        }
        document.getElementById("meteo-content").innerHTML = html;
    });
}

document.addEventListener("DOMContentLoaded", function() {
    renderZones();
    updateAllZones();
    fetchWeather();
    setInterval(updateAllZones, 10000);
    setInterval(fetchWeather, 60000);
});