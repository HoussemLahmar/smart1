import unittest
from unittest.mock import patch
from main import App

class TestDesktopApp(unittest.TestCase):
    @patch("dashboard.desktop.api.get_zones", return_value=[{"zone":"zone1"}])
    @patch("dashboard.desktop.api.get_history", return_value=[{"capteur":"humidite","valeur":42,"timestamp":"2025-06-01T08:00"}])
    def test_refresh_shows_data(self, mock_hist, mock_zones):
        app = App()
        app.zone_combo.set("zone1")
        app.refresh_zone()
        self.assertIn("humidite", app.data_text.get("1.0", "end"))

if __name__ == "__main__":
    unittest.main()