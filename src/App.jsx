import React from "react";
import { BrowserRouter, Routes, Route, Link } from "react-router-dom";
import GlobalIrrigation from "./components/GlobalIrrigation";
import ZoneTable from "./components/ZoneTable";
import ZoneDetail from "./components/ZoneDetail";

export default function App() {
  return (
    <BrowserRouter>
      <div className="container py-4">
        <h1>
          <Link to="/" style={{textDecoration:"none", color:"inherit"}}>Dashboard Irrigation Multi-zones</Link>
        </h1>
        <Routes>
          <Route path="/" element={<>
            <GlobalIrrigation />
            <ZoneTable />
          </>} />
          <Route path="/zone/:zone" element={<ZoneDetail />} />
        </Routes>
      </div>
    </BrowserRouter>
  );
}