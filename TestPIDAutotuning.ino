#include "forno.h"
#include "MillisTimer.h"
#include "PID_v2.h"
#include "pidautotuner.h"



MillisTimer TimerFcToHMI = MillisTimer(1000);  //creo un Timer che dura mezzo secondo. Per limitare il carico di lavoro del processore eseguo la diagnostica ogni 2sec e non ad ogni ciclo


//creo oggetto forno
Forno mioForno (0.003,0.006);  // istanzio oggetto Forno

double kp = 1;           // valore proporzionale PID
double ki = 5;           // valore integrale PID
double kd = 2;           // valore derivativo PID
double setpoint = 50.0;  // setpoint del PID
double PidOutput = 0.0;  // output del PID

//creo oggetto PID
PID_v2 mioPID(kp,ki,kd,PID::Direct); // istanzio oggetto PID

//variabili
double temperaturaAttuale; //temperatura attuale del forno



//Autotuner 
// PID_ATune myPIDAtune(&temperaturaAttuale, &PidOutput);  // istanzio oggetto PIDTune, dando come argomenti feedbackIn e output del PID da tunnare  <-- (1)

PIDAutotuner tuner = PIDAutotuner();
long loopInterval = 100000;
double tunerOutput = 0; //dichiaro variabile di uscita del tuner 
float tuner_kp;
float tuner_ki;
float tuner_kd;

//   ******* SETUP *******
void setup()
{
    // *** COMUNICAZIONE SERIALE ***
    // Inizializza la comunicazione seriale a 9600 baud
    Serial.begin(9600);

    //setup *** PID ***
    mioPID.Start(temperaturaAttuale, PidOutput, setpoint);  // input, current uotput, setpoint
    
    
    //setup *** AUTOTUNER ***
    PIDAutotuner tuner = PIDAutotuner();

    /* 
    Imposto il valore target su cui sintonizzarsi
    Dipende dal tipo di regolazione che si sta effettuando. Dovrebbe essere impostato su un valore all'interno
    dell'intervallo abituale del setpoint. Per i sistemi a bassa inerzia, i valori all'estremo inferiore di questo intervallo.
    */
    tuner.setTargetInputValue(setpoint);

    /* 
    Imposto l'intervallo del loop in microsecondi
    Deve essere uguale all'intervallo di funzionamento del loop di controllo PID.
    Il PID che uso di default ha un intervallo di 100 millisecondi, quindi 100.000microsecondi
    */
    tuner.setLoopInterval(loopInterval);

    /* 
    Imposto l'intervallo di uscita
    Questi sono i valori di uscita minimi e massimi possibili di ciò che si usa per controllare il sistema (Arduino analogWrite, ad esempio, è 0-255).
    per controllare il sistema (Arduino analogWrite, ad esempio, è 0-255).
    */
    tuner.setOutputRange(0, 255);

    /* 
    Impostare la modalità di sintonizzazione Ziegler-Nichols
    Impostare PIDAutotuner::ZNModeBasicPID, PIDAutotuner::ZNModeLessOvershoot o PIDAutotuner::ZNModeNoOvershoot. 
    L'impostazione predefinita è ZNModeNoOvershoot, in quanto è l'opzione più sicura.
    */
    tuner.setZNMode(PIDAutotuner::ZNModeBasicPID);

    /* 
    Deve essere richiamato immediatamente prima del ciclo di tuning.
    Deve essere richiamato con il tempo corrente in microsecondi
    */
    tuner.startTuningLoop(micros());
    Serial.println("Inzizio ciclo di tuning");
    // Eseguire un ciclo finché tuner.isFinished() non restituisce true
    long microseconds;
    while (!tuner.isFinished()) {
        Serial.println("Ciclo di Tuning");
        // Questo loop deve funzionare alla stessa velocità del loop di controllo PID da regolare.
        long prevMicroseconds = microseconds;
        microseconds = micros();

        // Ottenere qui il valore di ingresso (temperatura, posizione dell'encoder, velocità, ecc.)
        //double input = doSomethingToGetInput();
        mioForno.accendi();
        mioForno.aggiorna();                                    //aggiorna temperatura del forno
        temperaturaAttuale = mioForno.ottieniTemperatura();     //leggo la temperatura attuale del forno
        Serial.println("Ciclo di Tuning: temperatura attuale forno: " + String(temperaturaAttuale));
        // Chiamare tunePID() con il valore di ingresso e il tempo corrente in microsecondi
        tunerOutput = tuner.tunePID(temperaturaAttuale, microseconds);
        Serial.println("Ciclo di Tuning: potenza attuale forno: " + String(tunerOutput));
        /* 
        Impostare l'uscita - tunePid() restituirà valori compresi nell'intervallo configurato da
        da setOutputRange(). 
        */
        mioForno.impostaPotenza(tunerOutput);

        // Questo loop deve funzionare alla stessa velocità del loop di controllo PID da regolare.
        while (micros() - microseconds < loopInterval) delayMicroseconds(1);
    }

    // Disattivare l'uscita in questo punto.
    //doSomethingToSetOutput(0);
    tunerOutput = 0;

    // Ottenere i guadagni PID - impostare i guadagni del controllore PID su questi valori
    tuner_kp = tuner.getKp();
    tuner_ki = tuner.getKi();
    tuner_kd = tuner.getKd();
    Serial.println("Ciclo di Tuning: guadagno proporzionale: " + String(tuner_kp,2) + " - guadagno integrale: " + String(tuner_ki,2) + " - guadagno derivativo: " + String(tuner_kd,2));
    //  *** TIMER ***
    //Congigurazione del TIMER per richiamare la fc di diagnostica
    TimerFcToHMI.setInterval(1000);
    TimerFcToHMI.expiredHandler(FcToHmi);
    TimerFcToHMI.start();

 

    // CANCELLATO
    /*
    SetNoiseBand(double x); Questo parametro definisce la "banda di rumore" e rappresenta la massima variazione 
    che può essere tollerata prima che l'algoritmo di autotuning consideri il sistema stabile. 
    Se il segnale di ingresso varia meno della banda di rumore, viene considerato come rumore 
    di misura e ignorato. Questo parametro aiuta a ridurre l'effetto del rumore sulla stima 
    del modello del sistema.
    
    myPIDAtune.SetNoiseBand(1);

    
    SetOutputStep(double x). Questo parametro specifica la dimensione del passo (step) che verrà utilizzato per rilevare 
    la risposta in uscita del sistema durante l'autotuning. Un passo più grande può accelerare 
    il processo di autotuning, ma potrebbe influenzare la stabilità del sistema. 
    Un valore più piccolo può rendere l'autotuning più lento, ma può essere più preciso.
    
    myPIDAtune.SetOutputStep(1);

    
    Questo parametro definisce il periodo di tempo (in secondi) su cui l'algoritmo di autotuning 
    guarda indietro per analizzare la risposta in uscita del sistema. Un periodo di guardia più 
    lungo può catturare meglio le dinamiche a lungo termine del sistema, ma potrebbe rendere 
    l'autotuning più lento.
    
    myPIDAtune.SetLookbackSec(5);
    */
   Serial.println("Setup completato");
}


