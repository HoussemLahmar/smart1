import React, { useEffect, useState } from "react";
import axios from "axios";

const DAYS_FR = {
  1: "Lun", 2: "Mar", 3: "Mer", 4: "Jeu", 5: "Ven", 6: "Sam", 7: "Dim"
};

function slotToStyle(start, end) {
  const [sh, sm] = start.split(":").map(Number);
  const [eh, em] = end.split(":").map(Number);
  const from = (sh * 60 + sm) / (24 * 60) * 100;
  const to = (eh * 60 + em) / (24 * 60) * 100;
  return {
    left: `${from}%`,
    width: `${to - from}%`
  };
}

export default function IrrigationScheduleTimeline({ zone }) {
  const [slots, setSlots] = useState([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    setLoading(true);
    axios.get(`/api/schedule/${zone}`).then(res => {
      setSlots(res.data);
      setLoading(false);
    });
  }, [zone]);

  if (loading) return <div>Chargement…</div>;
  if (!slots.length) return <div>Aucun horaire programmé.</div>;

  const days = [1,2,3,4,5,6,7];

  return (
    <div>
      <h6>Timeline d'irrigation ({zone})</h6>
      <div style={{border: "1px solid #bbb", borderRadius:5, background:"#f9f9f9", padding:"8px"}}>
        <div style={{display:"flex", flexDirection:"column", gap:"8px"}}>
        {days.map(day =>
          <div key={day} style={{position:"relative", height:30, marginBottom:2}}>
            <span style={{position:"absolute", left:0, top:0, width:40, fontWeight:600}}>{DAYS_FR[day]}</span>
            <div style={{marginLeft:45, height:24, position:"relative", background:"#eee", borderRadius:12}}>
              {slots.filter(s => s.days.includes(day)).map((slot, idx) => {
                const style = slotToStyle(slot.start, slot.end);
                return (
                  <div key={idx}
                    style={{
                      ...style,
                      position:"absolute",
                      top:3,
                      height:18,
                      background:"#39c",
                      borderRadius:8,
                      color:"#fff",
                      fontSize:12,
                      textAlign:"center",
                      boxShadow:"0 1px 3px #0002"
                    }}>
                    {slot.start}–{slot.end}
                  </div>
                );
              })}
            </div>
          </div>
        )}
        </div>
        <div style={{marginLeft:45, position:"relative", height:18, fontSize:11, color:"#444"}}>
          {Array.from({length: 25}, (_,h)=>(
            <span key={h} style={{position:"absolute", left:`${h/24*100}%`, transform:"translateX(-50%)"}}>{h}</span>
          ))}
        </div>
        <div style={{marginLeft:45, fontSize:10, color:"#aaa"}}>Heure (0–24h)</div>
      </div>
    </div>
  );
}