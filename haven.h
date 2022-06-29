// Definitie van klasse Haven

#ifndef HavenHVar  // voorkom dat dit bestand meerdere keren
#define HavenHVar  // ge-include wordt

#include "constantes.h"
#include <vector>
#include <utility>  // lijkt soms nodig voor pair
using namespace std;

class Haven
{ public:

    // Default constructor
    Haven ();

    // Lees de gegevens van een haven in. Achtereenvolgens:
    // * een regel met een geheel getal: de breedte van
    //     het haventerrein
    // * een regel met een geheel getal: het aantal containers N
    // * een regel met N gehele getallen: de lengtes van de containers,
    //   in volgorde van nummering van de containters
    // * een regel met een geheel getal: de vaste ruimte s tussen twee
    //   containers in dezelfde rij
    // * een regel met een getal (NIET PER SE EEN GEHEEL GETAL):
    //   de constante c waarmee de rijkosten worden vermenigvuldigd
    // * een regel met een geheel getal: het aantal kranen
    // * voor elke kraan, in volgorde van nummering van de kranen:
    //   een regel met N getallen (NIET PER SE GEHELE GETALLEN):
    //   de operationele kosten om met deze kraan de verschillende containers
    //   op hun plaats te zetten, in volgorde van nummering van de containers
    // Getallen op dezelfde regel worden gescheiden door enkele spaties
    // (dat wil zeggen: steeds een enkele spatie tussen twee getallen).
    // Controleer of
    // * het bestand invoernaam te openen is
    // * het aantal containers en het aantal kranen binnen de grenzen
    //   van de opdracht vallen
    // * elke afzonderlijke container minstens lengte 1 heeft en binnen
    //   de breedte van het terrein past (de containers mogen niet te lang
    //   zijn)
    // * alle andere getallen niet negatief zijn (0 mag wel)
    // Retourneer:
    // * true, als alle controles goed uitpakken.
    // * false, anders
    // Post:
    // * Als returnwaarde true is, is alle ingelezen informatie in het object
    //   opgeslagen.
    // * De returnwaarde is ook in een membervariabele opgeslagen, zodat
    //   bij andere memberfuncties gecontroleerd kan worden of er wel een
    //   instantie is waarvoor die functies kunnen worden uitgevoerd.
    bool leesInInstantie (const char* invoerNaam);

    // Genereer een instantie met waardes voor b, N, s, c en K uit
    // de parameters, met random lengtes (van de containers) tussen
    // ogLengte en bgLengte, en random operationele kosten tussen ogOpKosten
    // en bgOpKosten (og=ondergrens, bg=bovengrens).
    // Sla alles op in het object.
    // Deze functie is nodig voor het experiment in de opdracht.
    // BIJ DEZE FUNCTIE MAG JE ERVAN UITGAAN DAT DE PARAMETERS BINNEN
    // DE GRENZEN VAN DE OPDRACHT LIGGEN. JE HOEFT DAT DUS NIET TE
    // CONTROLEREN.
    // Post:
    // * De gegenereerde instantie is opgeslagen in het object.
    // * Het is ook in een membervariabele opgeslagen dat er een instantie
    //   is, waarvoor andere memberfuncties kunnen worden uitgevoerd.
    void genereerInstantie (int b, int N, int ogLengte, int bgLengte,
      int s, double c, int K, double ogOpKosten, double bgOpKosten);

    // Druk alle gegevens van de instantie overzichtelijk af op het scherm.
    // Dit betreft de gegevens zoals ze in een invoer-tekstbestand staan.
    // Het gaat dus nog niet om (de kosten van) een optimale plaatsing
    // van de containers.
    // Controleer eerst of er al een instantie is.
    void drukAfInstantie ();

    // Bepaal rechtstreeks recursief de minimale totale kosten voor
    // het plaatsen van de containers uit de instantie.
    // Controleer eerst of er al een instantie is.
    // Retourneer:
    // * Als er al een instantie is: de minimale kosten voor
    //   het plaatsen van de containers.
    // * Als er nog geen instantie is: een passende default waarde.
    double bepaalMinKostenRec ();

    // Bepaal met top-down dynamisch programmeren de minimale totale kosten
    // voor het plaatsen van de containers uit de instantie.
    // Controleer eerst of er al een instantie is.
    // Retourneer:
    // * Als er al een instantie is: de minimale kosten voor
    //   het plaatsen van de containers.
    // * Als er nog geen instantie is: een passende default waarde.
    double bepaalMinKostenTD ();

