import React, { useEffect, useState } from "react";
import axios from "axios";

// Liste des jours en français
const DAYS = [
  { value: 1, label: "Lundi" },
  { value: 2, label: "Mardi" },
  { value: 3, label: "Mercredi" },
  { value: 4, label: "Jeudi" },
  { value: 5, label: "Vendredi" },
  { value: 6, label: "Samedi" },
  { value: 7, label: "Dimanche" }
];

export default function IrrigationScheduleEditor({ zone }) {
  const [schedule, setSchedule] = useState([]);
  const [loading, setLoading] = useState(true);
  const [saving, setSaving] = useState(false);

  // Chargement des horaires
  useEffect(() => {
    setLoading(true);
    axios.get(`/api/schedule/${zone}`).then(res => {
      setSchedule(res.data);
      setLoading(false);
    });
  }, [zone]);

  // Ajout d'un créneau vide
  const addSlot = () => {
    setSchedule([...schedule, { days: [], start: "06:00", end: "06:15" }]);
  };

  // Suppression d'un créneau
  const removeSlot = idx => {
    setSchedule(schedule.filter((_, i) => i !== idx));
  };

  // Modification d'un champ dans un créneau
  const updateSlot = (idx, field, value) => {
    setSchedule(
      schedule.map((slot, i) => i === idx ? { ...slot, [field]: value } : slot)
    );
  };

  // Modification des jours (multi-select)
  const updateDays = (idx, day) => {
    let days = schedule[idx].days.includes(day)
      ? schedule[idx].days.filter(d => d !== day)
      : [...schedule[idx].days, day];
    updateSlot(idx, "days", days.sort());
  };

  // Sauvegarde
  const save = () => {
    setSaving(true);
    axios.post(`/api/schedule/${zone}`, schedule).then(() => setSaving(false));
  };

  if (loading) return <div>Chargement…</div>;
  return (
    <div>
      <h5>Planification automatique ({zone})</h5>
      <button className="btn btn-success mb-2" onClick={addSlot}>Ajouter un créneau</button>
      {schedule.map((slot, idx) => (
        <div key={idx} className="border p-2 mb-2">
          <div>
            Jours :
            {DAYS.map(d => (
              <label key={d.value} className="ms-2">
                <input
                  type="checkbox"
                  checked={slot.days.includes(d.value)}
                  onChange={() => updateDays(idx, d.value)}
                /> {d.label}
              </label>
            ))}
          </div>
          <div>
            Heure début :
            <input
              type="time"
              value={slot.start}
              onChange={e => updateSlot(idx, "start", e.target.value)}
              className="ms-2 me-3"
            />
            Heure fin :
            <input
              type="time"
              value={slot.end}
              onChange={e => updateSlot(idx, "end", e.target.value)}
              className="ms-2"
            />
            <button className="btn btn-danger ms-3" onClick={() => removeSlot(idx)}>Supprimer</button>
          </div>
        </div>
      ))}
      <button className="btn btn-primary" onClick={save} disabled={saving}>
        {saving ? "Sauvegarde…" : "Enregistrer"}
      </button>
    </div>
  );
}