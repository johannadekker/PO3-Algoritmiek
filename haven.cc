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
  en binnen de breedte van het terrein past (de containers mogen niet te
  lang zijn) */
  for (int j = 0; j < aantalContainers; j++) {
    if (lengte[j] < 1 || lengte[j] > breedteHaven) {
      return false;
    }
  }
  /* Controleert of alle andere getallen niet negatief zijn (0 mag wel) */
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

// Genereer een instantie met waardes voor b, N, s, c en K uit
// de parameters, met random lengte (van de containers) tussen
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

/* Drukt alle gegevens van  van de instantie het invoer-tekstbestand
overzichtelijk af op het scherm. */

void Haven::drukAfInstantie () {
  if (haveHaven == true) {
    cout <<"Breedte haven: " << breedteHaven << endl;
    cout << "Aantal containers: " << aantalContainers << endl;
    cout << "Lengtes containers: ";
    for (int l = 0; l < aantalContainers; l++){
      cout << lengte[l] << ' ';}
    cout << endl;
    cout << "Vaste ruimte: " << vasteRuimte << endl;
    cout << "c = " << rijKostenConstante << endl;
    cout << "Aantal kranen: " << aantalKranen << endl;
    cout << "Operationele kosten: " << endl;
    for (int row = 0; row < aantalKranen; row++) {
      for (int col = 0; col < aantalContainers; col++) {
        cout << operationeleKosten[row][col] << ' ';
        if (col == aantalContainers-1) {
          cout << endl;
        }
      }
    }
  }
}  // drukAfInstantie

//*************************************************************************

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
  if (j >= aantalContainers || k >= aantalKranen) {
    return 0;
  }

  double kostNaPlaatsen = kraanKosten(j, j, k) + recHalverwegeRij(j, j + 1, k);

  if (k < aantalKranen - 1) { //hoger genummerde kraan beschikbaar
    double kostNaNieuweKraan = recStartRij(j, k + 1);
    //lagere kosten bij inzetten hogere kraan
    return min(kostNaPlaatsen, kostNaNieuweKraan);;
  }

  return kostNaPlaatsen;
} //recStartRij

//*************************************************************************

double Haven::recHalverwegeRij(int i, int j, int k) {
  if (j >= aantalContainers) { //alle containers zijn geplaatst
    return 0;
  }

  double kostNaNieuweRij = recStartRij(j, k) + rijKosten(i, j-1);

  if (rijKosten(i, j) < INT_MAX) { //container past in rij
    double kostNaAansluiten = kraanKosten(j, j, k) + recHalverwegeRij(i, j + 1, k);
    return min(kostNaAansluiten, kostNaNieuweRij);
    }
  //als container niet past begin een nieuwe rij
  return kostNaNieuweRij;
} //recHalverwegeRij

double Haven::recHalverwegeRij(int i, int j, int k) {
  if (j >= aantalContainers) { //alle containers zijn geplaatst
    return 0;
  }

  double kostNaNieuweRij = recStartRij(j, k) + rijKosten(i, j-1);

  if (rijKosten(i, j) < INT_MAX) { //container past in rij
    double kostNaAansluiten = kraanKosten(j, j, k) + recHalverwegeRij(i, j + 1, k);
    return min(kostNaAansluiten, kostNaNieuweRij);
    }
  //als container niet past begin een nieuwe rij
  return kostNaNieuweRij;
} //recHalverwegeRij

//*************************************************************************

double Haven::bepaalMinKostenTD ()
{
  if (!haveHaven) {
    cerr << "Er is geen instantie." << endl;
    return 0;
  }
  cout << "BEFORE: " << endl;
  for (int k = 0; k < aantalKranen; k++) {
    for (int j = 0; j < aantalContainers; j++) {
      cout << totaalKosten[k][k] << ' ';
      if (j == aantalContainers - 1) {
        cout << endl;
      }
    }
  }
  tdStartRij(0,0);
  cout << "AFTER: " << endl;
  for (int k = 0; k < aantalKranen; k++) {
    for (int j = 0; j < aantalContainers; j++) {
      cout << totaalKosten[k][k] << ' ';
      if (j == aantalContainers - 1) {
        cout << endl;
      }
    }
  }

  return totaalKosten[aantalKranen-1][aantalContainers-1];
}  // bepaalMinKostenTD

