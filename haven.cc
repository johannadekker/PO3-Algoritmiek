// Implementatie van klasse Haven

#include <iostream>
#include <iomanip>
#include <climits>
#include <fstream>
#include "standaard.h"
#include "haven.h"
using namespace std;

//*************************************************************************

Haven::Haven ()
{

// TODO: implementeer zo nodig deze constructor

}  // default constructor


//*************************************************************************

bool Haven::leesInInstantie (const char* invoerNaam)
{
  ifstream haven;
  haven.open(invoerNaam, ios::in);
  /* Controleert of het bestand invoernaam te openen is */
  if (haven.fail()) {
    cerr << "Kon het bestand '" << invoerNaam << "' niet openen." << endl;
    return false;
  }
   /* Gegevens inlezen */
  haven >> breedteHaven >> aantalContainers;

  for (int i = 0; i < aantalContainers; i++) {
    haven >> lengte[i];
  }
  haven >> vasteRuimte >> rijKostenConstante >> aantalKranen;
  // array vullen
  for (int row = 0; row < aantalKranen; row++) {
    for (int col = 0; col < aantalContainers; col++) {
      haven >> operationeleKosten[row][col];
    }
  }
  /* Controleert of het aantal containers en het aantal kranen binnen de
  grenzen van de opdracht vallen */
  if (aantalContainers > MaxN || aantalKranen > MaxK) {
    return false;
  }
  /* Controleert of elke afzonderlijke container minstens lengte 1 heeft
  en binnen de breedte van het terrein past */
  for (int j = 0; j < aantalContainers; j++) {
    if (lengte[j] < 1 || lengte[j] > breedteHaven) {
      return false;
    }
  }
  /* Alle andere parameters moeten positieve getallen zijn */
  if ( breedteHaven < 0 || vasteRuimte < 0 || rijKostenConstante < 0) {
    return false;
  }
  for (int k = 0; k < aantalContainers; k++) {
    for (int r = 0; r < aantalKranen; r++) {
      if (operationeleKosten[k][r] < 0) {
        return false;
      }
    }
  }
  haveHaven = true;
  return true;
}  // leesInInstantie
//****************************************************************************

void Haven::genereerInstantie (int b, int N, int ogLengte, int bgLengte,
  int s, double c, int K, double ogOpKosten, double bgOpKosten)
{
  b = randomGetal(1, INT_MAX);
  N = randomGetal(1, 1024);
  ogLengte = 1;
  bgLengte = b;
  for (int n = 0; n < N; n++) {
    lengte[n] = randomGetal(ogLengte, bgLengte);
  };
  s = randomGetal(0, INT_MAX);
  c = randomGetal(0, INT_MAX);
  K = randomGetal(1, 512);
  ogOpKosten = 0;
  bgOpKosten = 10.0;
  for (int k = 0; k < K; k++) {
    for (int i = 0; i < N; i++) {
      operationeleKosten[k][i] = randomDouble(ogOpKosten, bgOpKosten);
    }
  }
}  // genereerInstantie

//*************************************************************************

void Haven::drukAfInstantie () {
  if (haveHaven == true) {
    cout <<"b = " << breedteHaven << endl;
    cout << "N = " << aantalContainers << endl;
    cout << "lengte[] = {";
    for (int i = 0; i < aantalContainers; i++){
      cout << lengte[i] << "}";
    }
    cout << endl;
    cout << "s = " << vasteRuimte << endl;
    cout << "c = " << rijKostenConstante << endl;
    cout << "K = " << aantalKranen << endl;
    cout << "opKosten[k][j] =" << endl;
    for (int k = 0; k < aantalKranen; k++) {
      for (int j = 0; j < aantalContainers; j++) {
        cout << operationeleKosten[k][j] << ' ';
        if (j == aantalContainers-1) {
          cout << endl;
        }
      }
    }
  }
}  // drukAfInstantie

//*************************************************************************
/*  Retourtneert de waarde totaalkosten(aantalKranen, aantalContainers).
Roept de recursieve hulpfunctie recStartRij(j,k) aan met (0,0). */

double Haven::bepaalMinKostenRec ()
{ //Controle of er een instantie is
  if (!haveHaven) {
    cerr << "Er is geen instantie." << endl;
    return 0;
  }

  return recStartRij(0,0);
} // bepaalMinKostenRec

//*************************************************************************

double Haven::recStartRij(int j, int k) {
  if (j >= aantalContainers) {
    return 0;
  }
  //kosten NADAT container is geplaatst in huidige rij
  double kostNaPlaatsen = kraanKosten(j, j, k) + recHalverwegeRij(j, j + 1, k);

  if (k < aantalKranen - 1) { //hoger genummerde kraan beschikbaar
    double kostNaNieuweKraan = recStartRij(j, k + 1);
    //lagere kosten bij inzetten huidige of hogere kraan?
    return min(kostNaPlaatsen, kostNaNieuweKraan);
  }

  return kostNaPlaatsen; //laatste kraan is ingezet
} //recStartRij

