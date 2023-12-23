#include "Forno.h"


// Costruttore
Forno::Forno(double tassoRiscaldamento, double tassoRaffreddamento)
    : temperatura(0.0), tassoRiscaldamento(tassoRiscaldamento), tassoRaffreddamento(tassoRaffreddamento) {
        stato = SPENTO;
    }


void Forno::aggiorna(){

    if (stato == ACCESO)
    {
        
        // Simulo un aumento di temperatura basato su una funzione logistica. Il riscaldamento è legato alla potenza impostata del forno.
        //riscaldamento = 100.0 * potenzaPercentuale * (1 - exp(-tassoRiscaldamento / 1000.0));
        riscaldamento = potenzaPercentuale * tassoRiscaldamento;
    }
    

    // Simulo il raffreddamento con una funzione logistica. Il raffreddamento è legato alla temperatura. Maggiore temperatura significa maggior raffreddamento
    //raffreddamento = 100.0 * exp(-tassoRaffreddamento * millis() / 1000.0)* (temperatura / 100.0);
    raffreddamento = tassoRaffreddamento * temperatura / 3;
    
    temperatura = temperatura + riscaldamento - raffreddamento;

    //clippo valori di uscita tra 20°C e 300°C
    if (temperatura > 300.0) {
        temperatura = 300.0;
    }

    if (temperatura < 20.0) {
        temperatura = 20.0;
    }
}

void Forno::accendi(){
    stato = ACCESO;
}

void Forno:: spegni(){
    stato = SPENTO;
}

Forno::statoForno Forno::ottieniStato(){
    return stato;
}


void Forno::impostaPotenzaPercentuale(double potenza){
    // Clippo la potenza in modo che sia compresa tra 0 e 100
    potenzaPercentuale = fmin(fmax(potenza, 0.0), 100.0) / 100.0;
    
}

double Forno::ottieniTemperatura() {
    // Funzione per ottenere la temperatura attuale del forno
    return temperatura;
}

valoriDiagnostica Forno::getDiagnostica(){
    // Funzione che restituisce una struct con dentro i valori di diagnostica del forno
    valoriDiagnostica valori;
    valori._potenza = potenzaPercentuale;
    valori._riscaldamento = riscaldamento;
    valori._raffreddamento = raffreddamento;
    valori._temperatura = temperatura;

    return valori;
}