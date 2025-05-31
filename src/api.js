export const ZONES = [
  "zone1", "zone2", "zone3", "zone4",
  "zone5", "zone6", "zone7", "zone8"
];

export const ZONE_URLS = {
  zone1: "http://192.168.1.101",
  zone2: "http://192.168.1.101",
  zone3: "http://192.168.1.102",
  zone4: "http://192.168.1.102",
  zone5: "http://192.168.1.103",
  zone6: "http://192.168.1.103",
  zone7: "http://192.168.1.104",
  zone8: "http://192.168.1.104",
};

export async function fetchZoneStatus(zone) {
  const res = await fetch(`/zone/${zone}?json=1`);
  if (!res.ok) throw new Error("API unavailable");
  return await res.json();
}

export async function irrigate(zone, minutes=10) {
  const res = await fetch("/api/irrigate", {
    method: "POST",
    headers: {"Content-Type": "application/json"},
    body: JSON.stringify({zone, minutes})
  });
  return await res.json();
}

export async function irrigateLocal(zone, minutes=10) {
  const res = await fetch("/api/direct_irrigate", {
    method: "POST",
    headers: {"Content-Type": "application/json"},
    body: JSON.stringify({zone, minutes})
  });
  return await res.json();
}