//*************************************************************************

double Haven::recHalverwegeRij(int i, int j, int k) {
  if (j >= aantalContainers) { //alle containers zijn geplaatst
    return 0;
  }
  //Plaats container op een nieuwe rij
  double kostNaNieuweRij = recStartRij(j, k) + rijKosten(i, j-1);

  if (rijKosten(i, j) < INT_MAX) { //container past in rij
    double kostNaAansluiten = kraanKosten(j, j, k) + recHalverwegeRij(i, j + 1, k);
    //lagere kosten bij aansluiten of nieuwe rij?
    return min(kostNaAansluiten, kostNaNieuweRij);
    }

  return kostNaNieuweRij; //container past niet in rij
} //recHalverwegeRij

//*************************************************************************

double Haven::bepaalMinKostenTD ()
{
}  // bepaalMinKostenTD

//*************************************************************************

double Haven::bepaalMinKostenBU (vector<pair <int,int> > &plaatsing)
{
    for (int i = 0; i < aantalContainers; i++)
    {
        if (!kanContainerInRijPlaatsen(plaatsing)) { // test als de huidige container niet in de huidige rij geplaatst kan worden.
            plaatsing.push_back(make_pair(0, plaatsing[plaatsing.size() - 1].second + 1)); // voeg de container dan aan de volgende rij.
            continue;
        }

        int rijKostenHuidigeRij = rijKostenRaw(plaatsing.size(), plaatsing); // rijkoste nvan het huidige rij
        int rijKostenExtraContainer = rijKostenHuidigeRij - lengte[plaatsing[plaatsing.size() - 1].second];// rijkosten als we een container erbij toevoegen
        int winstRuimteRij1 = (rijKostenHuidigeRij * rijKostenHuidigeRij - rijKostenExtraContainer * rijKostenExtraContainer) * rijKostenConstante;// de lege ruimte in een rij

        vector<pair<int, int>> containerToekomst1(plaatsing);
        int rijKostenNieuweRijExtraContainer = 0;
        containerToekomst1.push_back(make_pair(0, containerToekomst1[containerToekomst1.size() - 1].second + 1));// we voegen (nep) de container aan de rij
        //en we kijken naar 2 senarios, wanneer het WEL mogelijk is om de container te verplaatsen en wanneer NIET.
        while (kanContainerInRijPlaatsen(containerToekomst1)) {
            rijKostenNieuweRijExtraContainer += lengte[containerToekomst1[containerToekomst1.size() - 1].second];
            containerToekomst1[containerToekomst1.size() - 1].second += 1; // voeg container aan de tweede rij.
        }

        vector<pair<int, int>> containerToekomst2(plaatsing);
        int rijKostenNieuweRij = 0;
        containerToekomst2.push_back(make_pair(0, containerToekomst2[containerToekomst2.size() - 1].second + 2));
        while (kanContainerInRijPlaatsen(containerToekomst2)) {
            rijKostenNieuweRij += lengte[containerToekomst2[containerToekomst2.size() - 1].second];
            containerToekomst2[containerToekomst2.size() - 1].second += 1;// voeg container aan de huidige rij.
        }

        int winstRuimteRij2 = (rijKostenNieuweRij * rijKostenNieuweRij - rijKostenNieuweRijExtraContainer * rijKostenNieuweRijExtraContainer) * rijKostenConstante;

        if (winstRuimteRij2 > winstRuimteRij1) {
            plaatsing.push_back(make_pair(0, plaatsing[plaatsing.size() - 1].second + 1));
            continue;
        }

        plaatsing[plaatsing.size() - 1].second += 1;
        continue;
    }

    return 0.0;
}  // bepaalMinKostenBU

//*************************************************************************
// check of de huidige container die we gaan plaatsen inderdaad in de laattste beschikbare rij wordt geplaatst.
bool Haven::kanContainerInRijPlaatsen(vector<pair <int,int>> &plaatsing) {
    int kosten = rijKostenRaw(plaatsing.size(), plaatsing);
    int huidigeRijContainers = plaatsing[plaatsing.size() - 1].second - plaatsing[plaatsing.size() - 2].second;
    return kosten - huidigeRijContainers * vasteRuimte - lengte[plaatsing[plaatsing.size() - 1].second + 1] >= 0; // als de ruimte de over is groter is dan 0, dan betekent het dat de container geplaatst mag worden.
}

void Haven::drukAfPlaatsing (vector<pair <int,int> > &plaatsing)
{

// TODO: implementeer deze memberfunctie

}  // drukAfPlaatsing

//*************************************************************************

