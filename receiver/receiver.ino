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

// Status der Lampe
bool lampOn = false;
bool replyState = false;

// Intervals
unsigned long sendInterval = 180000;
unsigned long lastSend = 0;

// Commands
char cmdLampSetOn[]    = "CLON"; // Anweisung, die Lampe einzuschalten
char cmdLampSetOff[]   = "CLOF"; // Anweisung, die Lampe auszuschalten
char cmdGetLampState[] = "CLST"; // Anweisung, den Status zurückgegeben

// Replys
char replyStateOn[]   = "LSON"; // Antwort: Die Lampe ist eingeschaltet
char replyStateOff[]  = "LSOF"; // Antwort: Die Lampe ist ausgeschaltet

// pins
int pinRelais = D1; // Relais


void setup() {
  // Ausgänge Setzen und Selbsthaltung aktivieren
  pinMode(pinRelais, OUTPUT);

}

void loop() {
  // ********************************
  // Serielle Daten lesen
  // ********************************
  
  
  while (Serial.available() >= 4) {
    char buf[5];
    Serial.readBytes(buf, 4);
    buf[4] = '\0';
    
    // Lampe ein
    if (strcmp(buf, cmdLampSetOn) == 0) {
      lampOn = true;
      replyState = true;

    // Lampe aus
    } else if (strcmp(buf, cmdLampSetOff) == 0) {
      lampOn = false;
      replyState = true;
      
    // Status Abfrage
    } else if (strcmp(buf, cmdGetLampState) == 0) {
      lampOn = false;
      replyState = true;
    }
  }
  
  // ********************************
  // Relais setzen
  // ********************************
  
  digitalWrite(pinRelais, lampOn ? HIGH : LOW);
  
  
  // ********************************
  // Antwort senden
  // ********************************
  
  if (replyState == true && (lastSend == 0 || (lastSend + sendInterval) < millis())) {
      Serial.print(lampOn ? replyStateOn : replyStateOff);
      lastSend = millis();
      replyState = false;
  }
}
