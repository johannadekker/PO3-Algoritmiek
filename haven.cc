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
  /* Controleert of het aantal containers en het aantal kranen binnen de
  grenzen van de opdracht vallen */
  if (N > MaxN || K > MaxK) {
    return false;
  }
  /* Controleert of elke afzonderlijke container minstens lengte 1 heeft
  en binnen de breedte van het terrein past (de containers mogen niet te
  lang zijn) */
  for (int j = 0; j < N; j++) {
    if (lengte[j] < 1 || lengte[j] > b) {
      return false;
    }
  }
  /* Controleert of alle andere getallen niet negatief zijn (0 mag wel) */
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
  this->b = b;
  this->N = N;
  for (int n = 0; n < N; n++) {
    this->lengte[n] = randomGetal(ogLengte, bgLengte);
  }
  this->s = s;
  this->c = c;
  this->K = K;
  for (int k = 0; k < K; k++) {
    for (int i = 0; i < N; i++) {
      operationeleKosten[k][i] = randomDouble(ogOpKosten, bgOpKosten);
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
         << "Minimale Tussenruimte (s): " << s << '\n'
         << "Rijkosten Constante   (c): " << c << '\n'
         << "Aantal kranen         (K): " << K << '\n'
         << "Operationele kosten      : " << '\n';
    for (int row = 0; row < K; row++) {
      for (int col = 0; col < N; col++) {
        cout << operationeleKosten[row][col] << ' ';
        if (col == N - 1) {
          cout << '\n';
        }
      }
    }
    cout << endl;
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
  if (j >= N || k >= K) {
    return 0;
  }

  double kostNaPlaatsen = kraanKosten(j, j, k) + recHalverwegeRij(j, j + 1, k);

  if (k < K - 1) { //hoger genummerde kraan beschikbaar
    double kostNaNieuweKraan = recStartRij(j, k + 1);
    //lagere kosten bij inzetten hogere kraan
    return min(kostNaPlaatsen, kostNaNieuweKraan);;
  }

  return kostNaPlaatsen;
} //recStartRij

//*************************************************************************

double Haven::recHalverwegeRij(int i, int j, int k) {
  if (j >= N) { //alle containers zijn geplaatst
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

  int i, j, k;
  for(k=0;k<K;k++) for(j=0;j<N;j++) for(i=0;i<N;i++) {
    totaalKosten[k][j][i] = 0;
  }
  /*
  cout << "BEFORE: " << endl;
  for (int k = 0; k < K; k++) {
    for (int j = 0; j < N; j++) {
      for (int i = 0; i < N; i++) {
        cout << setw(8) << totaalKosten[k][j][i] << ' ';
        if (i == N - 1) {
          cout << '\n';
        }
      }
    }
    cout << endl;
  }
  */
  return tdStartRij(0,0);
  /*
  cout << "AFTER: " << endl;
  for (int k = 0; k < K; k++) {
    for (int j = 0; j < N; j++) {
      for (int i = 0; i < N; i++) {
        cout << setw(8) << totaalKosten[k][j][i] << ' ';
        if (i == N - 1) {
          cout << '\n';
        }
      }
    }
    cout << endl;
  }
  */
  // return totaalKosten[0][0][0];
}  // bepaalMinKostenTD

//*************************************************************************

double Haven::tdStartRij(int j, int k) {
  if (j >= N) {
    return 0;
  }

  if (totaalKosten[k][j][j] > 0) {
    // cout << "tdStartRij(" << k << ',' << j << ") from cache!" << endl;
    return totaalKosten[k][j][j];
  }

  double kostNaPlaatsen = operationeleKosten[k][j] + tdHalverwegeRij(j, j + 1, k);

  if (k < K - 1) { //hoger genummerde kraan beschikbaar
    double kostNaNieuweKraan = tdStartRij(j, k + 1);
    //lagere kosten bij inzetten hogere kraan
    totaalKosten[k][j][j] = min(kostNaPlaatsen, kostNaNieuweKraan);
  } else {
    totaalKosten[k][j][j] = kostNaPlaatsen;
  }


  // cout << "tdStartRij(" << k << ',' << j << ") computed!" << endl;
  return totaalKosten[k][j][j];
} //tdStartRij

//*************************************************************************

double Haven::tdHalverwegeRij(int i, int j, int k) {
  if (j >= N) { //alle containers zijn geplaatst
    return 0;
  }
  if (totaalKosten[k][j][i] > 0) {
    // cout << "tdHalverwegeRij(" << k << ',' << j << ',' << i << ") from cache!" << endl;
    return totaalKosten[k][j][i];
  }

  double kostNaNieuweRij = tdStartRij(j, k) + rijKosten(i, j-1);

  if (rijKosten(i, j) < INT_MAX) { //container past in rij
    double kostNaAansluiten = kraanKosten(j, j, k) + tdHalverwegeRij(i, j + 1, k);
    totaalKosten[k][j][i] = min(kostNaAansluiten, kostNaNieuweRij);
  } else {
    totaalKosten[k][j][i] = kostNaNieuweRij;
  }
  //als container niet past begin een nieuwe rij
  // cout << "tdHalverwegeRij(" << k << ',' << j << ',' << i << ") computed!" << endl;
  return totaalKosten[k][j][i];
} //recHalverwegeRij

//*************************************************************************

ostream &operator<<(ostream &out, Actie const& actie) {
  switch (actie) 
  {
    case Actie::nieuwe_rij: return out << "NR";
    case Actie::container_plaatsen: return out << "CP";
    case Actie::nieuwe_kraan: return out << "NK";
    case Actie::klaar: return out << "TT";
    case Actie::unknown:
    default: return out << "??";
  }
}

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
  for(j= N - 1; j >= 0 ; j--) {
    for(i = j; i >= 0; i--) {
      delta = ruimteOver(i,j);

      for(k = K - 1; k >= 0; k--) {
        ok = operationeleKosten[k][j];

        // Berekenen verschillende acties.
        nieuweRij         = i < j               ? rijKosten2(i,j-1) + tableBU[j][j][k] : INF;
        containerPlaatsen = j < N && delta > 0 
                            ? ok + (j == N - 1 ? 0 : tableBU[i][j + 1][k])       
                            : INF;
        nieuweKraan       = k < K - 1 && i == j ? tableBU[i][j][k + 1]                 : INF;

        // Bekijk de kleinste actie.
        actionTableBU[i][j][k] = Actie::nieuwe_rij;
        tableBU[i][j][k]  = nieuweRij;

        if(containerPlaatsen < tableBU[i][j][k]) {
          actionTableBU[i][j][k] = Actie::container_plaatsen;
          tableBU[i][j][k]  = containerPlaatsen;
        }

        if(nieuweKraan < actionTableBU[i][j][k]) {
          actionTableBU[i][j][k] = Actie::nieuwe_kraan;
          tableBU[i][j][k]  = nieuweKraan;
        }

        tableBU[i][j][k] = min({nieuweRij, containerPlaatsen, nieuweKraan});
      }
    }
  }
  
  // Afdrukken van tabel.
  /*
  for(i=0; i<N; i++) {
    cout << "i=" << setw(4) << i << ":\n";
    for(int j=0; j<N+1; j++) {
      cout << "j=" << setw(4) << j;
      if (j < N && i <= j) {
        cout << ", delta=" << setw(4) << ruimteOver(i,j);
      } else {
        cout << ", undefined ";
      }
      cout << " :: ";
      for(int k=0; k<K; k++) {
        cout << setw(6) << table[i][j][k];
        if(j<N) {
          cout << aTable[i][j][k];
        } else {
          cout << Actie::klaar;
        }
      }
      cout << '\n';
    }
    cout << '\n';
  }
  */
 
  i = 0;
  j = 0;
  k = 0;

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
    for (int c1 = i; c1 <= j ; c1++) { //c1-1 voor index van array
      kraanKosten += operationeleKosten[k][c1];
    } //end for
      return kraanKosten;
  }
} //end kraanKosten

