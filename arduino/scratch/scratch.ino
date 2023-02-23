//0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include <math.h>
// header file della libreria standard del C che contiene definizioni di macro, costanti,
//  dichiarazioni di funzioni e tipi usati per le operazioni matematiche.
// viene implementato questo header file per consentire l'utilizzo del logaritmo naturale,
//  presente nella formula per determinare la temperatura

/*
 * PARAMETRI COSTANTI PER LA COMUNICAZIONE CON IL PROGRAMMA JAVA
 */

#define ready 'R'           //carattere che indica lo stato "Pronto"
#define delimiter_open '['  //carattere che indica l'inizio di trasmissione da parte di Arduino
#define delimiter_close ']' //carattere che indica la fine della trasmissione da parte di Arduino

/*
 * PARAMETRI PER LA LETTURA E CONVERSIONE DELLA TEMPERATURA
 */

#define TL082CP_out 0    // viene definita e inizializzata la variabile 'TL082CP_out' con il valore 0,
                         //  che corrisponde al pin analogico 'A0'

/*
Definizione delle costanti: 
(in quanto non verranno modificate nel corso dell'esecuzione del programma, ma bensì utilizzate per il
calcolo della temperatura finale)
*/

const unsigned int Vin = 5;   // viene definita la variabile Vin e inizializzata con il valore
                              //  di tensione corrispondente a quello fornito in ingresso al circuito
const unsigned int R0 = 330;  // viene definita la variabile R0 e inizializzata con il valore
                              //  corrispondente a quello delle due resistenze
                              //  in ingresso dell'operazionale
const unsigned int Rth0 = 50; // viene definita la variabile Rth0 e inizializzata con il valore
                              //  corrispondente alla resistenza nominale del termistore alla
                              //  temperatura di riferimento
const float K = 273.15;       // viene definita la variabile K e inizializzata con il valore
                              //  corrispondente ai zero gradi Celsius espressa in kelvin
                              //  (utile per una successiva conversione)
const float T2 = 298.15;      // viene definita la variabile T2 e inizializzata con il valore
                              //  corrispondente alla temperatura di riferimento
                              //  (25°C) espressa in kelvin 
const int B = 4090;           // viene definita la variabile B e inizializzata con il valore
                              //  corrispondente al parametro caratteristico del termistore

/*
Definizione delle variabili: 
(in quanto verranno modificate nel corso dell'esecuzione del programma)
*/

float Rth;      // viene definita la variabile Rth che corrisponde al valore
                //  di resistenza del termistore
float T1;       // viene definita la variabile T1 che corrisponde al valore di temperatura corrente
                //  espressa in gradi kelvin
float T;        // viene definita la variabile T che corrisponde al valore di temperatura corrente
                //  espressa in gradi celsius
float val = 0;  // viene definita la variabile val che andrà ad assumere il valore restituito
                //  dal comando analogRead, di conseguenza essa può assumere tutti i possibili valori
                //  compresi tra l'intervallo [0, 1023]
float Vout;     // viene definita la variabile Vout che verrà successivamente inizializzata
                // con il valore di tensione corrispondente a quello fornito in uscita dal circuito


void setup() // funzione invocata una sola volta all'inizio del programma
{
  Serial.begin(9600); // comando per permettere la comunicazione con il Monitor Seriale,
                      //  viene impostata la velocità di trasmissione dei dati
                      //  pari a 9600 bit per secondo
  while (!Serial) {
    // si attende che venga instaurata la connessione seriale
  }
  Serial.write(ready); 
}

// funzione utilizzata per convertire un numero di tipo float in uno in binario formato 
//  da quattro otteti
void sentfloat(float v)
{
  unsigned char c[sizeof(v)];        // viene definita una lista contenente tante variabili di tipo
                                     //  carattere senza segno, quante la lunghezza in byte di una
                                     //  variabile float (4 byte) 
  memcpy(c, &v, sizeof(v));          // il valore float contenuto nella variabile 'v' viene separato
                                     //  in 4 byte e copiato nella lista 'c'
  Serial.write(delimiter_open);      // viene inviato sulla seriale il carattere che identifica
                                     //  l'inizio della trasmissione dati  
  for (int i = 0; i< sizeof(v); i++) // ciclo for per inviare tutti i byte che formano il dato float
    Serial.write(c[i]);              // invio del singono otteto di bit
  Serial.write(delimiter_close);     // viene inviato sulla seriale il carattere che identifica 
                                     //  la chiusura della trasmissione dati
}

void loop() // funzione invocata ripetutamente
{
  // viene controllato che ci siano dei byte da leggere tramite la seriale
  if (Serial.available() > 0) {
    byte incomingByte = Serial.read(); // viene letto il byte ricevuto
    // viene realizzato un costrutto condizionale di tipi switch sulla variabile incomingByte
    //  convertita in carattere
    switch (char(incomingByte)) 
    {
      // nel caso in cui char(incomingByte) corrisponda a 'T', allora verrà inviato sulla seriale il
      //  valore corrispondente alla temperatura letta
      case 'T':
          /*
           * Lettura della tensione in uscita + conversione
           */
          val = analogRead( TL082CP_out ); // viene letto il dato della tensione scalata sul
                                           //  pin 'TL082CP_out' e assegnato alla variabile 'val'
          Vout = val/1023 * Vin;           // viene assegnato alla variabile Vout il valore di
                                           //  tensione precedentemente letto e convertito in Volt
          /*
           * Calcoli interni
           */
          Rth = -(Vout * (R0 + Rth0))/Vin + Rth0; // tramite la formula teorica, viene assegnato il 
                                                  //  valore di resistenza del termistore alla 
                                                  //  variabile Rth
          T1 = B/(log(Rth/Rth0) + B/T2);          // tramite la formula teorica, viene assegnato il 
                                                  //  valore di temperatura espresso in Kelvin alla 
                                                  //  variabile T1
          T = T1 - K;                             // viene assegnato il valore della temperatura 
                                                  //  espressa in Celsius alla variabile T
          sentfloat(T);
          break;
    }
  }
}