//   ******* LOOP *******
void loop()
{
    
    if (mioForno.stato())                                         //se il forno è acceso
    {
        mioForno.aggiorna();                                    //aggiorna temperatura del forno
        temperaturaAttuale = mioForno.ottieniTemperatura();     //leggo la temperatura attuale del forno
        PidOutput = mioPID.Run(temperaturaAttuale);             //iterazione PID e prendo il suo output
        mioForno.impostaPotenza(PidOutput);          //l'uscita del PID è la regolazione del forno
    }
    else
    {
        mioForno.accendi();     //accendo il forno
    }

    TimerFcToHMI.run();         //diagnostica  
    
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
    Serial.println("3 - Valori Diagnostica Forno: Potenza - " + String(valoriRX.Potenza));
    Serial.println("4 - Valori Diagnostica Forno: Riscaldamento - " + String(valoriRX.Riscaldamento));
    Serial.println("5 - Valori Diagnostica Forno: Raffreddamento - " + String(valoriRX.Raffreddamento));
    Serial.println("6 - Valori Diagnostica Forno: Temperatura - " + String(valoriRX.Temperatura));
    Serial.println("7a - Il guadagno proporzionale del PID vale : " + String(kp));
    Serial.println("7b - Il guadagno integrale del PID vale : " + String(ki));
    Serial.println("7c - Il guadagno derivativo del PID vale : " + String(kd));
    Serial.println("-------------------------------------------------------------");
}