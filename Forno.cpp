#include "Forno.h"


// Costruttore
Forno::Forno(double tassoRiscaldamento, double tassoRaffreddamento)
    : _temperatura(0.0), _tassoRiscaldamento(tassoRiscaldamento), _tassoRaffreddamento(tassoRaffreddamento) {
        _stato = SPENTO;
    }


void Forno::aggiorna(){

    if (_stato == ACCESO)
    {
        // Simulo un aumento di temperatura basato su una funzione logistica. Il riscaldamento è legato alla potenza impostata del forno.
        _riscaldamento = (_potenza * _tassoRiscaldamento)/10;
    }
    

    // Simulo il raffreddamento con una funzione logistica. Il raffreddamento è legato alla temperatura. Maggiore temperatura significa maggior raffreddamento
    _raffreddamento = (_tassoRaffreddamento * _temperatura / 3)/10;
    
    //aggiorno la temperatura
    _temperatura = _temperatura + _riscaldamento - _raffreddamento;

    //clippo valori di uscita tra 20°C e 300°C
    if (_temperatura > 300.0) {
        _temperatura = 300.0;
    }

    if (_temperatura < 20.0) {
        _temperatura = 20.0;
    }
}

void Forno::accendi() { _stato = ACCESO; }

void Forno:: spegni() { _stato = SPENTO; }

bool Forno::stato() { return _stato==ACCESO; }


void Forno::impostaPotenza(double potenza){
    // Clippo la potenza in modo che sia compresa tra 0 e 100
    _potenza = potenza;
    
}

double Forno::ottieniTemperatura() {
    // Funzione per ottenere la temperatura attuale del forno
    return _temperatura;
}

valoriDiagnostica Forno::getDiagnostica(){
    // Funzione che restituisce una struct con dentro i valori di diagnostica del forno
    valoriDiagnostica valori;
    valori.Potenza = _potenza;
    valori.Riscaldamento = _riscaldamento;
    valori.Raffreddamento = _raffreddamento;
    valori.Temperatura = _temperatura;

    return valori;
}