#include <DCF77.h>
#include <errno.h>
#include <string.h>

const char *const PATH = {"DCF77Sample.txt"};

//Setzt den angegebenen Abschnitt aus dem Buffer zu einer binären Zahl zusammen.
int concatBits(int start, int end, char buffer[])
{

    u_int16_t binary;
    int loops;
    int currentValue = start;

    if ((end - start) == 0)
    {
        loops = 1;
    }
    else
    {
        loops = (end - start) + 1;
    }

    for (size_t i = 0; i < loops; i++)
    {
        if (i == 0)
        {
            binary = buffer[currentValue] % 2 << i;
        }
        else
        {
            binary = binary | buffer[currentValue] % 2 << i;
        }

        //printf("%x", buffer[currentValue] % 2);
        currentValue++;
    }

    //printf("   BITS: %d\n", binary);

    return binary;
};

//Befüllt die DCF77 Bitstruktur für jede augelesene Zeile der Datei
int loadData()
{

    FILE *fp = {NULL};

    fp = fopen(PATH, "rb");

    if (!fp)
    {
        puts("EMPTY");
        return ENOENT;
    }
    else
    {
        char buffer[1024];

        while (fgets(buffer, sizeof(buffer), fp))
        {

            //Befüllen der Bitsruktur via Pointer
            ptrDCF77->pruefBit = concatBits(0, 0, buffer);
            ptrDCF77->meteo = concatBits(1, 14, buffer);
            ptrDCF77->rufBit = concatBits(15, 15, buffer);
            ptrDCF77->zeitumstellung = concatBits(16, 16, buffer);
            ptrDCF77->mesz = concatBits(17, 17, buffer);
            ptrDCF77->mez = concatBits(18, 18, buffer);
            ptrDCF77->schaltsekunde = concatBits(19, 19, buffer);
            ptrDCF77->pruefBitZeit = concatBits(20, 20, buffer);
            ptrDCF77->minute = concatBits(21, 27, buffer);
            ptrDCF77->minuteParitaet = concatBits(28, 28, buffer);
            ptrDCF77->stunde = concatBits(29, 34, buffer);
            ptrDCF77->stundeParitaet = concatBits(35, 35, buffer);
            ptrDCF77->kalenderTag = concatBits(36, 41, buffer);
            ptrDCF77->wochenTag = concatBits(42, 44, buffer);
            ptrDCF77->kalenderMonat = concatBits(45, 49, buffer);
            ptrDCF77->kalenderJahr = concatBits(50, 57, buffer);
            ptrDCF77->datumParitaet = concatBits(58, 58, buffer);

            /*printf("\nPuefbit: %d, Meteo: %d, Rufbit: %d, MEZ/MESZ: %d, MESZ: %d, MEZ: %d\n"
                   "Schaltsekunde: %d, BeginZeit: %d, Minute: %d, PritaetMinute: %d\n"
                   "Stunde: %d, StundeParitaet: %d, Tag: %d\n"
                   "Wochentag: %d, Monat: %d,Jahr: %d, DatumParitaet: %d\n\n",
                   ptrDCF77->pruefBit, ptrDCF77->meteo, ptrDCF77->rufBit, ptrDCF77->zeitumstellung, ptrDCF77->mesz, ptrDCF77->mez,
                   ptrDCF77->schaltsekunde, ptrDCF77->pruefBitZeit, ptrDCF77->minute, ptrDCF77->minuteParitaet,
                   ptrDCF77->stunde, ptrDCF77->stundeParitaet, ptrDCF77->kalenderTag,
                   ptrDCF77->wochenTag, ptrDCF77->kalenderMonat, ptrDCF77->kalenderJahr,
                   ptrDCF77->datumParitaet);
            */

            //Verarbeitung der aktuellen Zeile / Bitstruktur
            processData();
        }

        fclose(fp);
    }

    return 0;
};

