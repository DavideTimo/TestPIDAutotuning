/*
Oggetto forno per simulare funzionamento fisico di riscaldamento/raffreddamento. E' molto semplificato. Servirà per fare due test
con un PID.
Il forno si scalda se acceso e se viene settata una potenza diversa (maggiore) a zero.
Indipendentemente che sia attivo il riscaldamento o meno, il forno subisce anche un fenomeno di raffreddamento. 
La funzione aggiorna() si occupa di modificare la temperatura del forno. 

*/ 

#ifndef FORNO_H
#define FORNO_H
#include "Arduino.h"

class Forno {
    //attenzione. La struttura dell'enum va dichiarata prima di dichiarare una variabile di questo tipo, altrimenti giustamente impazzisce.


    private:

        enum statoForno {
            ACCESO,
            SPENTO
        };

        double temperatura;
        double potenzaPercentuale;  //potenza del forno settata (in futuro dinamicamente dal PID)
        double riscaldamento;       //variazione temperatura causata dall'azione di riscaldamento
        double raffreddamento;      // variazione temperatura causata dall'azione di raffreddamento
        double tassoRiscaldamento;  //una caratteristica del forno, un k che definisce il tasso di riscaldamento. 
        double tassoRaffreddamento; //una caratteristica del forno, un k che definisce il tasso di raffreddamento. 
        statoForno stato;           // enum, acceso o spento

    public:
        Forno( double tassoRiscaldamento, double tassoRaffreddamento );  //costruttore
        void aggiorna();  //aggiorna la temperatura del forno
        double ottieniTemperatura();  //funzione che restituisce la temperatura attuale del forno
        statoForno ottieniStato(); //funzione che restituisce lo stato (acceso/spento) del forno
        void impostaStato(statoForno nuovoStato); //funzione per settare lo stato (acceso/spento) del forno
        void impostaPotenzaPercentuale(double potenza); //fuznione che imposta la potenza di riscaldamento del forno
        
};

#endif  // FORNO_H