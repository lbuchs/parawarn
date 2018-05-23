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
bool stateTurnOn = true;     // Lampe wird eingeschaltet
bool stateTurnedOn = false;  // Lampe ist aktiv
bool stateTurnOff = false;   // Lampe wird ausgeschaltet
bool stateTurnedOff = false; // Lampe ist ausgeschaltet

// Intervals
unsigned long statusInterval = 180000; // Interval, in dem im ein- und ausgeschalteten Zustand die Lampe abgefragt wird

// Commands
char cmdLampSetOn[]    = "CLON"; // Anweisung, die Lampe einzuschalten
char cmdLampSetOff[]   = "CLOF"; // Anweisung, die Lampe auszuschalten
char cmdGetLampState[] = "CLST"; // Anweisung, den Status zurückgegeben

// Replys
char replyStateOn[]   = "LSON"; // Antwort: Die Lampe ist eingeschaltet
char replyStateOff[]  = "LSOF"; // Antwort: Die Lampe ist ausgeschaltet

// pins
int pinSelfhold = D1; // Relais


void setup() {
  // Ausgänge Setzen und Selbsthaltung aktivieren
  pinMode(pinLedOn, OUTPUT);
  pinMode(pinLedOff, OUTPUT);
  pinMode(pinSelfhold, OUTPUT);
  digitalWrite(pinSelfhold, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

}
