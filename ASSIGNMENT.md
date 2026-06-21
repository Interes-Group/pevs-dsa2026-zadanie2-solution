# PEVŠ DSA 2026 - Semestrálny projekt 2

# 🔍 VINDEX (Verbal Indexing Engine) – Textový analytický a vyhľadávací systém

![C++ 17](https://img.shields.io/badge/language-C++17-blue)
![Deadline 29.04.2026 23:59](https://img.shields.io/badge/deadline-29.04.2026%2023%3A59-red)
[![Github Classrom](https://img.shields.io/badge/GitHub-Classroom-green)](https://classroom.github.com/classrooms)
[![Static Badge](https://img.shields.io/badge/Web-DSA.Interes.Group-aquamarine)](https://dsa.interes.group/assignments/assignments2)

Cieľom zadania je implementovať konzolovú aplikáciu (CLI) pre efektívne indexovanie textových dokumentov a následné
vyhľadávanie kľúčových slov pomocou dátovej štruktúry **Prefixový strom (Trie)**.

Program umožní používateľovi spracovať textové súbory, vytvoriť z nich slovník (index) a následne rýchlo vyhľadávať, v
ktorých súboroch sa dané slovo nachádza a s akou frekvenciou. Aby bolo možné s indexom pracovať opakovane, program
ukladá stav spracovaných dokumentov do externého súboru.

Program má pracovať s triedami a kolekciami pre udržiavanie objektov a vlastnou implementáciou stromu.

Program po skompilovaní je spustení z konzoly/terminálu s uvedenými príkazmi napríklad:

```shell
./vindex --help
```

## Systém indexovania

Základom programu je schopnosť prečítať textový súbor, rozložiť ho na slová a každé unikátne slovo uložiť do prefixového
stromu. Pri rozložení vstupného textu do súboru je potrebné spracovať slová tak aby ich bolo možné efektívne indexovať.
Všetky slová sú pred indexovaním prevedené na malé písmená (lowercase) a všetky nealfanumerické znaky sa odstráňujú.
Interpunkcia pri slovách sa ignoruje pri indexovaní (napr. "word." -> "word").

Program o každom slove eviduje:

- Zoznam súborov, v ktorých sa slovo vyskytlo. Zaznamenáva sa názov vstupného súboru.
- Počet výskytov (frekvenciu) slova v každom zo vstupných súborov a taktiež celkovú frekvenciu spolu zo všetkých
  vstupov.

## Príkazy programu

O tom, ktorú operáciu má program vykonať, rozhoduje prvý argument (príkaz):

- `index` - Spracovanie a pridanie nového textového súboru do indexu.
- `search` - Vyhľadanie konkrétneho slova v indexe.
- `stats` - Zobrazenie celkových štatistík o indexovaných dátach.
- `clear` - Vymazanie celého indexu.
- `help` - Zobrazenie nápovedy k príkazom programu.

### Pridanie súboru do indexu - `index`

Program spracuje nový textový súbor a pridá jeho obsah do indexu. Ak súbor už existuje v indexe, program ho ignoruje.
Po zadaní príkazu je potrebné zadať cestu k vstupnému textovému súboru. Súbor musí obsahovať iba čitateľný text,
bez emoji či iných špeciálnych znakov, preto je možné očakávať len znaky preložitelné cez ASCII (anglická abedeca).
Pri indexovaní program ignoruje veľkosť písmen (Case Insensitive) a odstraňuje interpunkciu (bodky, čiarky).

Príklad použitia príkazu:

```shell
./vindex index /cesta/k/vstupnemu/souboru.txt
```

### Vyhľadávanie slov - `search`

Príkaz `search` slúži na dopytovanie sa nad vytvoreným indexom. Príkaz vyhľadá slovo, ktoré je argumentom príkazu.

Program vypíše zoznam všetkých súborov, kde sa slovo nachádza, spolu s počtom výskytov v každom z nich a na záver
celkový počet výskytu. Ak sa slovo v indexe nenachádza, program o tom informuje používateľa.

Príklad použitia príkazu:

```shell
./vindex search word
```

### Štatistiky indexu - `stats`

Príkaz `stats` slúži na vypísanie stavu a štatistík indexu. Program vypíše:

1. Celkový počet indexovaných unikátnych slov (t.j. počet uzlov v Trie označených ako koniec slova).
2. Celkový počet spracovaných súborov a zoznam spracovaných súborov.
3. Top 5 najčastejšie sa vyskytujúcich slov v celom indexe.
4. Pokojne uveďte aj ďalšie štatistiky podľa svojho uváženia, ako napríklad celkový počet slov v indexe, priemerná dĺžka
   slova, slovo, ktoré sa vyskytuje v najviac súboroch atď. - buďte kreatívni.

Príklad použitia príkazu:

```shell
./vindex stats
```

### Vyčistenie indexu - `clear`

Príkaz `clear` slúži na vyčistenie celého indexu. Program vymaže všetky indexované slová a ich výskyty.
Po úspešnom skončení príkazu sa zobrazí správa o úspešnom vyčistení indexu a index bude prázdny.

Príklad použitia príkazu:

```shell
./vindex clear
```

## Implementácia

V rámci implementácie môžete použiť všetky štandardné funkcie a knižnice jazyka C++, v štandarde C++17. Kód musí byť
skompilovateľný základnou inštaláciou programu **G++**, takže pozor na Windows/Mac špecifické kompilátory.

Funkcionalitu programu rozdeľte do niekoľkých funkcií a tried, ktoré následne použijete v programe.

Program implementujte v jednom súbore _main.cpp_, ktorý musí byť umiestnený v priečinku _src_ v tomto repozitári. Ak je
zdrojový súbor umiestnený na inom mieste, alebo bude nazvaný iným menom, nebude braný pri kompilácii do úvahy, a teda
ani pri hodnotení.

Pri spracovaní vstupov sa môžete inšpirovať
článkom [Práca s argumentami programu v jazyku C](https://zapr.interes.group/guides/program_arguments/#spracovanie-prep%c3%adna%c4%8dov-flags)
na stránke predmetu ZAPR, alebo inými technikami uvedených na internete, napr.:

- [https://www.geeksforgeeks.org/cpp/command-line-arguments-in-cpp/](https://www.geeksforgeeks.org/cpp/command-line-arguments-in-cpp/)
- [https://leimao.github.io/blog/Argument-Parser-Getopt-C/](https://leimao.github.io/blog/Argument-Parser-Getopt-C/)

Pri implementácii si môžete vypomáhať s AI nástrojmi, pre inšpiráciu, diskusiu riešenia problémov, či testovanie avšak
**je prísne zakázané priame generovanie kódu vypracovania zadania, či kopírovanie väčších častí kódu z AI či internetu**
. Táto práca je ukážkou vašich schopností a vedomostí programovania nie definovania promptu do AI.

### Implementácia indexu

V rámci tohto zadania **je povinné vlastnoručne implementovať Prefixový strom (Trie)**.

- Nie je dovolené použiť hotovú implementáciu Trie z internetu.
- Každý uzol stromu by mal obsahovať pole alebo inú dynamickú štruktúru pre potomkov (znaky abecedy) a indikátor konca
  slova, a ďalšie informácie o slove podľa zadania vyššie (ako súbory, počet výskytov).
- Pre ukladanie zoznamu súborov v rámci uzla môžete použiť STL kontajnery (napr. `std::vector` alebo `std::map` a
  ďalšie).

### Perzistencia dát (Ukladanie)

Aby sa index nestratil po vypnutí programu, program musí vytvorený index zapísať do súboru
`vindex_index.txt`. Môžete zvoliť vlastný binárny/textový formát pre uloženie celého stromu, ktorý je pri štarte
programu vyskladaný do spravnej funkčnej podoby.

Súbor je vždy načítaný pri spustení programu. Ak súbor neexistuje, program ho vytvorí prázdny.

Pre prácu so súborom sa môžte inšpirovať článkami:

- [https://www.programiz.com/cpp-programming/file-handling](https://www.programiz.com/cpp-programming/file-handling)
- [https://www.geeksforgeeks.org/cpp/file-handling-c-classes/](https://www.geeksforgeeks.org/cpp/file-handling-c-classes/)
- [https://www.w3schools.com/cpp/cpp_files.asp](https://www.w3schools.com/cpp/cpp_files.asp)

### Testovanie vypracovania

V rámci projektu sú dostupné textové súbory v priečinku _testovacie_subory_, ktoré obsahujú úryvky z anglických kníh.
Tieto súbory môžte použiť pre testovanie programu a zaistenia jeho funkčnosti.

Tieto súbory a ďalšie budú použité pri hodnotení zadania.

### Kompilácia

Pre testovanie je možné kód skompilovať príkazom:

```bash
g++ -std=c++17 -o bin/vindex -Wall -Wextra src/main.cpp
```

Kompilátor vytvorí spustený program v priečinku _bin_ v repozitári.

### Git

Zdrojový kód tohto projektu je manažovaný verziovacím systémom Git. Pomocou Gitu je toto vypracovanie zadania aj
odovzdané. Pre oboznámenie práce s Gitom si prečítajte tento
článok [Git pre začiatočníkov](https://dsa.interes.group/other/git-for-beginners/) alebo akýkoľvek iný tutoriál na
internete.

Pre kontrolu kompilácie je v repozitári nastavená automatizácia cez GitHub Actions pipeline. Pipeline sa
spustí automaticky pri aktualizácii kódu vo vetve `main` (napr. `git push`). GitHub zdrojový súbor `src/main.cpp`
skompiluje pomocou poslednej verzie kompilátora **G++** pre štandard jazyka C++17. Pipeline následne pokračuje
jednoduchým testom spustenia programu. Spustenie je testované a zaznamenané pomocou programu
[tui-test](https://github.com/microsoft/tui-test) a na konci pipeline je vypísaný výsledok takého testu.
Pipeline vždy testuje posledný commit vo vetve `main` a testuje program s argumentom `--help`. Pre správnu kontrolu
programu musí nápoveda/pomocný text programu obsah slovo "Help".
Pipeline je možné spustiť ľubovoľný počet krát. Spustenie pipeline je možné vidieť v záložke _Actions_ vo vašom
repozitári zadania.

## Hodnotenie

Zadanie je **ohodnotené 20 bodmi**. Odovzdaný program musí byť skompilovateľný kompilátorom G++ a spustiteľný, inak je
hodnotený 0 bodmi. Pri hodnotení vypracovania zadania sa kontroluje originalita odovzdaného vypracovania medzi všetkými
študentmi a zároveň aj voči AI nástrojom (kód vygenerovaný pomocou ChatGPT, Gemini a Claude Sonnet a pod.). Vypracovanie
**so zhodou vyššou ako 70% sú hodnotené 0 bodmi**. Hodnotený je iba kód ku poslednému commitu vo vetve `main`, ktorý bol
urobený do termínu odovzdania.

Vypracovanie bude **podrobené automatizovaným testom**, ktoré budú spúšťať program s definovanými argumentami a
kontrolovať obsah výstupu programu. Je teda veľmi dôležité dodržať špecifikáciu argumentov, ako aj formát výstupu
programu pre používateľa. Výsledky automatizovaných testov budú zverejnené do tohto repozitára do vetvy `feedback`.
V prípade chyby v testoch, budú testy spustené znovu a nový výsledkom zverejnený. Po uplynutí termínu odovzdania
zadania budú definície testov zverejnené a ak v nim nájdete chybu prosím nahláste
ju [sem ako nové GitHub issue](https://github.com/Interes-Group/pevs-dsa2026-zadanie2).

Vypracovanie bude hodnotené aj ručne. Pri hodnotení vypracovania sa bude prihliadať na:

- práca s premennými
- definovanie control flow programu
- definovanie vlastných funkcií a ich použitie
- definovanie tried a práca s nimi
- spracovanie argumentov
- využitie kontajnerov/kolekcií
- implementácia stromu
- komunikácia chybového stavu programu používateľovi
- štruktúrovane kódu
- dodržanie špecifikácie zadania

## Odovzdanie

Po prihlásení sa na zadanie 2 cez [GitHub Classroom](https://dsa.interes.group/assignments/assignment2)
vám je automaticky vytvorená kópia repozitára, ktorý bude
nastavený ako privátny pre vás na vypracovanie (t.j. máte povolený commit a push do repozitára). Skontrolujte si, či sa
váš repozitár nachádza pod skupinou _Interes-Group_, inak vyučujúci nemajú prístup ku vášmu repozitáru a zadanie sa
nepovažuje za odovzdané. Ak sa vám repozitár nevytvorí ihneď kontaktuje vyučujúceho na MS Teams alebo na univerzitnom
emaily. Je prísne zakázané dávať prístup k vášmu repozitáru inému študentovi, alebo osobe, ktorá nie je vyučujúci. V
repozitáry by sa mali vytvoriť dve vetvy `main` a `feedback` a vytvorí sa tzv. pull request z vetvy `main` do vetvy
`feedback`. **Váš kód pushujte (t.j. `git push`) do vetvy `main`**. Hodnotenie vypracovania, ako aj komentár ku kódu,
uvidíte v danom pull requeste. Ak sa vám vetva `feedback` alebo pull request nevytvorí ihneď kontaktuje vyučujúceho na
MS Teams alebo na univerzitnom emaily.

V repozitári upravujte iba súbory pod priečinkom **src**. Ostatné súbory je zakázané upravovať, predovšetkým súbory pre
kompiláciu, súbory obsahujúce GitHub pipeline (.github) a súbory obsahujúce automatizované testy (priečinok **test**).
Pri zmene týchto súborov môže byť vypracovanie ohodnotené 0 bodmi.

Vypracovanie zadania priebežne commitujte (`git commit`)/pushujte (`git push`) do repozitára vytvoreného GitHubom
pre toto zadanie. Váš kód commitujte/pushujte do vetvy `main`. Hodnotený bude posledný commit/push do termínu odovzdania
vypracovania. Vypracovanie je nutné odovzdať/commitnúť/pushnúť do repozitára **do 29.04.2026 23:59**. Neodovzdanie je
hodnotené 0 bodmi.

V prípade otázok, alebo technických problémov môžete kontaktovať vyučujúcich na MS Teams alebo na univerzitnom emaily.

## Automatizované testovanie

Tento repozitár obsahuje aj nastavenie pre automatizované testovanie pomocou nástroje _tui-test_.
Tento program je založený na jazyku Typescript a je spúšťaný pomocou Node.js. Všetky testy a nastavenia sú uchované
v priečinku _test_ a **prísne zakázané ich prípadnú zmenu pushnúť do repozitára na odovzdanie**. Pre tých, ktorý
ovládajú tento jazyk a sú schopní si rozšíriť test suite o vlastné testy pokojne môžu, len ich nikde nezdieľajte a
nepushujte do repozitára na GitHube na odovzdanie.

### Návod pre nastavenie a spustenie testov

#### Prerekvizity:

- Nainštalovaný **Node.js** (verzia 20 alebo vyššia)
- Nainštalovaný **npm** (Node Package Manager)
- Skompilovaný program `vindex` v priečinku `bin/`

#### Postup:

1. **Skompilujte program** (ak ste to ešte neurobili):

```shell script
mkdir -p bin
g++ -std=c++17 -o bin/vindex -Wall -Wextra src/main.cpp
```

2. **Prejdite do priečinka test**:

```shell script
cd test
```

3. **Nainštalujte závislosti**:

```shell script
npm install
```

4. **Spustite testy**:

```shell script
npm test
```

**Alternatívne spustenie:**

Môžete použiť priamo nástroj tui-test:

```shell script
cd test
npx @microsoft/tui-test
```

#### Čo testy kontrolujú:

Testy používajú nástroj **tui-test** od Microsoftu, ktorý:

- Spúšťa váš program s rôznymi argumentami
- Zachytáva výstup programu v termináli
- Porovnáva výstup s očakávanými výsledkami (uložené v `__snapshots__`)
- Overuje formát výstupu a správnosť výpočtov