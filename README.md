# dr-nare-grupp-1
Drönarprojekt med esp32

Felsökningsdokumentation

I början av projektet blev våra originalkomponenter stulna, vilket ledde till att vi fick ett ersättningskit som behövde felsökas för att fungera korrekt. Vi bytte bland annat ut vår ursprungliga drönare mot en som Sebastian tidigare använt i sitt gymnasiearbete. Trots detta kvarstod problemen med motorstyrningen – motorerna svarade fortfarande inte konsekvent på styrsignaler.
Genomförda felsökningssteg:

    Komponentbyten:

        Två olika ESP32-moduler har testats.

        Två olika uppsättningar motorer har använts (tre totalt om man räknar med de stulna).

        Två olika ESC:er har testats (tre totalt inklusive de ursprungliga).

    Symptom som observerats:

        Okontrollerade vibrationer från motorerna.

        Pipande ljud från motorerna (se bifogad video).

        Oförutsägbart eller uteblivet svar på styrsignaler.

    Kodtester:

        Vi har provat flera varianter av kod, bland annat med:

            Servo-biblioteket

            DShot-protokoll

            Kod som fungerat för två andra grupper – men som inte fungerade i vårt fall.

    Övriga åtgärder:

        En kondensator kopplades in för att stabilisera spänningen, men detta löste inte problemet.

        Oscilloskop användes för att analysera de signaler som skickades till motorerna.

        En multimeter användes också för att försöka mäta signalerna.

Vi misstänker att problemet kan ligga i samspelet mellan vår ESP32 och de specifika komponenterna i det nya kitet, eftersom Sebastians drönare fungerade utan problem med hans egen flight controller.
