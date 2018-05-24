/*
 *  Copyright (C) 2018 L. Buchs
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


// Status des Moduls
// 0: wird eingeschaltet
// 1: ist eingeschaltet
// 2: wird ausgeschaltet
// 3: ist ausgeschaltet
int state = 0;
bool shutdownPressed = false;

// Intervals
unsigned long pushInterval = 30000; // Interval, mit dem versucht wird, die Lampe einzuschalten
unsigned long statusInterval = 180000; // Interval, in dem im ein- und ausgeschalteten Zustand die Lampe abgefragt wird
unsigned long offTimeout = 30000; // Zeit bis zum Ausschalten des Moduls, wenn die Lampe abgeschaltet wurde

// Zeitstempel
unsigned long lastTurnOnSignal = 0;
unsigned long lastTurnOffSignal = 0;
unsigned long lastRequStateSignal = 0;
unsigned long lastLampOffInfo = 0;
unsigned long lastShutdownPressed = 0;

// Commands
char cmdLampSetOn[]    = "CLON"; // Anweisung, die Lampe einzuschalten
char cmdLampSetOff[]   = "CLOF"; // Anweisung, die Lampe auszuschalten
char cmdGetLampState[] = "CLST"; // Anweisung, den Status zurückgegeben

// Replys
char replyStateOn[]   = "LSON"; // Antwort: Die Lampe ist eingeschaltet
char replyStateOff[]  = "LSOF"; // Antwort: Die Lampe ist ausgeschaltet

// pins
int pinLedGreen    = D1; // Grünes LED
int pinLedRed   = D2; // Rotes LED
int pinLedBlue = D3; // Blaues LED
int pinSelfhold = D4; // Selbsthaltung
int pinBtn = D5; // Button zum ausschalten


void setup() {
  // Ausgänge Setzen und Selbsthaltung aktivieren
  pinMode(pinLedGreen, OUTPUT);
  pinMode(pinLedRed, OUTPUT);
  pinMode(pinLedBlue, OUTPUT);
  pinMode(pinSelfhold, OUTPUT);
  pinMode(pinBtn, INPUT);
  digitalWrite(pinSelfhold, HIGH);
  Serial.begin(9600);
  ledTest(0);
}

void loop() {
  // ********************************
  // Button lesen (2s Entprellt)
  // ********************************
  
  if (digitalRead(pinBtn) == HIGH && lastShutdownPressed + 2000 < millis()) {
    if (shutdownPressed == false) {
      shutdownPressed = true;
      state = 2; // wird ausgeschaltet 
      
    } else if (shutdownPressed == true) {
      shutdownPressed = false;
      state = 0; // wird wieder eingeschaltet
    }
    
    lastShutdownPressed = millis();
  }

  // ********************************
  // Serielle Daten lesen
  // ********************************
  
  while (Serial.available() >= 4) {
    char buf[4];
    Serial.readBytes(buf, 4);
    
    // Lampe ein
    if (strcmp(buf, replyStateOn) == 0) {
      state = 1;

    // Lampe aus
    } else if (strcmp(buf, replyStateOff) == 0) {
      state = 3;
      lastLampOffInfo = millis();
    }
  }

  // ********************************
  // Serielle Daten senden
  // ********************************
    
  // einschalten
  if (state == 0) {
    if (lastTurnOnSignal == 0 || (lastTurnOnSignal + pushInterval) < millis()) {
      Serial.print(cmdLampSetOn);
      lastTurnOnSignal = millis();
      lastRequStateSignal = millis();
    }

  // ausschalten
  } else if (state == 2) {
    if (lastTurnOnSignal == 0 || (lastTurnOnSignal + pushInterval) < millis()) {
      Serial.print(cmdLampSetOn);
      lastTurnOnSignal = millis();
      lastRequStateSignal = millis();
    }

  // Ein- und ausgeschaltet: Status abfragen
  } else if (state == 1 || state == 3) {
    if (lastRequStateSignal == 0 || (lastRequStateSignal + statusInterval) < millis()) {
      Serial.print(cmdGetLampState);
      lastRequStateSignal = millis();
    }
  }

  // ********************************
  // LEDs ansteuern
  // ********************************

  if (state == 0) { // wird eingeschaltet (blinkt)
    ledBlinker(pinLedGreen);
    digitalWrite(pinLedRed, LOW);
    
  } else if (state == 2) { // wird ausgeschaltet (blinkt)
    ledBlinker(pinLedRed);
    digitalWrite(pinLedGreen, LOW);

  } else if (state == 1) { // Ein
    digitalWrite(pinLedGreen, HIGH);
    digitalWrite(pinLedRed, LOW);
    
  }  else if (state == 3) { // Aus
    digitalWrite(pinLedGreen, LOW);
    digitalWrite(pinLedRed, HIGH);
  } 

  // ********************************
  // Modul überwachen
  // ********************************

  // Modul ausschalten wenn Zeit abgelaufen
  if (shutdownPressed == true && state == 3 && (lastLampOffInfo + offTimeout) < millis()) {
    digitalWrite(pinSelfhold, LOW);
  }
}


// ********************************
// Funktionen
// ********************************

/**
 * Testet alle LEDs durch blinken durch.
 */
void ledTest(int cnt) {
  digitalWrite(pinLedGreen, HIGH);
  delay(400);
  digitalWrite(pinLedGreen, LOW);
  digitalWrite(pinLedRed, HIGH);
  delay(400);
  digitalWrite(pinLedRed, LOW);
  digitalWrite(pinLedBlue, HIGH);
  delay(400);
  digitalWrite(pinLedBlue, LOW);

  //rekursion
  if (cnt < 3) {
    ledTest(cnt+1);  
  }
}

/**
 * Lässt die LED blinken (0.5s interval)
 */
bool ledOn = false;
unsigned long lastChange = 0;
void ledBlinker(int pin) {
  if (ledOn == false && lastChange + 500 < millis()) {
    digitalWrite(pin, ledOn ? LOW : HIGH);
    lastChange = millis(); 
    ledOn = !ledOn;
  }
}






































