import React, { useEffect, useState } from "react";
import axios from "axios";

// Utilitaire pour obtenir le nom du jour en français
const DAYS_FR = ["Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"];

// Utilitaire pour obtenir le nom du mois en français
const MONTHS_FR = [
  "Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
  "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"
];

// Vérifie si un slot correspond à ce jour
function isSlotForDate(slot, date) {
  // slot.days est une liste de 1 (Lun) à 7 (Dim)
  // JS : 0 (Dim) à 6 (Samedi)
  // On considère lundi=1 à dimanche=7
  // date.getDay(): 0=dimanche, 1=lundi, ..., 6=samedi
  let jsDay = date.getDay();
  jsDay = jsDay === 0 ? 7 : jsDay; // JS 0=dimanche → 7
  return slot.days.includes(jsDay);
}

export default function IrrigationScheduleMonthlyCalendar({ zone }) {
  const [slots, setSlots] = useState([]);
  const [loading, setLoading] = useState(true);
  const [month, setMonth] = useState(() => {
    const now = new Date();
    return { year: now.getFullYear(), month: now.getMonth() }; // month: 0=Jan
  });

  useEffect(() => {
    setLoading(true);
    axios.get(`/api/schedule/${zone}`).then(res => {
      setSlots(res.data);
      setLoading(false);
    });
  }, [zone]);

  // Calcule le calendrier du mois sélectionné
  const firstDayOfMonth = new Date(month.year, month.month, 1);
  const lastDayOfMonth = new Date(month.year, month.month + 1, 0);
  const daysInMonth = lastDayOfMonth.getDate();

  // Premier jour de la semaine (lundi=1) pour l'affichage
  const firstWeekDay = firstDayOfMonth.getDay() === 0 ? 7 : firstDayOfMonth.getDay();

  // Construction du tableau jours (6 semaines max)
  let calendar = [];
  let day = 1 - (firstWeekDay - 1);
  for (let w = 0; w < 6; w++) {
    let week = [];
    for (let d = 1; d <= 7; d++, day++) {
      if (day >= 1 && day <= daysInMonth) {
        week.push(day);
      } else {
        week.push(null);
      }
    }
    calendar.push(week);
    if (day > daysInMonth) break;
  }

  // Navigation mois
  function prevMonth() {
    setMonth(m => {
      let y = m.year, mo = m.month - 1;
      if (mo < 0) { mo = 11; y--; }
      return { year: y, month: mo };
    });
  }
  function nextMonth() {
    setMonth(m => {
      let y = m.year, mo = m.month + 1;
      if (mo > 11) { mo = 0; y++; }
      return { year: y, month: mo };
    });
  }

  if (loading) return <div>Chargement calendrier…</div>;
  return (
    <div>
      <h6>
        {MONTHS_FR[month.month]} {month.year} — Planning d'irrigation ({zone})
      </h6>
      <div className="mb-2">
        <button className="btn btn-outline-secondary btn-sm me-2" onClick={prevMonth}>&lt; Mois précédent</button>
        <button className="btn btn-outline-secondary btn-sm" onClick={nextMonth}>Mois suivant &gt;</button>
      </div>
      <table className="table table-bordered" style={{maxWidth:480, background:"#fff"}}>
        <thead>
          <tr>
            {DAYS_FR.map(jour => <th key={jour} className="text-center">{jour}</th>)}
          </tr>
        </thead>
        <tbody>
          {calendar.map((week, wi) =>
            <tr key={wi}>
              {week.map((dayNum, di) => {
                if (!dayNum) return <td key={di} style={{background:"#f0f0f0"}}></td>;
                // Date du jour
                const dateObj = new Date(month.year, month.month, dayNum);
                // Créneaux de ce jour
                const slotsForDay = slots.filter(slot => isSlotForDate(slot, dateObj));
                return (
                  <td key={di} style={{verticalAlign:"top", background: slotsForDay.length ? "#e6f7ff" : "#fafbfc"}}>
                    <div style={{fontWeight:600, fontSize:13}}>{dayNum}</div>
                    {slotsForDay.map((slot, idx) =>
                      <div key={idx} style={{
                        fontSize:12,
                        background:"#39c",
                        color:"#fff",
                        borderRadius:6,
                        margin:"2px 0",
                        padding:"0 3px"
                      }}>
                        {slot.start}–{slot.end}
                      </div>
                    )}
                  </td>
                );
              })}
            </tr>
          )}
        </tbody>
      </table>
      <div style={{fontSize:12, color:"#888"}}>Jour surligné = créneau(s) programmé(s)</div>
    </div>
  );
}