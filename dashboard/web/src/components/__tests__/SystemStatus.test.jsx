import { render, screen } from "@testing-library/react";
import SystemStatus from "../SystemStatus";

test("affiche le statut du système", () => {
  render(<SystemStatus data={{last_connection:"12:00", state:"Actif", mode:"Manuel", active_zones:"2/4"}} />);
  expect(screen.getByText(/12:00/)).toBeInTheDocument();
  expect(screen.getByText(/Actif/)).toBeInTheDocument();
  expect(screen.getByText("2/4")).toBeInTheDocument();
});