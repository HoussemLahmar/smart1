import tkinter as tk
from tkinter import ttk, messagebox, filedialog
from api import get_zones, get_history, export_zone_csv, set_mode, get_plan, add_plan, delete_plan

class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Dashboard Irrigation Desktop")
        self.geometry("1000x650")
        self.create_widgets()
        self.refresh_zones()

    def create_widgets(self):
        frm_top = ttk.Frame(self); frm_top.pack(fill="x")
        self.zone_label = ttk.Label(frm_top, text="Zone: ")
        self.zone_label.pack(side="left")
        self.zone_combo = ttk.Combobox(frm_top, values=[])
        self.zone_combo.pack(side="left")
        self.zone_combo.bind("<<ComboboxSelected>>", lambda e: self.refresh_zone())
        self.mode_var = tk.StringVar(value="manuel")
        ttk.Radiobutton(frm_top, text="Manuel", variable=self.mode_var, value="manuel", command=self.set_mode).pack(side="left")
        ttk.Radiobutton(frm_top, text="Intelligent", variable=self.mode_var, value="auto", command=self.set_mode).pack(side="left")
        ttk.Button(frm_top, text="Rafraîchir", command=self.refresh_zone).pack(side="left")

        self.data_text = tk.Text(self, height=16, width=70)
        self.data_text.pack()

        frm_plot = ttk.Frame(self); frm_plot.pack()
        self.plot_btn = ttk.Button(frm_plot, text="Voir graphique humidité", command=self.show_plot)
        self.plot_btn.pack(side="left")
        self.export_btn = ttk.Button(frm_plot, text="Exporter CSV", command=self.export_csv)
        self.export_btn.pack(side="left")

        frm_plan = ttk.LabelFrame(self, text="Planification")
        frm_plan.pack(fill="x", pady=10)
        self.date_entry = ttk.Entry(frm_plan, width=10)
        self.date_entry.pack(side="left"); self.date_entry.insert(0, "2025-06-01")
        self.time_entry = ttk.Entry(frm_plan, width=6)
        self.time_entry.pack(side="left"); self.time_entry.insert(0, "06:00")
        self.dur_spin = ttk.Spinbox(frm_plan, from_=1, to=60, width=3)
        self.dur_spin.pack(side="left"); self.dur_spin.set(15)
        self.qty_spin = ttk.Spinbox(frm_plan, from_=1, to=10, width=3)
        self.qty_spin.pack(side="left"); self.qty_spin.set(3)
        self.rec_combo = ttk.Combobox(frm_plan, values=["once","daily","weekly"], width=8)
        self.rec_combo.pack(side="left"); self.rec_combo.set("once")
        ttk.Button(frm_plan, text="Ajouter", command=self.add_plan).pack(side="left")
        self.plan_list = tk.Listbox(frm_plan, width=80, height=3)
        self.plan_list.pack()
        ttk.Button(frm_plan, text="Supprimer planning", command=self.remove_plan).pack()

    def refresh_zones(self):
        zones = get_zones()
        zone_names = [z["zone"] for z in zones]
        self.zone_combo["values"] = zone_names
        if zone_names:
            self.zone_combo.set(zone_names[0])
            self.refresh_zone()

    def refresh_zone(self):
        zone = self.zone_combo.get()
        data = get_history(zone)
        self.data_text.delete("1.0", tk.END)
        for row in data:
            self.data_text.insert(tk.END, f"{row}\n")
        # Refresh plan
        plans = get_plan(zone)
        self.plan_list.delete(0, tk.END)
        for p in plans:
            self.plan_list.insert(tk.END, f"{p['date']} {p['time']} {p['duration']}min {p['quantity']}L/min [{p['recurrence']}] (id:{p['id']})")

    def set_mode(self):
        zone = self.zone_combo.get()
        mode = self.mode_var.get()
        set_mode(zone, mode)
        messagebox.showinfo("Mode", f"Mode changé: {mode}")

    def show_plot(self):
        zone = self.zone_combo.get()
        import webbrowser
        webbrowser.open(f"http://localhost:5000/api/plot/humidite?zone={zone}")

    def export_csv(self):
        zone = self.zone_combo.get()
        filename = filedialog.asksaveasfilename(defaultextension=".csv")
        if filename:
            export_zone_csv(zone, filename)
            messagebox.showinfo("Export", "Export CSV réussi")

    def add_plan(self):
        zone = self.zone_combo.get()
        plan = {
            "date": self.date_entry.get(),
            "time": self.time_entry.get(),
            "duration": int(self.dur_spin.get()),
            "quantity": int(self.qty_spin.get()),
            "recurrence": self.rec_combo.get()
        }
        add_plan(zone, plan)
        self.refresh_zone()

    def remove_plan(self):
        sel = self.plan_list.curselection()
        if not sel: return
        line = self.plan_list.get(sel[0])
        plan_id = int(line.split("id:")[1].split(")")[0])
        zone = self.zone_combo.get()
        delete_plan(zone, plan_id)
        self.refresh_zone()

if __name__ == "__main__":
    App().mainloop()