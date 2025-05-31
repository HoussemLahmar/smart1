const { app, BrowserWindow } = require('electron');
const { spawn } = require('child_process');
let flaskProcess = null;

function createWindow () {
  const win = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true
    }
  });
  win.loadFile('build/index.html');
}

app.whenReady().then(() => {
  // Démarre le backend Flask packagé
  flaskProcess = spawn('./dist/app.exe', [], { detached: true });

  flaskProcess.stdout.on('data', (data) => {
    console.log(`Flask: ${data}`);
  });

  flaskProcess.stderr.on('data', (data) => {
    console.error(`Flask erreur: ${data}`);
  });

  flaskProcess.on('close', (code) => {
    console.log(`Flask terminé avec code ${code}`);
  });

  createWindow();
});

app.on('window-all-closed', () => {
  if (flaskProcess) {
    flaskProcess.kill();
  }
  app.quit();
});