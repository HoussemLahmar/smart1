import React, { useEffect, useState } from "react";
import { ZONES, fetchZoneStatus } from "../api";
import ZoneRow from "./ZoneRow";

export default function ZoneTable() {
  const [data, setData] = useState({});
  const [refresh, setRefresh] = useState(false);

  const reload = () => setRefresh(r => !r);

  useEffect(() => {
    let mounted = true;
    Promise.all(ZONES.map(z => fetchZoneStatus(z).then(d => [z, d])
      .catch(()=>[z, {}])))
      .then(items => {
        if(mounted) setData(Object.fromEntries(items));
      });
    return () => { mounted = false; };
  }, [refresh]);

  useEffect(() => {
    const interval = setInterval(reload, 10000);
    return () => clearInterval(interval);
  }, []);

  return (
    <table className="table table-bordered">
      <thead>
        <tr>
          <th>Zone</th>
          <th>Lien direct</th>
          <th>Humidité</th>
          <th>Température</th>
          <th>LDR1</th>
          <th>LDR2</th>
          <th>POMPE</th>
          <th>Actions</th>
        </tr>
      </thead>
      <tbody>
        {ZONES.map(zone =>
          <ZoneRow key={zone} zone={zone} data={data[zone]} onRefresh={reload} />
        )}
      </tbody>
    </table>
  );
}