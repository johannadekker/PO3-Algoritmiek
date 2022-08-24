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

}  // default constructor


//*************************************************************************

// Deze functie leest gegevens in, en controleert daarbij of:
//  * het bestand te openen is;
//  * aantal kranen & containers binnen de grenzen valt;
//  * lengtes van containers binnen de grenzen vallen;
//  * alle getallen niet negatief zijn.
// Ook wordt haveHaven = true, i.e. er is een instantie.

bool Haven::leesInInstantie (const char* invoerNaam)
{
  ifstream haven;
  haven.open(invoerNaam, ios::in);


  if (haven.fail()) {
    cerr << "Kon het bestand '" << invoerNaam << "' niet openen." << endl;
    return false;
  }
  haven >> b >> N;

  for (int i = 0; i < N; i++) {
    haven >> lengte[i];
  }
  haven >> s >> c >> K;

  for (int row = 0; row < K; row++) {
    for (int col = 0; col < N; col++) {
      haven >> operationeleKosten[row][col];
    }
  }

  if (N > MaxN || K > MaxK) {
    return false;
  }

  for (int j = 0; j < N; j++) {
    if (lengte[j] < 1 || lengte[j] > b) {
      return false;
    }
  }

  if ( b < 0 || s < 0 || c < 0) {
    return false;
  }

  for (int k = 0; k < N; k++) {
    for (int r = 0; r < K; r++) {
      if (operationeleKosten[k][r] < 0) {
        return false;
      }
    }
  }
  haveHaven = true;
  return true;
}  // leesInInstantie
//****************************************************************************

// Geneert een instantie met waardes voor b, N, s, c, en K uit de parameters,
// met random lengtes en random operationele kosten tussen de grenzen uit de
// parameters. 
//  * Geen controle op parameters!

void Haven::genereerInstantie (int b, int N, int ogLengte, int bgLengte,
  int s, double c, int K, double ogOpKosten, double bgOpKosten)
{ 
  this->b = b;
  this->N = N;
  for (int n = 0; n < N; n++) {
    this->lengte[n] = randomGetal(ogLengte, bgLengte);
  }
  this->s = s;
  this->c = c;
  this->K = K;
  for (int k = 0; k < K; k++) {
    for (int j = 0; j < N; j++) {
      operationeleKosten[k][j] = randomDouble(ogOpKosten, bgOpKosten);
    }
  }
  this->haveHaven = true;
}  // genereerInstantie

//*************************************************************************

/* Drukt alle gegevens van  van de instantie het invoer-tekstbestand
overzichtelijk af op het scherm. */

void Haven::drukAfInstantie () {
  if (haveHaven == true) {
    cout << "Breedte haven         (b): " << b << '\n'
         << "Aantal containers     (N): " << N << '\n'
         << "Lengtes containers       : ";
    for (int l = 0; l < N; l++){
      cout << lengte[l] << ' ';}
    cout << '\n'
         << "Minimale tussenruimte (s): " << s << '\n'
         << "Rijkosten Ccnstante   (c): " << c << '\n'
         << "Aantal kranen         (K): " << K << '\n'
         << "Operationele kosten      : " << '\n';
    for (int k = 0; k < K; k++) {
      for (int j = 0; j < N; j++) {
        cout << operationeleKosten[k][j] << ' ';
        if (j == N - 1) {
          cout << '\n';
        }
      }
    }
    cout << endl;
  }
}  // drukAfInstantie

//*************************************************************************

// Roept de helperfunctie recStartRij aan voor de eerste container en kraan.
// PRE:
//  * Controleert of er een instantie is.

double Haven::bepaalMinKostenRec ()
{
  if (!haveHaven) {
    cerr << "Er is geen instantie." << endl;
    return 0;
  }

  return recStartRij(0,0);
} // bepaalMinKostenRec

//*************************************************************************

// Helperfunctie voor het berekenen van de kosten, rechtstreeks recursief,
// in de state 'start rij'. Hierbij worden de minimale kosten gevonden door
// alle beschikbare kranen te vergelijken.
// PRE:
// * Controleert of er nog containers zijn om te plaatsen;
// * Controleert of er nog kranen beschikbaar zijn.
// POST:
// * Retourneert de laagste waarde voor de kosten van het plaatsen van een
//   container aan het begin van een rij.


double Haven::recStartRij(int j, int k) {
  if (j >= N || k >= K) { //alle containers zijn geplaatst
    return 0;
  }

  double kostNaPlaatsen = kraanKosten(j, j, k) + recHalverwegeRij(j, j + 1, k);

  if (k < K - 1) { //indien er een kraan beschikbaar is
    double kostNaNieuweKraan = recStartRij(j, k + 1);
    return min(kostNaPlaatsen, kostNaNieuweKraan);
  }

  //hoogst genummerde kraan is ingezet
  return kostNaPlaatsen;
} //recStartRij

//*************************************************************************

