# SID Studio 64 — v0.3.5 — ChatID C5A9E2D7

## BUG-06: hou vas gedraagt zich als default

Oorzaak in v0.3.4: `Arpeggiator::noteOff(note, hold)` onderdrukte het verwijderen van een noot wanneer hold aan stond, maar hield geen aparte fysieke-toetsstatus bij. Daardoor kon een gelatchte lijst niet worden opgeschoond wanneer `Hou vas` weer UIT ging. Ook bleef de lijst bestaan bij wisselen van speelmodus.

Wijzigingen: afzonderlijke `physical_` en `latched_` status, `setHold(false)` verwijdert alle losgelaten noten, moduswisseling leegt noten en stopt audio, `prepareToPlay` reset state. Default van `houvas` blijft false. Tests voor loslaten van 1/alle toetsen, hold aan/uit, chord replacement en richtingen.

## Belangrijk
De test bewijst gedrag van de C++ arpeggiator; echte Logic MIDI Note Off en audiogedrag moeten op de Mac worden geaccepteerd. De stappensequencer mag autonoom spelen. De AU-versie wordt in `CMakeLists.txt` naar 0.3.5 gezet, maar JUCE/macOS kan de componentversie apart publiceren; verifieer met `auval`.
