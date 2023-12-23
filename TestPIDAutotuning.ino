#include "forno.h"
#include "MillisTimer.h"
#include "PID_v2.h"
#include "PID_AutoTune_v0.h"


MillisTimer TimerFcToHMI = MillisTimer(500);  //creo un Timer che dura mezzo secondo. Per limitare il carico di lavoro del processore eseguo la diagnostica ogni 2sec e non ad ogni ciclo


//creo oggetto forno
Forno mioForno (0.001,0.002);  // istanzio oggetto Forno

double kp = 2; // valore proporzionale PID
double ki = 5; // valore integrale PID
double kd = 1;  // valore derivativo PID
double setpoint = 80.0;  // setpoint del PID
double PidOutput = 0.0;  // output del PID

//creo oggetto PID
PID_v2 mioPID(kp,ki,kd,PID::Direct); // istanzio oggetto PID

//variabili
double temperaturaAttuale; //temperatura attuale del forno


PID_ATune myPIDAtune(&temperaturaAttuale, &PidOutput);  // istanzio oggetto PIDTune, dando come argomenti feedbackIn e output del PID da tunnare


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
La funzione di gestione degli eventi gestita dal tipo MillisTimer
viene chiamata quando il timer scade, e la funzione FcToHmi viene eseguita oneshot ogni N secondi. 
Passare un riferimento a un oggetto 
MillisTimer come parametro (&mt), fornisce alla funzione di gestione degli eventi 
accesso all'oggetto timer stesso, consentendole di interagire con le sue 
proprietà o di effettuare operazioni specifiche.
es. mt.setTimeout(2000, myTimerFunction); //  reimpostare un nuovo intervallo nel Timer
*/
void FcToHmi(MillisTimer &mt){
    Serial.println("1 - La temperatura del forno è " + String(temperaturaAttuale) + " °C");
    Serial.println("2 - L'uscita del PID è " + String(PidOutput));
    valoriDiagnostica valoriRX = mioForno.getDiagnostica();
    Serial.println("3 - Valori Diagnostica Forno: Potenza - " + String(valoriRX._potenza));
    Serial.println("4 - Valori Diagnostica Forno: Riscaldamento - " + String(valoriRX._riscaldamento));
    Serial.println("5 - Valori Diagnostica Forno: Raffreddamento - " + String(valoriRX._raffreddamento));
    Serial.println("6 - Valori Diagnostica Forno: Temperatura - " + String(valoriRX._temperatura));
    Serial.println("-------------------------------------------------------------");
}