// Helperfunctie voor het berekenen van de minimale kosten, rechtstreeks
// recursief, in de state 'halverwege rij'. Let op: dit betekent dat de
// VORIGE container aan het begin van een rij is geplaatst. Hierbij worden 
// de minimale kosten gevonden door de actie 'aansluiten' te vergelijken met
// 'nieuwe rij beginnen'.
// PRE:
//  * Controleert of er nog containers zijn om te plaatsen
//  * Controleert of de container nog in de rij past
// POST:
//  * Retourneert de laagste waarde van de kosten voor het plaatsen van
//    een container in een rij.

double Haven::recHalverwegeRij(int i, int j, int k) {
  if (j >= N) { //alle containers zijn geplaatst
    return 0;
  }

  double kostNaNieuweRij = recStartRij(j, k) + rijKosten(i, j - 1);

  if (rijKosten(i, j) < INF) { //indien de container in huidige rij past
    double kostNaAansluiten = kraanKosten(j, j, k) + recHalverwegeRij(i, j + 1, k);
    return min(kostNaAansluiten, kostNaNieuweRij);
    }
  //container past niet, begin nieuwe rij
  return kostNaNieuweRij;
} //recHalverwegeRij

//*************************************************************************

// Bepaalt de minimale totale kosten voor het plaatsen van alle containers
// van de instantie met top-down dynamisch programmeren. Indien er geen
// instantie is, retourneert de functie 0. 

double Haven::bepaalMinKostenTD ()
{
  if (!haveHaven) {
    cerr << "Er is geen instantie." << endl;
    return 0;
  }
  //tabel initialiseren
  int i, j, k;
  for(k = 0; k < K; k++) for(j = 0; j < N; j++) for(i = 0; i < N; i++) {
    totaalKosten[k][j][i] = 0;
  }

  return tdStartRij(0,0);
}  // bepaalMinKostenTD

//*************************************************************************

// Helperfunctie voor het berekenen van de minimale kosten in de state 
// 'start rij'. Let op: dit betekent dat de VORIGE container aan het begin van 
// een rij is geplaatst. Als de kosten voor het plaatsen van container j met 
// kraan k al eerder is berekend, retourneert de functie de waarde uit de cache. 
// Zo niet, wordt het berekend.
// PRE:
//  * Controleert of er nog containers zijn om te plaatsen
//  * Controleert of de container nog in de rij past
// POST:
//  * Retourneert de laagste waarde van de kosten voor het plaatsen van
//    een container in een rij.

double Haven::tdStartRij(int j, int k) {
  if (j >= N) {
    return 0;
  }

  if (totaalKosten[k][j][j] > 0) {
    return totaalKosten[k][j][j];
  }

  double kostNaPlaatsen = operationeleKosten[k][j] + tdHalverwegeRij(j, j + 1, k);

  if (k < K - 1) { //hoger genummerde kraan beschikbaar
    double kostNaNieuweKraan = tdStartRij(j, k + 1);
    //vergelijk twee opties
    totaalKosten[k][j][j] = min(kostNaPlaatsen, kostNaNieuweKraan);
  } else {
    totaalKosten[k][j][j] = kostNaPlaatsen;
  }

  return totaalKosten[k][j][j];
} //tdStartRij

//*************************************************************************

// Helperfunctie voor het berekenen van de totaalkosten TD in de state
// 'HalverwegeRij'. Hierbij is i de eerste container van de rij, j de 
// container die we willen plaatsen.

double Haven::tdHalverwegeRij(int i, int j, int k) {
  if (j >= N) { //alle containers zijn geplaatst
    return 0;
  }
  if (totaalKosten[k][j][i] > 0) {
    return totaalKosten[k][j][i];
  }

  double kostNaNieuweRij = tdStartRij(j, k) + rijKosten(i, j-1);

  if (rijKosten(i, j) < INT_MAX) { //container past in rij
    double kostNaAansluiten = kraanKosten(j, j, k) + tdHalverwegeRij(i, j + 1, k);
    totaalKosten[k][j][i] = min(kostNaAansluiten, kostNaNieuweRij);
  } else {
    totaalKosten[k][j][i] = kostNaNieuweRij;
  }

  return totaalKosten[k][j][i];
} //recHalverwegeRij

//*************************************************************************

// Bepaalt de minimale kosten met bottom-up dynamisch programmeren.
// Wanneer een waarde is berekend, wordt de juiste cel van tabelBU ingevuld,
// en de bijbehorende Actie aangeroepen. Dit vult dan de actionTableBU in,
// en maakt de juiste paren aan.

