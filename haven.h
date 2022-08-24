// Definitie van klasse Haven

#ifndef HavenHVar  // voorkom dat dit bestand meerdere keren
#define HavenHVar  // ge-include wordt


#define INF numeric_limits<double>::infinity()

#include "constantes.h"
#include <vector>
#include <utility>  // lijkt soms nodig voor pair
using namespace std;

enum Actie { unknown, nieuwe_rij, container_plaatsen, nieuwe_kraan, klaar };

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




  private:
/* Ingelezen parameters */
      int lengte[MaxN];
      double operationeleKosten[MaxK][MaxN];

  /* Toegevoegde variabelen */
    bool haveHaven;
    double totaalKosten[MaxK][MaxN][MaxN];

    double tableBU[MaxN][MaxN][MaxK];
    Actie actionTableBU[MaxN][MaxN][MaxK];

    pair<int, int> cachedKraanContainerParen[MaxN];
    int cachedRijKosten[MaxN];

  /* Geeft de begincontainer bij de eindcontainer */
    double recStartRij(int j, int k);
    double recHalverwegeRij(int i, int j, int k);
    double tdStartRij(int j, int k);
    double tdHalverwegeRij(int i, int j, int k);

  /* Berekent voor elke i en j met 1 <= i <= j <= N de waarde
  rijkosten(i,j): de rijkosten bestaande uit containers i tot
  en met j
    - als er aan het eind van een rij nog δ ruimte over is,
    dan bedragen de rijkosten c · δ2 voor een constante c*/
  int rijKosten(int beginContainer, int eindContainer);

  /* Berekent voor elke k met 1 <= k <= K en voor elke i en j
  met 1 <= i <= j <= N de waarde kraankosten (k, i, j): de
  totale operationele kosten voor kraan k om containers i tot
  en met j op hun plaats te zetten */
  double kraanKosten(int c1, int c2, int kraan);

  int rijKostenRaw(int rij, vector<pair<int, int>> &plaatsing);
  bool kanContainerInRijPlaatsen(vector<pair <int,int>> &plaatsing);

  int rijBreedte(int van, int tot);
  int ruimteOver(int van, int tot);
  double rijKosten2(int van, int tot);


};

#endif
