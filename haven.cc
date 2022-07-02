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
    cout << endl;
    containerPlaatsenRij ();
    cout << endl;
    rijKostenBerekenen(aantalContainers, lengtes, vasteRuimte, rijKostenConstante, breedteHaven);
    cout << endl;
    kraanKostenBerekenen(1, 5, 1);
  }
}  // drukAfInstantie

//*************************************************************************

double Haven::bepaalMinKostenRec (int aantalContainers) {
  int r = 0; //bijhouden hoe veel rijen je gebruikt
  int k = aantalKranen; //bijhouden welke kraan als laatste wordt ingezet
  for (int c = aantalContainers; c >=0; c--) {
    double nieuweKraan = kraanKostenBerekenen(c-1, c, k);
    double zelfdeKraan = kraanKostenBerekenen(aantalContainers-1, aantalContainers, k-1);
    if (c == 0) {
      return 0;
    }//end if1
    if (c == 1) {
      return bepaalMinKostenRec(c-1) + kraanKostenBerekenen(1,1,1);
    }//end if2
    else {
      for (int i = 0; i < aantalContainers; i++) { //loop door alle mogelijkheden van een kolom door
        while (c >= 2) {//containerCombinations[0][-1] is niet gedefinieerd
          //Als de container kan aansluiten in een vorige rij
          if (containerCombinaties[i][c - 1] == 1) {
            //Dan begin je alleen een nieuwe rij als de rijkosten voor die container
            //alleen plaatsen kleiner zijn dan de kosten van dezelfde kraan gebruiken
            if (zelfdeKraan > rijKosten[i][c-2] + nieuweKraan) {
              return bepaalMinKostenRec(c-1) + nieuweKraan + rijKosten[i][c-2];
              r++;
              //Voeg alleen de rijkosten toe van de rij ervoor wanneer je een nieuwe rij begint
            } //end if4
            else {
              return bepaalMinKostenRec(c-1) + zelfdeKraan;
            } //end else2
          }//end if
          //Als de container op een nieuwe rij moet
          else {
            r++;
            if (nieuweKraan < zelfdeKraan) { //goedkoper om een nieuwe kraan te gebruiken
              return bepaalMinKostenRec(aantalContainers-1) + nieuweKraan; //laatste rij heeft geen rijkosten
            } //end else if
            if (zelfdeKraan <= nieuweKraan) { //geen verschil
              return bepaalMinKostenRec(aantalContainers-1) + zelfdeKraan;
            } //end else if
          } //end while
        }//end for2
      }//end else1
    }//end for
  //De container MOET op een nieuwe rij
  } //end else1
}

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
  /* Test cout */
  cout << "Container Plaatsingen: " << endl;
  for (int i = 0; i < aantalContainers; i++) {
    for (int j = 0; j < aantalContainers; j++) {
      cout << containerCombinaties[i][j] << ' ';
      if (j == aantalContainers - 1) {
        cout << endl;
      }
    }
  }
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
  for (int i = c1-1; i <= c2; i++) { //c1-1 voor index van array
    kraanKosten = kraanKosten + operationeleKosten[kraan-1][i];
  } //end for
    return kraanKosten;
} //end kraanKostenBerekenen

//*************************************************************************

/* Berekent voor elke i en j met 1 <= i <= j <= N de waarde
  rijkosten(i,j): de rijkosten bestaande uit containers i tot
  en met j
    - als er aan het eind van een rij nog δ ruimte over is,
    dan bedragen de rijkosten c · δ2 voor een constante c */

  void Haven::rijKostenBerekenen(int aantalContainers, int lengtes[], int vasteRuimte,
    int rijKostenConstante, int breedteHaven) {
    double eindruimte = 0; //Delta, ruimte aan einde van de rij
    for (int row = 0; row < aantalContainers; row++) { //loop over rijen haven
    double temp = 0;
      for (int col = row; col < aantalContainers; col++) {//loop over de containers
        if (containerCombinaties[row][col] == 1) {//als het een geldige plaatsing is
          temp = temp + lengtes[col] + vasteRuimte; //tel lengte van de containers in de rij op
          eindruimte = breedteHaven - temp + vasteRuimte; //trek dit van de breedteHaven af, en houd rekening met de extra getelde vaste ruimte
          rijKosten[row][col] = rijKostenConstante * (eindruimte * eindruimte); //bereken rijKosten, sla op in array
        }
        else if (containerCombinaties[row][col] == 0) {//indien ongeldige plaatsing
          rijKosten[row][col] = 0; //geen rijKosten
          break; //en bekijk de volgende rij met containers
        }
        /* cout << "row @: " << row << endl;
        cout << "col @: " << col << endl;
        cout << "temp @: " << temp << endl;
        cout << "lengtes[col] @: " << lengtes[col] << endl;
        cout << "eindruimte @: " << eindruimte << endl;
        cout << endl; */
      }//end for2
    }//end for1
    cout << "Rij Kosten: " << endl;
    for (int i = 0; i < aantalContainers; i++) {
      for (int j = 0; j < aantalContainers; j++) {
        cout << rijKosten[i][j] << ' ';
        if (j == (aantalContainers - 1)) {
          cout << endl;
        }
      }
    }
  }//end rijKostenBerekenen

  //*************************************************************************
