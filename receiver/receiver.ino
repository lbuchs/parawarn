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
bool relaisState = false;

// Intervals
unsigned long sendInterval = 10000; // nur alle 10s senden
unsigned long lampOffTimeout = 36000000; // 10h


// Zeitstempel
unsigned long lastSend = 0;
unsigned long lastLampOn = 0;
unsigned long lastLedBlink = 0;

// Commands
char cmdLampSetOn[]    = "CLON"; // Anweisung, die Lampe einzuschalten
char cmdLampSetOff[]   = "CLOF"; // Anweisung, die Lampe auszuschalten
char cmdGetLampState[] = "CLST"; // Anweisung, den Status zurückgegeben

// Replys
char replyStateOn[]   = "LSON"; // Antwort: Die Lampe ist eingeschaltet
char replyStateOff[]  = "LSOF"; // Antwort: Die Lampe ist ausgeschaltet

// pins
int pinRelais = 2; // Relais
int pinLed = 4; // LED Statusanzeige
int pinTelemetry = 12; // Telemetrie
int transmitterStateA = 0; // Sender
int transmitterStateB = 15; // Sender


char buf[5] = {'\0','\0','\0','\0','\0'};

void setup() {
  // Ausgänge Setzen 
  pinMode(pinRelais, OUTPUT);
  pinMode(pinTelemetry, OUTPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(transmitterStateA, OUTPUT);
  pinMode(transmitterStateB, OUTPUT);
  
  digitalWrite(pinRelais, LOW);
  digitalWrite(pinTelemetry, LOW);
  digitalWrite(transmitterStateA, LOW);  
  digitalWrite(transmitterStateB, LOW);
  
  // Serielle Schnittstelle zu Funkmodul starten  
  Serial2.begin(9600); 

  // LED Testen
  ledTest(0);
}

void loop() {
  // ********************************
  // Serielle Daten lesen
  // ********************************

  // alle seriellen Daten lesen und prüfen
  if (Serial2.available() > 0) {

    // Array nachrutschen
    buf[0] = buf[1];
    buf[1] = buf[2];
    buf[2] = buf[3];    
    buf[3] = Serial2.read();
    
    // Lampe ein
    if (strcmp(buf, cmdLampSetOn) == 0) {
      lampOn = true;
      replyState = true;
      lastSend = 0;
      lastLampOn = millis();

    // Lampe aus
    } else if (strcmp(buf, cmdLampSetOff) == 0) {
      lampOn = false;
      replyState = true;
      lastSend = 0;
      
    // Status Abfrage
    } else if (strcmp(buf, cmdGetLampState) == 0) {
      replyState = true;      
    }
    
  }
  
  // ********************************
  // Relais setzen
  // ********************************

  if (lampOn != relaisState) {
    digitalWrite(pinRelais, lampOn ? HIGH : LOW);
    digitalWrite(pinTelemetry, lampOn ? HIGH : LOW);    
    digitalWrite(pinLed, lampOn ? HIGH : LOW);
    relaisState = lampOn;
  }
  
  // ********************************
  // Antwort senden
  // ********************************
  
  if (replyState == true && (lastSend == 0 || (lastSend + sendInterval) < millis())) {
      Serial2.print(lampOn ? replyStateOn : replyStateOff);
      lastSend = millis();
      replyState = false;
  }


  // ********************************
  // lampe nach timeout ausschalten
  // ********************************
  if (lampOn == true && (lastLampOn + lampOffTimeout) < millis()) {
      lampOn = false;
      replyState = true;
  }

  // ********************************
  // LED blinken lassen
  // ********************************
  if ((lastLedBlink + 10000) < millis()) {
    digitalWrite(pinLed, relaisState ? LOW : HIGH);
    delay(150);
    digitalWrite(pinLed, relaisState ? HIGH : LOW);
    lastLedBlink = millis();
  }
  
  
}

void ledTest(int cnt) {
  digitalWrite(pinLed, HIGH);
  delay(200);
  digitalWrite(pinLed, LOW);
  delay(200);


  //rekursion
  if (cnt < 3) {
    ledTest(cnt+1);  
  }
}