double Haven::bepaalMinKostenBU (vector<pair <int,int> > &plaatsing)
{
  int i,j,k;
  double ok, delta;
  double nieuweKraan, nieuweRij, containerPlaatsen;

  // Initialiseren van de bottom up tabel.
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {
      for(k = 0; k < K; k++) {
        tableBU[i][j][k] = INF;
        actionTableBU[i][j][k] = Actie::unknown;
      }
    }
  }

  // Invullen van de bottom up tabel.
  for(j= N - 1; j >= 0 ; j--) {                                                                 //N maal uitgevoerd
    for(i = j; i >= 0; i--) {                                                                   //N maal uitgevoerd
      delta = ruimteOver(i,j);

      for(k = K - 1; k >= 0; k--) {                                                             //K maal uitgevoerd
        ok = operationeleKosten[k][j];                                                          //constant

        // Berekenen verschillende acties.
        nieuweRij         = i < j               ? rijKosten(i,j-1) + tableBU[j][j][k] : INF;   //constant
        containerPlaatsen = j < N && delta > 0 
                            ? ok + (j == N - 1 ? 0 : tableBU[i][j + 1][k])       
                            : INF;                                                              //constant
        nieuweKraan       = k < K - 1 && i == j ? tableBU[i][j][k + 1]                 : INF;   //constant

        // Bekijk de kleinste actie.
        actionTableBU[i][j][k] = Actie::nieuwe_rij;                                             //constant
        tableBU[i][j][k]  = nieuweRij;

        if(containerPlaatsen < tableBU[i][j][k]) {                                              //constant
          actionTableBU[i][j][k] = Actie::container_plaatsen;                                   //constant
          tableBU[i][j][k]  = containerPlaatsen;                                                //constant
        }

        if(nieuweKraan < actionTableBU[i][j][k]) {                                              //constant
          actionTableBU[i][j][k] = Actie::nieuwe_kraan;                                         //constant
          tableBU[i][j][k]  = nieuweKraan;                                                      //constant
        }

        tableBU[i][j][k] = min({nieuweRij, containerPlaatsen, nieuweKraan});                    //constant
      }
    }
  }
 
  i = 0;
  j = 0;
  k = 0;

  //Keuzes van algoritme bijhouden om zo paren te vormen.
  plaatsing.push_back({k, i});
  while(j < N) {
    switch (actionTableBU[i][j][k])
    {
    case Actie::container_plaatsen:
      j++;
      break;
    case Actie::nieuwe_rij:
      i = j;
      plaatsing.push_back({k, i});
      break;
    case Actie::nieuwe_kraan:
      k++;
      plaatsing.back().first = k;
      break;
    default:
      throw runtime_error("Unknown Action");
      break;
    }
  }

  // Resultaat geven.
  return tableBU[0][0][0];
}  // bepaalMinKostenBU

//*************************************************************************

void Haven::drukAfPlaatsing (vector<pair <int,int> > &plaatsing)
{
  //Overzichtelijk tabel printen
  int i, j, jStart, jEnd;
  cout << "| rijIx | kraan | haven" << string(max(0, b - 5), ' ') << "| containers \n"
       << "+-------+-------+" << string(b+2, '-') << "+" << string(20, '-') << '\n';

  for(i=0; i<plaatsing.size(); i++) {
    cout << "|" << setw(6) << i << ' '
         << "|" << setw(6) << plaatsing[i].first  << ' '
         << "|[";
  
    jStart = plaatsing[i].second;
    jEnd = i<plaatsing.size() - 1 ? plaatsing[i + 1].second : N;
    for(j=jStart; j < jEnd ; j++ ) {
      cout << string(lengte[j], 'X') 
           << string(j == jEnd - 1 ? ruimteOver(jStart, j) : s, ' ');
    }
    cout << "]|";

    for(j=jStart; j<jEnd ; j++) {
      cout << setw(4) << j;
      if(j < jEnd - 1) {
         cout << " +";
      }
    }

    cout << '\n';
    
  }

  cout << endl;

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
  if (i < 0 || j >= N || k < 0 || k >= K) {
    return INF;
  }
  else {
    for (int c1 = i; c1 <= j ; c1++) {
      kraanKosten += operationeleKosten[k][c1];
    } //end for
      return kraanKosten;
  }
} // kraanKosten

//*************************************************************************

// Bepaalt recursief de breedte van een rij door telkens de tussenruimte en
// de lengte van de volgende container erbij op te tellen.
// PRE:
//  * Controleert ook op geldige input

int Haven::rijBreedte(int van, int tot) {
  if(van <  0  ) throw runtime_error("negative starting container index.");
  if(tot <  0  ) throw runtime_error("negative end container index.");
  if(van >= N  ) throw runtime_error("start container index too large.");
  if(tot >= N  ) throw runtime_error("end container index too large.");
  if(van >  tot) throw runtime_error("start container index larger than end container index.");
  if(van == tot) return lengte[van];
  return lengte[van] + s + rijBreedte(van + 1, tot);
} // rijBreedte

//*************************************************************************

// Bepaalt de ruimte die over is aan het einde van de rij

int Haven::ruimteOver(int van, int tot) {
  return b - rijBreedte(van, tot);
} // ruimteOver

//*************************************************************************

// Bepaalt de rijkosten met de gegeven formule, c * delta^2

double Haven::rijKosten(int van, int tot) {
  double delta = ruimteOver(van, tot);
  return delta >= 0 ? c * square(delta) : INF;
} // rijKosten

//*************************************************************************