//Verarbeitet die aktuell befüllte Bitstruktur und gibt die gewünschten Werte aus
int processData()
{

    auswertungDCF77 DCF77Field[] = {
        {"pruefBit", 0, "Start einer neuen Minute", 1, 0, 1, ptrDCF77->pruefBit},
        {"Meteo", 1, "Beschreibung", 14, 2, 1, auswertung.meteo},
        {"Rufbit", 0, "Reserveantenne", 1, 2, 1, ptrDCF77->rufBit},
        {"Zeitumstellung", 0, "„1“: Am Ende dieser Stunde wird MEZ/MESZ umgestellt.", 1, 0, 1, ptrDCF77->zeitumstellung},
        {"MESZ", 0, "„1“: MESZ („0“: MEZ)", 1, 0, 0, ptrDCF77->mesz},
        {"MEZ", 0, "„1“: MEZ („0“: MESZ)", 1, 0, 0, ptrDCF77->mez},
        {"Schaltsekunde", 0, "„1“: Am Ende dieser Stunde wird eine Schaltsekunde eingefügt.", 1, 0, 1, ptrDCF77->schaltsekunde},
        {"pruefBitZeit", 0, "Beginn der Zeitinformation (ist immer „1“)", 1, 1, 1, ptrDCF77->pruefBitZeit},
        {"Minute", 1, "Minute in BCD", 7, 2, 0, ptrDCF77->minute},
        {"MinuteParitaetBit", 0, "Paritaet Minute", 1, 2, 1, ptrDCF77->minuteParitaet},
        {"Stunde", 1, "Stunde in BCD", 6, 2, 0, ptrDCF77->stunde},
        {"StundeParitaetBit", 0, "Pritaet Stunde", 1, 2, 1, ptrDCF77->stundeParitaet},
        {"KalenderTag", 1, "Kalendertag in BCD", 6, 2, 0, ptrDCF77->kalenderTag},
        {"WochenTag", 1, "Wochentag in BCD", 3, 2, 0, ptrDCF77->wochenTag},
        {"Monat", 1, "Monat in BCD", 5, 2, 0, ptrDCF77->kalenderMonat},
        {"Jahr", 1, "Jahr in BCD", 8, 2, 0, ptrDCF77->kalenderJahr},
        {"DatumParitaetBit", 0, "Paritaet Datum", 1, 2, 1, ptrDCF77->datumParitaet},
    };

    for (size_t index = 0; index < 17; index++)
    {
        if (DCF77Field[index].skipThisValue == 1)
        {
            continue;
        }

        //Unterscheidung zwischen einzelnem Bit und BCD Bits
        switch (DCF77Field[index].typ)
        {
        case 0:
            DCF77Field[index].processedData = DCF77Field[index].rawData;
            //printf(" %s : %d ", DCF77Field[index].name, DCF77Field[index].processedData);
            break;
        case 1:

            DCF77Field[index].processedData = convertToBCD(DCF77Field[index]);
            //printf(" %s : %d ", DCF77Field[index].name, DCF77Field[index].processedData);
            break;

        default:
            printf("Unkonwn typ. It is neither SingleBit nor BCD.");
            break;
        }
    }
    //printf("\n");

    printf("Datum: %02d.%02d.%02d | Wochentag: %d | Uhrzeit: %02d:%02d\n", DCF77Field[12].processedData, DCF77Field[14].processedData, DCF77Field[15].processedData,
           DCF77Field[13].processedData, DCF77Field[10].processedData, DCF77Field[8].processedData);

    return 0;
};

//Übersetzt den aktuellen Wert aus der Bitstruktur in eine BCD Zahl und gibt diese zurück
int convertToBCD(auswertungDCF77 currentField)
{

    u_int8_t counter = 0;
    u_int16_t binary[16];
    u_int8_t value = 0;
    u_int8_t bcd[8] = {1, 2, 4, 8, 10, 20, 40, 80};

    //Dezimal zu Binär
    while (currentField.rawData > 0)
    {
        binary[counter] = currentField.rawData % 2;
        currentField.rawData = currentField.rawData / 2;
        counter++;
    }
    if (counter < currentField.laenge && currentField.rawData == 0)
    {
        int offset = currentField.laenge - counter;
        for (size_t i = 0; i < offset; i++)
        {
            binary[counter] = 0;
            counter++;
        }
    }

    for (size_t i = 0; i < counter; i++)
    {
        if (binary[i] == 1)
        {
            value = value + bcd[i];
        }
    };
    return value;
};

int main()
{
    //Leeren der Bitstruktur
    memset(ptrDCF77, 0, sizeof(DCF77));
    
    //DCF77 Daten laden und verarbeiten
    loadData();

    return 0;
}