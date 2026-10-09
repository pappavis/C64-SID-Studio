🆔 C5A9E2D7 | 09-10-2026, 11:01 CEST | Den Haag

🎹 SID Studio 64 — v0.3.5

🔧 Ronde 3C.2 — BUG-06: Hou vas

De correctie is gebouwd. Standaard staat ‘Hou vas’ UIT, zodat de arpeggiator stopt zodra je alle MIDI-toetsen loslaat.

📦 Download v0.3.5

⬇️ SID Studio 64 v0.3.5 — Hou vas Fix (ZIP)⁠￼

Dit pakket bevat de volledige broncode, CMake-configuratie, build.sh, aangepaste tests en FIX.md.

Wat is gecorrigeerd?

Ik heb de arpeggiator aangepast zodat hij onderscheid maakt tussen fysiek ingedrukte toetsen en vastgehouden (gelatchte) noten.

Situatie	Verwacht gedrag v0.3.5
Plugin openen	Hou vas = UIT
C-akkoord indrukken	Arpeggio begint
Eén toets loslaten	Overige tonen blijven spelen
Alle toetsen loslaten	Arpeggio stopt
Hou vas = AAN	Arpeggio blijft spelen na loslaten
Hou vas weer UIT	Losgelaten noten worden verwijderd
Nieuw akkoord bij Hou vas AAN	Vorig akkoord wordt vervangen
Wisselen van speelmodus	Actieve noten worden gestopt

De zelfstandige Stappe-sequencer blijft onafhankelijk van ingedrukte MIDI-toetsen functioneren.

🧪 Testresultaten

De C++-code is daadwerkelijk gecompileerd en getest in deze omgeving.

sid64_arpeggiator .... Passed
sid64_sequence ....... Passed
sid64_engine ......... Passed
sid64_character ...... Passed
100% tests passed
0 tests failed

4/4 GREEN. De macOS Audio Unit en de hoorbare werking in Logic Pro moeten nog op jouw Mac worden gecontroleerd.

Installeren op je Mac mini M4

Pak de ZIP uit, sluit Logic Pro en voer uit:

cd ~/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5
source ./build.sh

Het script voert de CMake-build, CTest en Audio Unit-validatie uit.

🎯 Acceptatietest in Logic Pro

Open de plugin, selecteer Reekse → Speelwyse → Arpeggio en controleer dat Hou vas = UIT.

Houd een C-akkoord ingedrukt. Je moet de opeenvolgende tonen horen. Laat vervolgens alle toetsen los: het arpeggio moet stoppen.

Schakel daarna Hou vas = AAN. Herhaal het akkoord en laat de toetsen los. Nu moet het arpeggio blijven spelen. Zet Hou vas weer UIT; het moet stoppen.

📋 Agile-status

ID	Functionaliteit	Status
BUG-01	Arpeggio produceert geluid	✅ Door jou bevestigd
BUG-02	Stappe produceert geluid	✅ Door jou bevestigd
BUG-06	Correcte Hou vas / Note Off	🟡 Fix gebouwd, acceptatie open
BUG-07	Noten stoppen bij moduswisseling	🟡 Fix gebouwd, acceptatie open
BUG-08	AU-versienummer correct publiceren	🟡 Verificatie open
QA-04	Geautomatiseerde arpeggiatortests	✅ 4/4 CTest
SEQ-07	90 C64-geïnspireerde reeksen	⏳ Backlog
PRE-01	330 fabrieksklanken	⏳ Backlog

Ronde 3C.2 blijft IN TEST totdat je bevestigt dat de arpeggiator in Logic Pro stopt bij het loslaten van de toetsen.
