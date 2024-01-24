# GraphRanker
Progetto finale di algoritmi e strutture dati

## Descrizione
Il progetto consiste nell'implementare un programma in grado di gestire una classifica tra grafi diretti pesati.<br>
La classifica tiene traccia dei k "migliori" grafi.<br><br>
Il programma riceve in ingresso:
  - due parametri, una sola volta (sulla prima riga del file, separati da spazio):
      - d: il numero di nodi dei grafi;
      - k: la lunghezza della classifica;
  - una sequenza di comandi tra:
      - AggiungiGrafo [matrice-di-adiacenza];
      - TopK.

### AggiungiGrafo
Richiede di aggiungere un grafo a quelli considerati per stilare la classifica. È seguito dalla matrice di adiacenza del grafo stesso, stampata una riga per
ogni rigo, con gli elementi separati da virgole.

I nodi del grafo sono da considerarsi etichettati logicamente con un indice intero tra 0 e d-1; il nodo in posizione 0 è quello la cui stella uscente è
descritta dalla prima riga della matrice.

### TopK
Si consideri ogni grafo dall'inizio del programma fino al comando TopK etichettato con un indice intero corrispondente al numero di grafi letti
prima di esso (partendo da 0).<br>
TopK richiede di stampare gli indici interi dei k grafi aventi i k valori più piccoli della seguente metrica:
  - Somma dei cammini più brevi tra il nodo 0 e tutti gli altri nodi del grafo raggiungibili da 0.

Se ci sono più grafi con lo stesso valore della metrica, si dà la precedenza ai primi arrivati.<br>
I k indici interi sono stampati, su un unico rigo, separati da uno spazio.

![es](https://github.com/Alessandro297/graphRanker/assets/64686320/69eee694-247f-400d-9fcf-4829b8d10f28)
