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
    haven >> lengtes[i];
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
    if (lengtes[j] < 1 || lengtes[j] > breedteHaven) {
      return false;
    }
  }
  /* Controleert of alle andere getallen niet negatief zijn (0 mag wel) */
  if ( breedteHaven < 0 || vasteRuimte < 0 || rijKostenConstante < 0) {
    return false;
  }
  for (int k = 0; k < aantalContainers; k++) {
    for (int r = 0; r < aantalKranen; r++)
    if (operationeleKosten[k][r] < 0) {
      return false;
    }
  }
  haveHaven = true;
  return true;
}  // leesInInstantie
//****************************************************************************

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

void Haven::genereerInstantie (int b, int N, int ogLengte, int bgLengte,
  int s, double c, int K, double ogOpKosten, double bgOpKosten)
{
  b = randomGetal(1, INT_MAX);
  N = randomGetal(1, 1024);
  ogLengte = 1;
  bgLengte = b;
  for (int n = 0; n < N; n++) {
    lengtes[n] = randomGetal(ogLengte, bgLengte);
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
    cout <<"breedte haven: " << breedteHaven << endl;
    cout << "aantal containers: " << aantalContainers << endl;
    cout << "lengtes: ";
    for (int l = 0; l < aantalContainers; l++)
    cout << lengtes[l] << ' ';
    cout << endl;
    cout << "vaste ruimte: " << vasteRuimte << endl;
    cout << "rijkosten: " << rijKostenConstante << endl;
    cout << "aantalKranen: " << aantalKranen << endl;
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
{
  int j = aantalContainers;
  int laatsteKeuze = 1;
  if (!haveHaven) {
    cerr << "Er is geen instantie." << endl;
    return 0;
  }
  if (aantalContainers == 0) {
    return 0;
  }
  if (aantalContainers == 1 && aantalKranen == 1) {
    j = 1;
    laatsteKeuze = 1;
    minKosten = kraanKostenBerekenen(1, 1, 1);
  }
  //Vult een rij met containers op de goedkoopst mogelijke manier
  if (aantalContainers > 1 && aantalKranen > 1) {
    double temp = INT_MAX; int rK; double oK;
    int eersteContainer;
    for (int i = 1; i <= j; i++) {
      for (int k = laatsteKeuze; k <= aantalKranen; k++) {
        if (rijKostenBerekenen(i,j) + kraanKostenBerekenen(i, j, k) < temp) {
          temp = rijKostenBerekenen(i, j) + kraanKostenBerekenen(i, j, k);
          rK = rijKostenBerekenen(i, j);
          eersteContainer = i;
          laatsteKeuze = k;
        }
      }
    }
    minKosten += temp; rijKosten.push_back(rK);
    aantalKranen = laatsteKeuze; aantalContainers = eersteContainer-1;
    bepaalMinKostenRec();
  }
  else if (aantalContainers > 1 && aantalKranen == 1) {
    double temp = INT_MAX; int rK; double oK;
    for (int i = j-1; i > 0; i--) {
      if (rijKostenBerekenen(i,j) + kraanKostenBerekenen(i, j, 1) < temp) {
        temp = rijKostenBerekenen(i,j) + kraanKostenBerekenen(i, j, 1);
        rK = rijKostenBerekenen(i,j);
        j = i-1;
      }
    }
    minKosten += temp; rijKosten.push_back(rK); teller++;
    bepaalMinKostenRec();
  }
  else if (aantalContainers == 1 && aantalKranen > 1) {
    double temp = INT_MAX;
    for (int k = laatsteKeuze; k > 0; k--) {
      if (kraanKostenBerekenen(1,1,k) < temp) {
        temp = kraanKostenBerekenen(1,1,k);
      }
    }
    minKosten += temp;
  }
  return minKosten - rijKosten[0];
}  // bepaalMinKostenRec

//*************************************************************************

double Haven::bepaalMinKostenTD ()
{
  if (!haveHaven) {
    cerr << "Er is geen instantie." << endl;
    return 0;
  }
  if (aantalContainers == 1 ) {
    double min = 10.1; //maximale operationeleKosten zijn 10.0
    //kijk naar alle kranen welke het goedkoopst is
    for (int k = 0; k < aantalKranen; k++) {
      if (operationeleKosten[k][0] < min) {
        minKosten = operationeleKosten[k][0];
        lengteContainersRij = lengtes[0];
        laatsteKeuze = k;
        rijkosten = rijKostenConstante * ((breedteHaven - lengteContainersRij)*(breedteHaven - lengteContainersRij));
        break;
      }
    }
    aantalContainers++;
    bepaalMinKostenTD();
  }
  //Als container 1 is bepaald
  if (minKosten > 0 && lengtes[aantalContainers-1] != 0) {
    int j = aantalContainers-1;
    //eerste check: kan volgende container aansluiten?
    if (lengtes[j]+lengteContainersRij+vasteRuimte <= breedteHaven) {
      double min = operationeleKosten[laatsteKeuze][j];
      rijkosten = rijKostenConstante * ((breedteHaven - lengteContainersRij)*(breedteHaven - lengteContainersRij));
      //Als de laatste kraan is ingezet, voeg dan gewoon o_k,j toe.
      if (laatsteKeuze + 1 == aantalKranen) {
        minKosten += min;
        }
      //Nu wordt er vanaf de laatst gekozen kraan gekeken, ipv alle kranen
      //kijk of een hogere kraan inzetten en de gemaakte rijkosten goedkoper is
      //dan aansluiten
      for (int k = laatsteKeuze + 1; k < aantalKranen; k++) {
        if (operationeleKosten[k][j] + rijkosten < min) {
          min = operationeleKosten[k][j] + rijkosten;
          lengteContainersRij = 0 + lengtes[j];
          laatsteKeuze = k;
        }
        //Als een hogere kraan niet goedkoper is
        else {
          lengteContainersRij += lengtes[j] + vasteRuimte;
        } // end else
        minKosten += min;
      } //end for
    }//end if
    else { //kan niet aansluiten
      rijkosten = rijKostenConstante * ((breedteHaven - lengteContainersRij)*(breedteHaven - lengteContainersRij));
      minKosten += rijkosten; //Voeg rijkosten vorige rij toe bij het beginnen van een nieuwe rij
      lengteContainersRij = lengtes[j]; //Opnieuw initializeren
      double min = operationeleKosten[laatsteKeuze][j]; //min is laatste ingezette kraan gebruiken
      //Indien de hoogst genummerde kraan is ingezet, voeg gewoon o_k,j toe.
      if (laatsteKeuze + 1 == aantalKranen) {
        minKosten += min;
      }
      //Kijk anders of een hogere kraan lagere kosten oplevert
      for (int k = laatsteKeuze + 1; k < aantalKranen; k++) {
        if (operationeleKosten[k][j] < min) {
          min = operationeleKosten[k][j];
          minKosten += min; //tel de rijKosten op;
          laatsteKeuze = k;
        }
        else {
          minKosten += min;
          break;
        }
      }//end for
    } //end else
    //Container is geplaatst, kijk nu naar de volgende
    aantalContainers++;
    bepaalMinKostenTD();
    //Laatste rij heeft geen rijkosten, dus trek de laatst berekende rijkosten af
    return (minKosten - rijkosten);
    //end loop containers;
  }
  //Als de eerste container niet is bepaald, reduceer het aantal containers
  if (aantalContainers > 1) {
    while (minKosten == 0) {//niks is bepaald
      aantalContainers--;
      bepaalMinKostenTD();
    }
  }
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
        int rijKostenExtraContainer = rijKostenHuidigeRij - lengtes[plaatsing[plaatsing.size() - 1].second];// rijkosten als we een container erbij toevoegen
        int winstRuimteRij1 = (rijKostenHuidigeRij * rijKostenHuidigeRij - rijKostenExtraContainer * rijKostenExtraContainer) * rijKostenConstante;// de lege ruimte in een rij

        vector<pair<int, int>> containerToekomst1(plaatsing);
        int rijKostenNieuweRijExtraContainer = 0;
        containerToekomst1.push_back(make_pair(0, containerToekomst1[containerToekomst1.size() - 1].second + 1));// we voegen (nep) de container aan de rij
        //en we kijken naar 2 senarios, wanneer het WEL mogelijk is om de container te verplaatsen en wanneer NIET.
        while (kanContainerInRijPlaatsen(containerToekomst1)) {
            rijKostenNieuweRijExtraContainer += lengtes[containerToekomst1[containerToekomst1.size() - 1].second];
            containerToekomst1[containerToekomst1.size() - 1].second += 1; // voeg container aan de tweede rij.
        }

        vector<pair<int, int>> containerToekomst2(plaatsing);
        int rijKostenNieuweRij = 0;
        containerToekomst2.push_back(make_pair(0, containerToekomst2[containerToekomst2.size() - 1].second + 2));
        while (kanContainerInRijPlaatsen(containerToekomst2)) {
            rijKostenNieuweRij += lengtes[containerToekomst2[containerToekomst2.size() - 1].second];
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
    return kosten - huidigeRijContainers * vasteRuimte - lengtes[plaatsing[plaatsing.size() - 1].second + 1] >= 0; // als de ruimte de over is groter is dan 0, dan betekent het dat de container geplaatst mag worden.
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

void Haven::containerPlaatsenRij() {
  int temp = 0;
  for (int row = 0; row < aantalContainers; row++) { //Loop door containers
    temp = lengtes[row]; //temp houdt de gevulde ruimte bij; nu de lengte van de eerste container in de rij
    containerCombinaties[row][row] = 1;// Iedere container kan sowieso op een aparte rij mogelijk worden geplaatst: worst case
    for (int col = row + 1; col < aantalContainers; col++) { // Alleen de containers die na de eerste container op de rij komen kunnen in dezelfde rij worden geplaatst
      if (temp + vasteRuimte + lengtes[col] <= breedteHaven) { // Is het een geldige plaatsing om de volgende container in de rij te plaatsen?
        temp = temp + vasteRuimte + lengtes[col]; //Dan wordt de gevulde ruimte ide som van de lengte van containers j en j+1 plus de vaste tussenruimte
        containerCombinaties[row][col] = 1; // Onthoud dat dit een geldige mogelijke plaatsing van containers samen op een rij is
        if (containerCombinaties[row][col-1] == 0) { //Indien een eerder bekeken container niet kon worden geplaatst
          containerCombinaties[row][col] = 0; //Dan kunnen de containers die volgen nooit in deze rij worden geplaatst
        }//end if2
      } //end if1
      else { //Indien de lengtes samen groter zijn dan de breedte van de haven
        containerCombinaties[row][col] = 0; //Markeer het als ongeldige plaatsing
      } //end else
    }//for2
  }//for1
}//end functie

//*************************************************************************
/* voor elke k met 1 ≤ k ≤ K en voor elke i en j met 1 ≤ i ≤ j ≤ N de waarde
kraankosten(k,i,j) te berekenen: de totale operationele kosten voor kraan k
om containers i tot en met j op hun plaats te zetten.

Berekent de totale operationele kosten voor kraan k om containers i (c1) tot en
met j (c2) op hun plaats te zetten. Let op! Geen controles of dit geldig is.

*/

double Haven::kraanKostenBerekenen(int c1, int c2, int kraan) {
  double kraanKosten = 0.0;
  if (c1 < 1 || c2 > aantalContainers || kraan < 1 || kraan > aantalKranen) {
    return 10.1;
  }
  else {
    for (int i = c1-1; i < c2; i++) { //c1-1 voor index van array
      kraanKosten = kraanKosten + operationeleKosten[kraan-1][i];
    } //end for
      return kraanKosten;
  }
} //end kraanKostenBerekenen

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
            containersLengte += lengtes[i];// berekent het totale lengte van de geplaatste containers in rij 1.
        }

        cachedKraanContainerParen[rij - 1] = containersKraanPaar; // update de cach.
        cachedRijKosten[rij - 1] = breedteHaven - containersLengte;// update de cach.
        return cachedRijKosten[rij - 1]; // retuneer rijkosten en het aantal containers van rij 1.
    }
    // in het geval dat het niet gecacht is
    int aantalContainersTotVorigeRij = plaatsing[rij - 2].second;
    int containersLengte = 0;
    for (int i = aantalContainersTotVorigeRij; i < plaatsing[rij - 1].second; i++) {
        containersLengte += lengtes[i]; // tel de lengtes bij elkaar op.
    }

    cachedKraanContainerParen[rij - 1] = plaatsing[rij - 1];
    cachedRijKosten[rij - 1] = breedteHaven - containersLengte; //check voor cach
    return cachedRijKosten[rij - 1];// retuneer de nieuwe waarde van rijkosten
}


/* Berekent voor elke i en j met 1 <= i <= j <= N de waarde
  rijkosten(i,j): de rijkosten bestaande uit containers i tot
  en met j
    - als er aan het eind van een rij nog δ ruimte over is,
    dan bedragen de rijkosten c · δ2 voor een constante c */

int Haven::rijKostenBerekenen(int beginContainer, int eindContainer) {
  int containersInRij;
  if (beginContainer == eindContainer) {
    containersInRij = 1;
  }
  else {
    containersInRij = eindContainer - beginContainer + 1;
  }
  int benodigeTussenRuimte = (containersInRij - 1) * vasteRuimte;
  int totaleLengtesContainers = 0;
  for (int i = beginContainer; i <= eindContainer; i++) {
    totaleLengtesContainers += lengtes[i-1];
  }
  //Als de containers te lang zijn voor de rij
  if (totaleLengtesContainers + benodigeTussenRuimte > breedteHaven) {
    int maxRK = rijKostenConstante * intSquare(breedteHaven);
    return maxRK;
  }
  else {
  //Als het een geldige rij voorstelt, return rijkosten
    int eindruimte = breedteHaven - (totaleLengtesContainers + benodigeTussenRuimte);
    return (rijKostenConstante * square(eindruimte));
  }
}// end rijKostenBerekenen

  //*************************************************************************
