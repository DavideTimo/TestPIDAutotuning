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

// attenzione. Dichiaro questa struct fuori dalla classe, in modo che il tipo abbia lo scopo che sia utilizzabile sia all'interno del programma principale, che dentro la classe Forno.
struct valoriDiagnostica {
    double Potenza;
    double Riscaldamento;
    double Raffreddamento;
    double Temperatura;
};

class Forno {
    //attenzione. L'enum va dichiarata prima di dichiarare una variabile di questo tipo, altrimenti giustamente impazzisce.

    public:
        enum statoForno {
            ACCESO,
            SPENTO
        };
        
    private:
        double _temperatura;          //temperatura attuale del forno
        double _potenza;              //potenza del forno settata (in futuro dinamicamente dal PID)
        double _riscaldamento;        //variazione temperatura causata dall'azione di riscaldamento
        double _raffreddamento;       // variazione temperatura causata dall'azione di raffreddamento
        double _tassoRiscaldamento;   //una caratteristica del forno, un k che definisce il tasso di riscaldamento. 
        double _tassoRaffreddamento;  //una caratteristica del forno, un k che definisce il tasso di raffreddamento. 
        statoForno _stato;            // enum, acceso o spento

    public:
        Forno( double tassoRiscaldamento, double tassoRaffreddamento );  //costruttore
        void aggiorna();                 //aggiorna la temperatura del forno
        double ottieniTemperatura();     //funzione che restituisce la temperatura attuale del forno
        bool stato();                    //funzione che restituisce true se lo stato del forno è acceso
        void accendi();                  //funzione per accendere (acceso/spento) il forno
        void spegni();                   //funzione per spegnere (acceso/spento) il forno
        void impostaPotenza(double potenza); //funzione che imposta la potenza di riscaldamento del forno
        valoriDiagnostica getDiagnostica();             //funzione che ritorna la struct con dentro i valori di diagnostica
        void resetForno();                // resetta la temperatura del forno
};

#endif  // FORNO_H