import React, { useEffect, useState } from "react";
import { useParams } from "react-router-dom";
import { fetchZoneStatus, irrigate, irrigateLocal, ZONE_URLS } from "../api";

export default function ZoneDetail() {
  const { zone } = useParams();
  const [data, setData] = useState({});
  const [minutes, setMinutes] = useState(10);

  const reload = () => fetchZoneStatus(zone).then(setData);

  useEffect(() => { reload(); }, [zone]);

  return (
    <div>
      <h2>Détails de la {zone}</h2>
      <ul>
        <li><b>Température :</b> {data.temperature ?? "-"} °C</li>
        <li><b>Humidité :</b> {data.humidite ?? "-"} %</li>
        <li><b>LDR1 :</b> {data.ldr1 ?? "-"}</li>
        <li><b>LDR2 :</b> {data.ldr2 ?? "-"}</li>
        <li><b>POMPE :</b> {data.pump ? <b style={{color:"green"}}>ON</b> : <span style={{color:"red"}}>OFF</span>}</li>
      </ul>
      <input type="number" min={1} max={60} value={minutes}
        onChange={e => setMinutes(e.target.value)} style={{width:54, marginRight: 8}} />
      <button className="btn btn-success me-2" onClick={async ()=>{
        await irrigate(zone, minutes); reload(); alert("Commande envoyée !");
      }}>Irriguer</button>
      <button className="btn btn-secondary" onClick={async ()=>{
        await irrigateLocal(zone, minutes); reload(); alert("Commande locale envoyée !");
      }}>Irriguer (local)</button>
      <div className="mt-3">
        <a href={ZONE_URLS[zone]} target="_blank" rel="noopener noreferrer" className="btn btn-info">Dashboard local</a>
      </div>
    </div>
  );
}