#include "forno.h"
#include "MillisTimer.h"

MillisTimer TimerFcToHMI = MillisTimer(500);  //creo un Timer che dura mezzo secondo


Forno mioForno (0.01,0.002);

double temperaturaAttuale;

//   ******* SETUP *******
void setup()
{
    TimerFcToHMI.setInterval(500);
    TimerFcToHMI.expiredHandler(FcToHmi);
    TimerFcToHMI.start();

    // Inizializza la comunicazione seriale a 9600 baud
    Serial.begin(9600);

}

//   ******* LOOP *******
void loop()
{
    mioForno.aggiorna();
    temperaturaAttuale = mioForno.ottieniTemperatura();
    TimerFcToHMI.run();
    
    
}




/*
Nel contesto di un temporizzatore, la funzione di gestione degli eventi 
viene chiamata quando il timer scade. Passare un riferimento a un oggetto 
MillisTimer come parametro fornisce alla funzione di gestione degli eventi 
accesso all'oggetto timer stesso, consentendole di interagire con le sue 
proprietà o di effettuare operazioni specifiche.
es. mt.mt.setTimeout(2000, myTimerFunction); //  reimpostare un nuovo intervallo nel Timer
*/
void FcToHmi(MillisTimer &mt){
    Serial.println("La temperatura del forno è " + String(temperaturaAttuale) + " °C");
    
}