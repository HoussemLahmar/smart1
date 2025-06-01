import React, { useEffect, useState } from "react";
import SystemStatus from "./components/SystemStatus";
import EnvironmentalSensors from "./components/EnvironmentalSensors";
import IrrigationZones from "./components/IrrigationZones";
import PlanningControl from "./components/PlanningControl";
import SmartParams from "./components/SmartParams";
import Alerts from "./components/Alerts";
import LoginPage from "./components/Auth/LoginPage";
import { getStatus, getZones, getAlerts } from "./api/api";
import "./styles/main.css";

function App() {
  const [auth, setAuth] = useState(false);
  const [status, setStatus] = useState({});
  const [zones, setZones] = useState([]);
  const [alerts, setAlerts] = useState([]);

  useEffect(() => {
    if (!auth) return;
    getStatus().then(setStatus);
    getZones().then(setZones);
    getAlerts().then(setAlerts);
    const interval = setInterval(() => {
      getStatus().then(setStatus);
      getZones().then(setZones);
      getAlerts().then(setAlerts);
    }, 10000);
    return () => clearInterval(interval);
  }, [auth]);

  if (!auth) return <LoginPage onSuccess={() => setAuth(true)} />;

  return (
    <div className="dashboard">
      <h1>Système d'Irrigation Intelligente</h1>
      <div className="top-row">
        <SystemStatus data={status} />
        <EnvironmentalSensors data={zones} />
        <IrrigationZones zones={zones} />
      </div>
      <div className="bottom-row">
        <PlanningControl zones={zones} />
        <SmartParams />
        <Alerts alerts={alerts} />
      </div>
    </div>
  );
}

export default App;