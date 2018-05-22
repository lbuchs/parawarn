
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
