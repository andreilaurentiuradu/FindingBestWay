# tema3_sda
RADU Andrei-Laurentiu - 312CC

Cream graful utilizand liste de adiacenta. Asociem numele unei locatii
cu cate un nr de la 1 la n(unde n este numarul de locatii)
Cerinta 1
	a) Cerinta este echivalenta cu aflarea numarului de componente conexe
ale unui graf neorientat ponderat. Vom face o parcurgere in adancime
pentru a afla acest numar(DFS).

	b) Cerinta este echivalenta cu determinarea arborelui partial de cost
minim pentru fiecare componenta conexa. Vom aplica algoritmul lui Kruskal
pentru a determina acest numar, retinand simultan costul fiecarui drumurilor
din fiecare arbore

Cerinta 2
	La aceasta cerinta ni se da un graf orientat.
	Mai intai verificam prin 2 parcurgeri in adancime daca exista drum
corabie->insula si insula->corabie. In caz afirmativ aplicam algoritmul
lui Dijkstra retinand intr-un vector scorurile minime pana la start,
sortand de fiecare data cand se adauga o muchie noua. Retinem intr-un vector
de "tati" drumul gasit. Pe baza vectorului de "tati" facem un vector de "fii"
pentru a putea face afisarea de la corabie la insula.
Afisam:
- drumul cu scorul optim de la corabie la insula
- costul sau (calculat simultan cu drumul)
- adancimea minima de pe parcurs(calculata in momentul afisarii drumului,
actualizand de fiecare data cand gasim o locatie cu adancime mai mica)
- numarul de ture necesare pentru a incarca toata(raportul greutate
adancime minima)

Optimizare: utilizare unei cozi de prioritati in loc de vector

Punctaj teste: 120/120
