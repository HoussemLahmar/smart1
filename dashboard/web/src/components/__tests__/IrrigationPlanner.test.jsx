import { render, screen, fireEvent } from "@testing-library/react";
import IrrigationPlanner from "../IrrigationPlanner";

test("affiche le formulaire de planification", () => {
  render(<IrrigationPlanner selectedZone="zone1" />);
  expect(screen.getByText(/Nouvelle planification/)).toBeInTheDocument();
});