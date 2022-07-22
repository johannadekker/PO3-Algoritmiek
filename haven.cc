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
}  // leesInInstantie
//****************************************************************************

void Haven::genereerInstantie (int b, int N, int ogLengte, int bgLengte,
  int s, double c, int K, double ogOpKosten, double bgOpKosten)
{
}  // genereerInstantie

//*************************************************************************

/* Drukt alle gegevens van  van de instantie het invoer-tekstbestand
overzichtelijk af op het scherm. */

void Haven::drukAfInstantie () {

}  // drukAfInstantie

//*************************************************************************

double Haven::bepaalMinKostenRec ()
{
} // bepaalMinKostenRec

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

  //*************************************************************************
