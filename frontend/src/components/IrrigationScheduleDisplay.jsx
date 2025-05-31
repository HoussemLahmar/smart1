import React, { useEffect, useState } from "react";
import axios from "axios";

const DAYS_FR = {
  1: "Lundi", 2: "Mardi", 3: "Mercredi", 4: "Jeudi",
  5: "Vendredi", 6: "Samedi", 7: "Dimanche"
};

export default function IrrigationScheduleDisplay({ zone }) {
  const [slots, setSlots] = useState([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    setLoading(true);
    axios.get(`/api/schedule/${zone}`).then(res => {
      setSlots(res.data);
      setLoading(false);
    });
  }, [zone]);

  if (loading) return <div>Chargement des horaires…</div>;
  if (!slots.length) return <div>Aucun horaire programmé.</div>;

  return (
    <div>
      <h6>Horaires d'irrigation programmés ({zone})</h6>
      <ul>
        {slots.map((slot, idx) => (
          <li key={idx}>
            <b>Jours :</b> {slot.days.map(d => DAYS_FR[d]).join(", ")}<br />
            <b>De</b> {slot.start} <b>à</b> {slot.end}
          </li>
        ))}
      </ul>
    </div>
  );
}