#include "Forno.h"


// Costruttore
Forno::Forno(double tassoRiscaldamento, double tassoRaffreddamento)
    , _tassoRiscaldamento(tassoRiscaldamento)
    , _tassoRaffreddamento(tassoRaffreddamento) {}

void Forno::aggiorna() {
    if (stato == ACCESO) {
        // Simulo un aumento di temperatura basato su una funzione logistica. Il riscaldamento è legato alla potenza impostata del forno.
        _riscaldamento = 100.0 * _potenzaPercentuale * (1 - exp(-_tassoRiscaldamento * millis() / 1000.0));
    }
    // Simulo il raffreddamento con una funzione logistica. Il raffreddamento è legato alla temperatura. Maggiore temperatura significa maggior raffreddamento
    _raffreddamento = 100.0 * exp(-_tassoRaffreddamento * millis() / 1000.0) * (_temperatura / 100.0);
    _temperatura = _riscaldamento - _raffreddamento;

    //clippo valori di uscita tra 20°C e 300°C
    _temperatura = (_temperatura > _max_temp) ? _max_temp : _temperatura;
    _temperatura = (_temperatura < _min_temp) ? _min_temp : _temperatura;
}

void Forno::accendi() { _stato = ACCESO; }
void Forno::spento() { _stato = SPENTO; }

bool Forno::stato() { return _stato == ACCESO; }

void Forno::impostaPotenzaPercentuale(double potenza) {
    _potenzaPercentuale = fmin(fmax(_potenza, 0.0), 100.0) / 100.0;
}

double Forno::temperatura() { return _temperatura; }
