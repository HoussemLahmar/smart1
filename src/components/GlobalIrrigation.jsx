import React, { useState } from "react";
import { ZONES, irrigate, irrigateLocal } from "../api";

export default function GlobalIrrigation() {
  const [minutes, setMinutes] = useState(10);
  const [loading, setLoading] = useState(false);

  const handleAll = async (fn, label) => {
    setLoading(true);
    await Promise.all(ZONES.map(zone => fn(zone, minutes)));
    setLoading(false);
    alert(`Commande "${label}" envoyée à toutes les zones !`);
  };

  return (
    <div className="mb-3">
      <input type="number" min={1} max={60} value={minutes}
        onChange={e => setMinutes(e.target.value)} style={{width: 54, marginRight: 8}} />
      <button className="btn btn-primary me-2"
        disabled={loading}
        onClick={() => handleAll(irrigate, "central")}>
        Irriguer toutes les zones (central)
      </button>
      <button className="btn btn-secondary"
        disabled={loading}
        onClick={() => handleAll(irrigateLocal, "locale")}>
        Irriguer toutes les zones (local)
      </button>
    </div>
  );
}