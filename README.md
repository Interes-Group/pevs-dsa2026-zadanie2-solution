# PEVŠ DSA 2026 - Semestrálny projekt 2 - Riešenie

# 🔍 VINDEX – Verbal Indexing Engine

Konzolová aplikácia (CLI) pre indexovanie textových dokumentov a vyhľadávanie slov pomocou dátovej štruktúry **Prefixový
strom (Trie)**.

## Popis

VINDEX číta textové súbory, spracuje ich obsah a uloží každé unikátne slovo do prefixového stromu. Pre každé slovo
eviduje, v ktorých súboroch sa nachádza a s akou frekvenciou. Index je ukladaný do súboru `vindex_index.txt` a pri
každom spustení programu automaticky načítaný.

## Kompilácia

```bash
mkdir -p bin
g++ -std=c++17 -o bin/vindex -Wall -Wextra src/main.cpp
```

## Použitie

```shell
./bin/vindex <príkaz> [argumenty]
```

## Príkazy

| Príkaz           | Popis                                        |
|------------------|----------------------------------------------|
| `index <súbor>`  | Zaindexuje textový súbor do indexu           |
| `search <slovo>` | Vyhľadá slovo v indexe a vypíše jeho výskyty |
| `stats`          | Zobrazí štatistiky o indexe                  |
| `clear`          | Vymaže celý index                            |
| `help`           | Zobrazí nápovedu                             |

### Príklady

```shell
# Zaindexovanie súboru
./bin/vindex index testovacie_subory/kniha1.txt

# Vyhľadanie slova
./bin/vindex search hello

# Zobrazenie štatistík
./bin/vindex stats

# Vymazanie indexu
./bin/vindex clear
```

## Štruktúra projektu

```
├── src/
│   └── main.cpp              # Zdrojový kód programu
├── bin/
│   └── vindex                # Skompilovaný program
├── test/
│   └── ...                   # Automatizované testy (tui-test)
├── testovacie_subory/        # Vzorové textové súbory na testovanie
├── CMakeLists.txt
└── ASSIGNMENT.md             # Zadanie projektu
```

## Technické detaily

- **Jazyk:** C++17
- **Kompilátor:** G++
- **Dátová štruktúra indexu:** vlastná implementácia Prefixového stromu (Trie)
- **Perzistencia:** index sa ukladá do súboru `vindex_index.txt` vo formáte `slovo súbor:počet,...`
- **Predspracovanie slov:** všetky slová sú prevedené na malé písmená a nealfanumerické znaky sú odstránené
