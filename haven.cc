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
  haven >> vasteRuimte >> rijKosten >> aantalKranen;

  for (int kol = 0; kol < aantalContainers; kol++) {
    for (int rij = 0; rij < aantalKranen; rij++) {
      haven >> operationeleKosten[kol][rij];
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
  if ( breedteHaven < 0 || vasteRuimte < 0 || rijKosten < 0) {
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

void Haven::genereerInstantie (int b, int N, int ogLengte, int bgLengte,
  int s, double c, int K, double ogOpKosten, double bgOpKosten)
{

// TODO: implementeer deze memberfunctie

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
    cout << "rijkosten: " << rijKosten << endl;
    cout << "aantalKranen: " << aantalKranen << endl;
    cout << "Operationele kosten: ";
    for (int col = 0; col < aantalContainers; col++) {
      for (int row = 0; row < aantalKranen; row++) {
        cout << operationeleKosten[col][row] << ' ';
      }
    }
    cout << endl;
    cout << "De rijkosten zijn: " << rijKostenBerekenen(aantalContainers, lengtes, vasteRuimte,
      rijKosten, breedteHaven) << endl;
  }

}  // drukAfInstantie

//*************************************************************************

double Haven::bepaalMinKostenRec ()
{

// TODO: implementeer deze memberfunctie

  return 0.0;

}  // bepaalMinKostenRec

//*************************************************************************

double Haven::bepaalMinKostenTD ()
{

// TODO: implementeer deze memberfunctie

  return 0.0;

}  // bepaalMinKostenTD

//*************************************************************************

double Haven::bepaalMinKostenBU (vector<pair <int,int> > &plaatsing)
{

// TODO: implementeer deze memberfunctie

  return 0.0;

}  // bepaalMinKostenBU

//*************************************************************************

void Haven::drukAfPlaatsing (vector<pair <int,int> > &plaatsing)
{

// TODO: implementeer deze memberfunctie

}  // drukAfPlaatsing

//*************************************************************************

/* Berekent voor elke i en j met 1 <= i <= j <= N de waarde
  rijkosten(i,j): de rijkosten bestaande uit containers i tot
  en met j
    - als er aan het eind van een rij nog δ ruimte over is,
    dan bedragen de rijkosten c · δ2 voor een constante c*/
  double rijKostenBerekenen(int aantalContainers, int lengtes[], int vasteRuimte,
    int rijKosten, int breedteHaven) {
    double temp = 0; //Houdt de lengte van de containers bij + tussenruimte s
    double eindruimte = 0; //Delta, ruimte aan einde van de rij
    for (int i = 0; i < aantalContainers; i++) {
      temp = lengtes[i] + vasteRuimte;
      if (temp > breedteHaven) {
          break; } //Verbreekt for loop indien containers te lang zijn
      }
      eindruimte = breedteHaven - temp;
      cout << (rijKosten * (eindruimte * eindruimte)) << endl;
      return (rijKosten * (eindruimte * eindruimte));
  }