/* Deze functie creeërt een Boolean matrix die aangeeft of containers mogelijk
met elkaar in een rij kunnen worden geplaatst.
- de diagonaal is gevuld met 1 gezien de worst case is dat iedere container op
  een eigen rij wordt geplaatst
- Kolommen met alleen één 1 zijn containers die sowieso als eerste in een rij
  worden geplaatst. EG: container 1, een container met een grote lengte waarbij
  nooit een andere container erbij zal passen, etc.
- Doordat alle mogelijke combinaties van containers in rijen zijn gevonden, is
  het makkelijk om alle mogelijke rijkosten te berekenen. Dit is verantwoord om
  te doen omdat containers in volgorde van nummering worden geplaatst. Hierdoor
  is het aantal combinaties sterk begrensd.
*/



//*************************************************************************
/* "Voor elke k met 1 ≤ k ≤ K en voor elke i en j met 1 ≤ i ≤ j ≤ N de waarde
kraankosten(k,i,j) te berekenen: de totale operationele kosten voor kraan k
om containers i tot en met j op hun plaats te zetten."                 */

double Haven::kraanKosten(int i, int j, int k) {
  double kraanKosten = 0.0;
  //Indien invoer ongeldig is
  if (i < 0 || j >= aantalContainers || k < 0 || k >= aantalKranen) {
    return INT_MAX;
  }
  else {
    for (int c = i; c <= j ; c++) {
      kraanKosten += operationeleKosten[k][c];
    } //end for
      return kraanKosten;
  }
} //end kraanKostenBerekenen

//*************************************************************************
/* "Berekent voor elke i en j met 1 <= i <= j <= N de waarde
  rijkosten(i,j): de rijkosten bestaande uit containers i tot
  en met j"
    - als er aan het eind van een rij nog δ ruimte over is,
    dan bedragen de rijkosten c · δ2 voor een constante c
    - Gebruikt index posities, niet container nummering!                      */

int Haven::rijKosten(int i, int j) {
  int extraContainersInRij;
  //Indien invoer geldig is
  if (i + 1 > 0 && j < aantalContainers && i <= j) {
    extraContainersInRij = j - i;
    int benodigeTussenRuimte = extraContainersInRij * vasteRuimte;
    //Bereken de lengte van alle containers in de rij
    int totalelengteContainers = 0;
    for (int c = i; i <= j; c++) {
      totalelengteContainers += lengte[i];
    }
    //totale lengte is langer dan breedte terrein
    if (totalelengteContainers + benodigeTussenRuimte > breedteHaven) {
      return INT_MAX;
    }
    //Geldige rij containers, return rijkosten
    else {
      //Laatste rij heeft geen rijkosten
      if (j == aantalContainers - 1) {
        return 0;
      }
      else {
        int eindruimte = breedteHaven - (totalelengteContainers + benodigeTussenRuimte);
        return (rijKostenConstante * square(eindruimte));
      }//else
    }//else
  }//if
  return INT_MAX;
} // end rijKostenBerekenen

//*************************************************************************

int Haven::rijKostenRaw(int rij, vector<pair <int,int>> &plaatsing) {
    if (plaatsing.size() < rij) { //kijk of de rij niet is gebruikt
        return breedteHaven;// retuneer de haven breedte.
    }

    pair<int, int> containersKraanPaar = plaatsing[rij - 1];// kijken of rijkosten van de huidige rij al bepaalt is (gecacht).
    if (cachedKraanContainerParen[rij - 1].first == containersKraanPaar.first && cachedKraanContainerParen[rij - 1].second == containersKraanPaar.second) {
        return cachedRijKosten[rij - 1];// retuneer dan de opgeslagen waarde.
    }
    //recursie
    if (rij == 1) { //begin positie
        int aantalContainers = containersKraanPaar.second; //Bepaal het aantal containers die in rij 1 staan
        int containersLengte = 0;
        for (int i = 0; i < aantalContainers; i++) {
            containersLengte += lengte[i];// berekent het totale lengte van de geplaatste containers in rij 1.
        }

        cachedKraanContainerParen[rij - 1] = containersKraanPaar; // update de cach.
        cachedRijKosten[rij - 1] = breedteHaven - containersLengte;// update de cach.
        return cachedRijKosten[rij - 1]; // retuneer rijkosten en het aantal containers van rij 1.
    }
    // in het geval dat het niet gecacht is
    int aantalContainersTotVorigeRij = plaatsing[rij - 2].second;
    int containersLengte = 0;
    for (int i = aantalContainersTotVorigeRij; i < plaatsing[rij - 1].second; i++) {
        containersLengte += lengte[i]; // tel de lengte bij elkaar op.
    }

    cachedKraanContainerParen[rij - 1] = plaatsing[rij - 1];
    cachedRijKosten[rij - 1] = breedteHaven - containersLengte; //check voor cach
    return cachedRijKosten[rij - 1];// retuneer de nieuwe waarde van rijkosten
}
