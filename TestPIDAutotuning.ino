#include "forno.h"
#include "MillisTimer.h"
#include "PID_v2.h"
#include "PID_AutoTune_v0.h"


MillisTimer TimerFcToHMI = MillisTimer(500);  //creo un Timer che dura mezzo secondo




//creo oggetto forno
Forno mioForno (0.1,0.002);

double kp = 2;
double ki = 5;
double kd = 1;
double setpoint = 150.0;
double PidOutput = 0.0;
//creo oggetto PID
PID_v2 mioPID(kp,ki,kd,PID::Direct);

//variabili
double temperaturaAttuale; //temperatura attuale del forno


PID_ATune myPIDAtune(&temperaturaAttuale, &PidOutput);


//   ******* SETUP *******
void setup()
{
    mioPID.Start(temperaturaAttuale, PidOutput, setpoint);  // input, current uotput, setpoint

    /*
    SetNoiseBand(double x); Questo parametro definisce la "banda di rumore" e rappresenta la massima variazione 
    che può essere tollerata prima che l'algoritmo di autotuning consideri il sistema stabile. 
    Se il segnale di ingresso varia meno della banda di rumore, viene considerato come rumore 
    di misura e ignorato. Questo parametro aiuta a ridurre l'effetto del rumore sulla stima 
    del modello del sistema.
    */
    myPIDAtune.SetNoiseBand(1);

    /*
    SetOutputStep(double x). Questo parametro specifica la dimensione del passo (step) che verrà utilizzato per rilevare 
    la risposta in uscita del sistema durante l'autotuning. Un passo più grande può accelerare 
    il processo di autotuning, ma potrebbe influenzare la stabilità del sistema. 
    Un valore più piccolo può rendere l'autotuning più lento, ma può essere più preciso.
    */
    myPIDAtune.SetOutputStep(1);

    /*
    Questo parametro definisce il periodo di tempo (in secondi) su cui l'algoritmo di autotuning 
    guarda indietro per analizzare la risposta in uscita del sistema. Un periodo di guardia più 
    lungo può catturare meglio le dinamiche a lungo termine del sistema, ma potrebbe rendere 
    l'autotuning più lento.
    */
    myPIDAtune.SetLookbackSec(5);



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
    mioForno.accendi();      //accendo il forno
    PidOutput = mioPID.Run(temperaturaAttuale); //iterazione PID e prendo il suo output
    mioForno.impostaPotenzaPercentuale(PidOutput); //l'uscita del PID è la regolazione del forno
    mioForno.aggiorna();

    TimerFcToHMI.run(); //diagnostica  
    
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
    Serial.println("L'uscita del PID è " + String(PidOutput));
    
}