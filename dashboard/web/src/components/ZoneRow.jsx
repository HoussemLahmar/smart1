import React, { useState } from "react";
import { ZONE_URLS, irrigate, irrigateLocal } from "../api";
import { Link } from "react-router-dom";

export default function ZoneRow({ zone, data, onRefresh }) {
  const [minutes, setMinutes] = useState(10);
  const [loading, setLoading] = useState(false);

  const handleIrrigate = async (local=false) => {
    setLoading(true);
    if(local) await irrigateLocal(zone, minutes);
    else await irrigate(zone, minutes);
    setLoading(false);
    onRefresh();
    alert("Commande envoyée !");
  };

  return (
    <tr>
      <td>
        <Link to={`/zone/${zone}`}>{zone}</Link>
      </td>
      <td>
        <a href={ZONE_URLS[zone]} target="_blank" rel="noopener noreferrer">Dashboard local</a>
      </td>
      <td>{data?.humidite ?? "-"}</td>
      <td>{data?.temperature ?? "-"}</td>
      <td>{data?.ldr1 ?? "-"}</td>
      <td>{data?.ldr2 ?? "-"}</td>
      <td>
        {data?.pump ? <b style={{color:"green"}}>ON</b> : <span style={{color:"red"}}>OFF</span>}
      </td>
      <td>
        <input type="number" min={1} max={60} value={minutes}
          onChange={e => setMinutes(e.target.value)} style={{width:54}} />
        <button className="btn btn-sm btn-success ms-1"
          disabled={loading}
          onClick={()=>handleIrrigate(false)}>
          Irriguer
        </button>
        <button className="btn btn-sm btn-secondary ms-1"
          disabled={loading}
          onClick={()=>handleIrrigate(true)}>
          Local
        </button>
      </td>
    </tr>
  );
}