//*************************************************************************

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
  if (i + 1 > 0 && j < N && i <= j) {
    //Is er meer dan 1 container in de rij?
    extraContainersInRij = j - i;
    int benodigeTussenRuimte = extraContainersInRij * s;
    //Bereken de lengte van alle containers in de rij
    int totalelengteContainers = 0;
    for (int c = i; c <= j; c++) {
      totalelengteContainers += lengte[c];
    }
    //totale lengte is langer dan breedte terrein
    if (totalelengteContainers + benodigeTussenRuimte > b) {
      return INT_MAX;
    }
    //Geldige rij containers, return rijkosten
    else {
      //Laatste rij heeft geen rijkosten
      if (j == N - 1) {
        return 0;
      }
      else {
        int eindruimte = b - (totalelengteContainers + benodigeTussenRuimte);
        return (c * square(eindruimte));
      }//else
    }//else
  }//if
  return INT_MAX;
} // end rijKosten

//*************************************************************************

int Haven::rijBreedte(int van, int tot) {
  if(van <  0  ) throw runtime_error("negative starting container index.");
  if(tot <  0  ) throw runtime_error("negative end container index.");
  if(van >= N  ) throw runtime_error("start container index too large.");
  if(tot >= N  ) throw runtime_error("end container index too large.");
  if(van >  tot) throw runtime_error("start container index larger than end container index.");
  if(van == tot) return lengte[van];
  return lengte[van] + s + rijBreedte(van + 1, tot);
}

int Haven::ruimteOver(int van, int tot) {
  return b - rijBreedte(van, tot);
}

double Haven::rijKosten2(int van, int tot) {
  double delta = ruimteOver(van, tot);
  return delta >= 0 ? c * square(delta) : INF;
}

  //*************************************************************************