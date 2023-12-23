#include "Forno.h"


// Costruttore
Forno::Forno(double tassoRiscaldamento, double tassoRaffreddamento)
    : _temperatura(0.0), _tassoRiscaldamento(tassoRiscaldamento), _tassoRaffreddamento(tassoRaffreddamento) {
        stato = SPENTO;
    }


void Forno::aggiorna(){

    if (stato == ACCESO)
    {
        
        // Simulo un aumento di temperatura basato su una funzione logistica. Il riscaldamento è legato alla potenza impostata del forno.
        //riscaldamento = 100.0 * potenzaPercentuale * (1 - exp(-tassoRiscaldamento / 1000.0));
        _riscaldamento = (_potenzaPercentuale * _tassoRiscaldamento)/10;
    }
    

    // Simulo il raffreddamento con una funzione logistica. Il raffreddamento è legato alla temperatura. Maggiore temperatura significa maggior raffreddamento
    //raffreddamento = 100.0 * exp(-tassoRaffreddamento * millis() / 1000.0)* (temperatura / 100.0);
    _raffreddamento = (_tassoRaffreddamento * _temperatura / 3)/10;
    
    _temperatura = _temperatura + _riscaldamento - _raffreddamento;

    //clippo valori di uscita tra 20°C e 300°C
    if (_temperatura > 300.0) {
        _temperatura = 300.0;
    }

    if (_temperatura < 20.0) {
        _temperatura = 20.0;
    }
}

void Forno::accendi(){ stato = ACCESO;}

void Forno:: spegni(){ stato = SPENTO; }

bool Forno::Stato(){ return stato==ACCESO; }


void Forno::impostaPotenzaPercentuale(double potenza){
    // Clippo la potenza in modo che sia compresa tra 0 e 100
    _potenzaPercentuale = potenza;
    
}

double Forno::ottieniTemperatura() {
    // Funzione per ottenere la temperatura attuale del forno
    return _temperatura;
}

valoriDiagnostica Forno::getDiagnostica(){
    // Funzione che restituisce una struct con dentro i valori di diagnostica del forno
    valoriDiagnostica valori;
    valori.Potenza = _potenzaPercentuale;
    valori.Riscaldamento = _riscaldamento;
    valori.Raffreddamento = _raffreddamento;
    valori.Temperatura = _temperatura;

    return valori;
}