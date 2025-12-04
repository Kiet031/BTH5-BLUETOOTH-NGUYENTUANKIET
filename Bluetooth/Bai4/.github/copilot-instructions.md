## Repo-specific Copilot instructions

This project is a small Express/EJS web app that reads DHT11 sensor data from an Arduino over a serial port. The guidance below is focused, actionable, and concrete so an AI coding agent can be immediately productive.

- **Project layout (big picture):**
  - `app.js`: Express app bootstrap. Registers routes and serves `public` static files and `views` (EJS).
  - `routes/`: HTTP endpoints. Notable: `routes/DHT11.js` mounts at `/DHT11`.
  - `public/services/serialService.js`: Serial communication with Arduino (uses `serialport`).
  - `public/javascripts/dht11_fun.js`: Front-end polling logic (calls `/DHT11/connect`, `/DHT11/read`, `/DHT11/disconnect`).
  - `views/DHT11_Display.ejs`: Simple UI with connect/disconnect buttons and `#dht11Data` element.
  - `package.json`: start script uses `node ./bin/www` (run `npm start` from the `dht11_web` folder).

- **Data flow & integration points (how data moves):**
  - Browser -> HTTP: client polls `/DHT11/read` every second (see `dht11_fun.js`).
  - `/DHT11/connect` and `/DHT11/disconnect` call into `public/services/serialService.js` which opens/closes the serial port.
  - `serialService.js` creates a `SerialPort` on a hardcoded `path: 'COM2'` and `baudRate: 9600`. Update to match the developer's machine.

- **Concrete examples & patterns to follow**
  - Route wiring: `app.js` does `var DHT11Router = require('./routes/DHT11'); app.use('/DHT11', DHT11Router);`. New routes should follow this pattern.
  - Static assets: `app.use(express.static(path.join(__dirname, 'public')));`. Public JS/CSS are referenced as `/javascripts/...` or `/stylesheets/...` from EJS.
  - Service exports: `serialService.js` exports `{ connectSerial, readDHT11, disconnectSerial }` and `routes/DHT11.js` imports it with `const serial = require('../public/services/serialService');`.

- **Important, discoverable issues an agent should know (do not assume):**
  - `serialService.readDHT11` uses `parser.on('data', ...)` and calls `callback(line)`, but `callback` is not defined inside that module. The router (`routes/DHT11.js`) currently maintains a `latesData` variable but never updates it — so `/DHT11/read` will always return the initial value unless an agent wires the parser data into the route (for example, have `serialService` accept an event emitter or provide a setter callback).
  - `serialport` and `@serialport/parser-readline` are used in `serialService.js` but are not listed in `package.json` dependencies. Before running on a fresh machine, run:

    npm install --save serialport @serialport/parser-readline

  - Serial port path is hardcoded to `COM2`. On Windows, confirm the device via Device Manager and update `public/services/serialService.js` accordingly.

- **Developer workflows / useful commands**
  - Install dependencies (if missing): `npm install` from the `dht11_web` folder.
  - Start server: `npm start` (runs `node ./bin/www`).
  - If adding native modules (like `serialport`), run `npm rebuild` or reinstall on the target platform.

- **When changing serial behavior prefer these minimal patterns**
  - Prefer a single `EventEmitter` or callback registration API on `serialService` rather than global variables. Example pattern: `serialService.on('data', (line) => { /* update shared state */ })`.
  - Keep the HTTP route layer simple: do not block routes waiting for serial data. Use in-memory last-value caching or push notifications (e.g., add socket.io) if realtime is needed.

- **Tests & debugging hints**
  - There are no automated tests in the repo. For debugging serial flows, add console logs in `serialService.js` and temporarily expose parser events to the route for quick verification.
  - To reproduce missing-dependency failures, run `node ./bin/www` and observe the stack trace — it will show `Cannot find module 'serialport'` if `serialport` isn't installed.

- **Files you will likely edit when implementing features or fixes**
  - `public/services/serialService.js` — serial open/close, parser handling.
  - `routes/DHT11.js` — HTTP endpoints and where `latesData` is returned.
  - `public/javascripts/dht11_fun.js` and `views/DHT11_Display.ejs` — front-end polling and display.

If anything is unclear or you'd like me to implement one of the suggested fixes (for example: add `serialport` to `package.json`, wire parser output into `/DHT11/read`, or switch to an event-emitter pattern), tell me which change you prefer and I will implement and test it.