//*************************************************************************

double Haven::tdStartRij(int j, int k) {
  if (j >= aantalContainers) {
    return 0;
  }
  if (totaalKosten[k][j] > 0) {
    double kostNaPlaatsen = totaalKosten[k][j];
  }
  else {
    double kostNaPlaatsen = kraanKosten(j, j, k) + tdHalverwegeRij(j, j + 1, k);
  }
  if (k < aantalKranen - 1) { //hoger genummerde kraan beschikbaar
    if (totaalKosten[k+1][j] > 0) {
      double kostNaNieuweKraan = totaalKosten[k+1][j];
    }
    else {
      double kostNaNieuweKraan = recStartRij(j, k + 1);
      //lagere kosten bij inzetten hogere kraan
      totaalKosten[k][j] = min(kostNaPlaatsen, kostNaNieuweKraan);
      return totaalKosten[k][j];
    }
  }
  totaalKosten[k][j] = kostenNaPlaatsen;
  return totaalKosten[k][j];
} //tdStartRij

//*************************************************************************

double Haven::tdHalverwegeRij(int i, int j, int k) {
  if (j >= aantalContainers) { //alle containers zijn geplaatst
    return 0;
  }
  if (totaalKosten[k][j] > 0) {
    return totaalKosten[k][j];
  }
  else {
    double kostNaNieuweRij = recStartRij(j, k) + rijKosten(i, j-1);

    if (rijKosten(i, j) < INT_MAX) { //container past in rij
      double kostNaAansluiten = kraanKosten(j, j, k) + recHalverwegeRij(i, j + 1, k);
      totaalKosten[k][j] = min(kostNaAansluiten, kostNaNieuweRij);
      return totaalKosten[k][j];
      }
    //als container niet past begin een nieuwe rij
    totaalKosten[k][j] = kostNaNieuweRij;
    return totaalKosten[k][j];
  }
} //recHalverwegeRij

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
/* voor elke k met 1 ≤ k ≤ K en voor elke i en j met 1 ≤ i ≤ j ≤ N de waarde
kraankosten(k,i,j) te berekenen: de totale operationele kosten voor kraan k
om containers i tot en met j op hun plaats te zetten.

Berekent de totale operationele kosten voor kraan k om containers i (c1) tot en
met j (c2) op hun plaats te zetten. Let op! Geen controles of dit geldig is.

*/

double Haven::kraanKosten(int i, int j, int k) {
  double kraanKosten = 0.0;
  if (i < 0 || j >= aantalContainers || k < 0 || k >= aantalKranen) {
    return INT_MAX;
  }
  else {
    for (int c1 = i; c1 <= j ; c1++) { //c1-1 voor index van array
      kraanKosten += operationeleKosten[k][c1];
    } //end for
      return kraanKosten;
  }
} //end kraanKosten

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


/* Berekent voor elke i en j met 1 <= i <= j <= N de waarde
  rijkosten(i,j): de rijkosten bestaande uit containers i tot
  en met j
    - als er aan het eind van een rij nog δ ruimte over is,
    dan bedragen de rijkosten c · δ2 voor een constante c
    - Gebruikt index posities, niet container nummering!!
 */

int Haven::rijKosten(int i, int j) {
  int extraContainersInRij;
  //Indien invoer geldig is
  if (i + 1 > 0 && j < aantalContainers && i <= j) {
    //Is er meer dan 1 container in de rij?
    extraContainersInRij = j - i;
    int benodigeTussenRuimte = extraContainersInRij * vasteRuimte;
    //Bereken de lengte van alle containers in de rij
    int totalelengteContainers = 0;
    for (int c = i; c <= j; c++) {
      totalelengteContainers += lengte[c];
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
} // end rijKosten

  //*************************************************************************
