# SID Studio 64 — Agile productbacklog v0.3.3
ChatID: 8F2C7A41 | 2026-10-09

| ID | Prio | Status | Story / acceptatiecriterium |
|---|---|---|---|
| BUG-01 | P0 | IN TEST | Arpeggio uit C-E-G hoorbaar in Logic; stoppen bij loslaten (standaard) |
| BUG-02 | P0 | IN TEST | Stappen 1/5/9/13 produceren hoorbare audio bij host PLAY |
| BUG-03 | P0 | TODO | Bij MIDI midden in raster start arpeggio op volgende stap, zonder vastlopers |
| BUG-04 | P0 | TODO | Transport stop/seek/loop/tempo wissel zonder hangende noten |
| BUG-05 | P0 | TODO | Standalone heeft eigen PLAY/STOP en tempo |
| BUG-06 | P1 | TODO | Pluginversie AU metadata komt overeen met UI |
| BUG-07 | P1 | TODO | Alle tekst UTF-8 zonder mojibake |
| BUG-08 | P1 | TODO | Resizable UI zonder overlapping bij minimale venstergrootte |
| SEQ-01 | P0 | IN TEST | 16/32 stappen en 1/4–1/32 volgen host PPQ |
| SEQ-02 | P1 | TODO | Individuele toonhoogte per stap (niet één globale noot) |
| SEQ-03 | P1 | TODO | Sample-accurate note-off/gatelengte per stap |
| SEQ-04 | P1 | TODO | 8+ patronen en patroonwissel zonder haperen |
| SEQ-05 | P1 | TODO | Swing, accent, velocity, rusten, transpositie |
| SEQ-06 | P1 | TODO | Loop-wrap en host jump opnieuw uitlijnen |
| ARP-01 | P0 | IN TEST | Op, af, op-en-af, willekeurig, speelvolgorde |
| ARP-02 | P0 | IN TEST | Hou vas default UIT; AAN blijft spelen na note-off |
| ARP-03 | P1 | TODO | Latch reset knop, sustain CC64, akkoordwissels |
| ARP-04 | P1 | TODO | Octaafbereik 1–4 en ritmische gates |
| SYN-01 | P0 | TODO | Audiovergelijking met echte MOS 6581 / betrouwbare emulator |
| SYN-02 | P1 | TODO | SID 6581 vs 8580 filter en chipafwijkingen |
| SYN-03 | P1 | TODO | Gecombineerde golfvormen, voice3-off, oscillator sync tests |
| SYN-04 | P1 | TODO | Polyfonie en voice stealing testen |
| PRE-01 | P1 | TODO | 330 werkelijk verschillende presets, 30 per 11 categorieën |
| PRE-02 | P1 | TODO | Presetbrowser zoeken/filteren/favorieten |
| PRE-03 | P1 | TODO | Eigen presets opslaan/importeren/exporteren |
| UI-01 | P1 | TODO | Volledige AF/NL/RU vertaling, geen Engelse zichtbare plugintekst |
| UI-02 | P2 | TODO | C64 breadbox visuele verfijning, meters, stap-highlight |
| FX-01 | P2 | TODO | Delay, reverb, chorus, distortion met bypass |
| QA-01 | P0 | TODO | Integratietest: gegenereerde arpeggionoten resulteren in niet-stille audio |
| QA-02 | P0 | TODO | AU auval + Logic 12 handmatige luistertest op M4 |
| QA-03 | P1 | TODO | State save/restore, project reopen, automation |
| REL-01 | P2 | TODO | Code signing, notarization, installer en licentiecontrole |

**Definition of Done:** RED test vooraf, GREEN C++/JUCE-build, ctest, auval, Logic-luistertest, code review, versie+ChatID in bron. Geen audio-GREEN op basis van alleen auval.
