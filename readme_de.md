# Typing Simulator

## Beschreibung

**Typing Simulator** ist ein einfaches Konsolenprogramm, das den Vim-Editor emuliert. Dieses Programm ist darauf ausgelegt, den Prozess des Tippens zu simulieren, einschließlich der Möglichkeit, zufällige Fehler einzufügen, diese zu korrigieren und Zeilen automatisch umzubrechen, wenn das Ende des Bildschirms erreicht wird. Das Programm ist ideal für die Aufnahme von Programmier-Screencasts geeignet, da es realistische Demonstrationen der Arbeit in einem Texteditor ermöglicht. **Typing Simulator** ist in C geschrieben und verwendet die Bibliothek ncurses zur Steuerung der Textoberfläche.

### Hauptfunktionen:
- Simulation des Tippvorgangs mit einstellbarer Geschwindigkeit.
- Möglichkeit, zufällige Tippfehler für mehr Realismus hinzuzufügen.
- Automatischer Zeilenumbruch beim Erreichen des Bildschirmendes.
- Syntax-Highlighting für Schlüsselwörter und Zeichenfolgen in den Sprachen C/C++.
- Anzeige einer Vim-ähnlichen Statuszeile mit der aktuellen Cursorposition.
- Unterstützung von Breitzeichen und Verwendung der ncurses-Bibliothek für die Terminalsteuerung.

## Installation

Um das Programm zu kompilieren, benötigen Sie die installierte Bibliothek `ncurses` oder `ncursesw`. Sie können das Programm mit folgendem Befehl kompilieren:

```bash
gcc typing_simulator.c -o typing_simulator -lncursesw
```

Falls Sie Probleme mit `ncursesw` haben, versuchen Sie `ncurses` zu verwenden:

```bash
gcc typing_simulator.c -o typing_simulator -lncurses
```

## Verwendung

Nach der Kompilierung können Sie das Programm wie folgt ausführen:

```bash
./typing_simulator
```

Das Programm simuliert dann den Tippvorgang eines Textes aus der Datei "code.c", hebt Schlüsselwörter und Zeichenfolgen hervor und zeigt eine Statuszeile ähnlich wie in Vim an.

### Beispielverwendung:

```bash
./typing_simulator example.c
```

## Einstellungen

Sie können die Parameter des Programms ändern, wie z.B. die minimale und maximale Verzögerung zwischen den Eingaben von Zeichen sowie die Wahrscheinlichkeit eines Fehlers, indem Sie die entsprechenden `#define`-Werte im Code ändern:

```c
#define DELAY_MIN 40000      // Minimale Verzögerung in Mikrosekunden
#define DELAY_MAX 400000     // Maximale Verzögerung in Mikrosekunden
#define ERROR_PROB 0.018     // Wahrscheinlichkeit eines Fehlers (1,8%)
```

## Anwendung

**Typing Simulator kann nützlich sein für:**

- Die Aufnahme von Programmier-Screencasts: Erstellen Sie eine realistische Nachbildung des Live-Codings.
- Einfach zum Spaß: Beobachten Sie, wie Ihr Computer "von selbst" Code schreibt.

## Plattformunterstützung

Das Programm ist kompatibel mit Linux und macOS. Für Windows wird die Verwendung von Terminal-Emulatoren wie WSL empfohlen.

## Lizenz

Dieses Projekt wird unter der BSD-2-Clause-Lizenz vertrieben. Weitere Details finden Sie in der LICENSE-Datei.

## Fazit

Typing Simulator ist ein nützliches Werkzeug zur Erstellung realistischer Demonstrationen der Arbeit in Vim, das für die Aufnahme von Lehrvideos hilfreich sein kann.