    // Bepaal met bottom-up dynamisch programmeren de minimale totale kosten
    // voor het plaatsen van de containers uit de instantie.
    // Controleer eerst of er al een instantie is.
    // Retourneer:
    // * Als er al een instantie is: de minimale kosten voor
    //   het plaatsen van de containers.
    // * Als er nog geen instantie is: een passende default waarde.
    // Post:
    // * Als er al een instantie is, bevat parameter plaatsing
    //   een specificatie van een optimale plaatsing: voor elke rij
    //   (in volgorde rij 0, rij 1, rij 2, enzovoort) een paar met:
    //   - de kraan die gebruikt wordt om containers in die rij te plaatsen
    //   - de eerste container die in die rij geplaatst wordt
    //   Het eerste paar in de vector bevat dus (als tweede onderdeel)
    //   container 1.
    double bepaalMinKostenBU (vector<pair <int,int> > &plaatsing);

    // Druk de plaatsing van containers, zoals beschreven in parameter
    // plaatsing, overzichtelijk af op het scherm, met voor elke rij:
    // - de kraan die de containers in die rij plaatst
    // - de nummers van de containers die in die rij geplaatst worden
    // Controleer eerst of er al een instantie is.
    // Pre:
    // * Als er een instantie gelezen is, beschrijft parameter plaatsing
    //   een geldige plaatsing van de containers voor die instantie.
    void drukAfPlaatsing (vector<pair <int,int> > &plaatsing);

    int b; //breedte haven
    int N; // aantal containers
    int ogLengte; //
    int bgLengte;
    int s;
    double c;
    int K;
    double ogOpKosten;
    double bgOpKosten;

/* Ingelezen parameters */
    int breedteHaven;
    int aantalContainers;
    int lengtes[MaxN];
    int vasteRuimte;
    double rijKostenConstante;
    int aantalKranen;
    double operationeleKosten[MaxK][MaxN];

/* Toegevoegde variabelen */
    bool haveHaven;
    int containerCombinaties[MaxN][MaxN];
    double rijKosten[MaxN][MaxN];
    double kraanKosten;

  private:

  void containerPlaatsenRij();

  /* Berekent voor elke i en j met 1 <= i <= j <= N de waarde
  rijkosten(i,j): de rijkosten bestaande uit containers i tot
  en met j
    - als er aan het eind van een rij nog δ ruimte over is,
    dan bedragen de rijkosten c · δ2 voor een constante c*/
  void rijKostenBerekenen(int aantalContainers, int lengtes[], int vasteRuimte,
    int rijKosten, int breedteHaven);

  /* Berekent voor elke k met 1 <= k <= K en voor elke i en j
  met 1 <= i <= j <= N de waarde kraankosten (k, i, j): de
  totale operationele kosten voor kraan k om containers i tot
  en met j op hun plaats te zetten */
  double kraanKostenBerekenen(int c1, int c2, int kraan);

  /* Berekent de totaalkosten(k,j), welke de minimale kosten
  zijn om met kranen 1 tot en met k containers 1 tot en met j
  op hun plaats te zetten volgens de regels uit de specificatie.
    - totaalkosten(k,j) is dus de oplossing voor het deelprobleem
    met de eerste k kranen en de eerste j containers
    - Ook gedefinieerd voor k = 0 en j = 0
    - Niet per se nodig dat alle k kranen worden gebruikt
    - Zoeken uiteindelijke totaalkosten (K, N).
    - Bereken waardes totaalkosten(k,j) met dynamisch programmeren
    in een recursieve formulering waaraan tk(k,j) voldoet!
    - Oftewel, druk waarde uit in kleinere deelproblemen
      o totaalkosten(k',j') met k' < k en/of j' < j.
      o kan gebruik maken van rijkosten(.) en kraankosten(.)
    - Hint: gebruiken we kraan k wel of niet? Indien wel, welke
    containers allemaal samen met container j in de laatste rij
    van de oplossing staan. Wat zijn de totale kosten bij elke
    mogelijkheid?
   */
  double totaalKostenBerekenen();





// TODO: uw eigen private memberfuncties en membervariabelen

};

#endif
