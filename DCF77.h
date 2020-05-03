#ifndef _DCF77_H_
#define _DCF77_H_

#include <stdio.h>

//Struktur welche die rohen DCF77 Bits beinhaltet
typedef struct
{
    u_int64_t pruefBit : 1;
    u_int64_t meteo : 14;

    u_int64_t rufBit : 1;
    u_int64_t zeitumstellung : 1;
    u_int64_t mesz : 1;
    u_int64_t mez : 1;
    u_int64_t schaltsekunde : 1;

    u_int64_t pruefBitZeit : 1;
    u_int64_t minute : 7;
    u_int64_t minuteParitaet : 1;
    u_int64_t stunde : 6;
    u_int64_t stundeParitaet : 1;

    u_int64_t kalenderTag : 6;
    u_int64_t wochenTag : 3;
    u_int64_t kalenderMonat : 5;
    u_int64_t kalenderJahr : 8;
    u_int64_t datumParitaet : 5;

} DCF77;

//Struktur welche allgemeine Infos, sowie die rohen und bearbeiteten Daten jedes DCF77 Werts behinhaltet
typedef struct
{
    char name[20];
    u_int8_t typ : 2;
    char beschreibung[80];
    u_int8_t laenge;
    u_int8_t defaultValue;
    u_int8_t skipThisValue;
    u_int64_t rawData;
    u_int32_t processedData;

} auswertungDCF77;

//Pointer zur DCF77 Bitstruktur
DCF77 auswertung;
DCF77 *ptrDCF77 = &auswertung;

#endif