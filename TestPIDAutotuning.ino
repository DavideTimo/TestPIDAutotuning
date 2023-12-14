#include "Forno.h"

Forno forno(0.01, 0.002);

//   ******* SETUP *******
void setup() {

}

//   ******* LOOP *******
void loop() {
    forno.aggiorna();
    Serial.println("La temperatura del forno è " + String(forno.temperatura()) + " °C");
}
