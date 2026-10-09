// Vliegbasis71 SID Studio 64 | v0.3.5 | ChatID C5A9E2D7
#include "PluginEditor.hpp"
namespace {
const juce::Colour beige(0xffc6bba7), cream(0xffe7decb), dark(0xff443a32), brown(0xff615348), blue(0xff3347a0), cyan(0xff8cdbef);
const juce::StringArray langs{"AF","NL","РУ"};
}
SIDEditor::SIDEditor(SIDProcessor& p) : AudioProcessorEditor(&p), sidProcessor(p) {
    setSize(1040,680);
    setResizable(true,true);
    setResizeLimits(850,570,1600,1000);
    mode.addItemList({"Oorspronklik", "Uitgebrei"},1);
    wave.addItemList({"Driehoek", "Saagtand", "Puls", "Ruis"},1);
    for(auto* c : {static_cast<juce::Component*>(&mode),static_cast<juce::Component*>(&wave),static_cast<juce::Component*>(&modeLabel),static_cast<juce::Component*>(&waveLabel)}) addAndMakeVisible(*c);
    modeAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(sidProcessor.params,"modus",mode);
    waveAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(sidProcessor.params,"golf",wave);
    const char* ids[]={"aanval","verval","aanhou","vrylating"};
    for(int i=0;i<4;++i) {
        auto& sl=envSliders[(size_t)i]; sl.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        sl.setTextBoxStyle(juce::Slider::TextBoxBelow,false,68,19);
        sl.setColour(juce::Slider::rotarySliderFillColourId,cyan);
        sl.setColour(juce::Slider::rotarySliderOutlineColourId,dark);
        sl.setColour(juce::Slider::textBoxTextColourId,cream);
        sl.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
        addAndMakeVisible(sl); addAndMakeVisible(envLabels[(size_t)i]);
        envAttachments[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(sidProcessor.params,ids[i],sl);
    }
    const char* chipIds[]={"pulsbreedte","filtergrens","resonansie"};
    const char* switchIds[]={"filteraan","ring","sink"};
    for(int i=0;i<3;++i) {
        auto& sl=chipSliders[(size_t)i];
        sl.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        sl.setTextBoxStyle(juce::Slider::TextBoxBelow,false,82,18);
        sl.setColour(juce::Slider::rotarySliderFillColourId,cyan);
        sl.setColour(juce::Slider::textBoxTextColourId,cream);
        sl.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
        addAndMakeVisible(sl); addAndMakeVisible(chipLabels[(size_t)i]);
        chipAttachments[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(sidProcessor.params,chipIds[i],sl);
        addAndMakeVisible(chipSwitches[(size_t)i]);
        chipSwitchAttachments[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(sidProcessor.params,switchIds[i],chipSwitches[(size_t)i]);
    }
    for(int i=0;i<3;++i) {
        auto& b=languageButtons[(size_t)i]; b.setButtonText(langs[i]); addAndMakeVisible(b);
        b.onClick=[this,i]{language=i;updateLanguage();repaint();};
    }
    for(auto* b:{&soundTab,&sequenceTab,&effectsTab}) addAndMakeVisible(*b);
    soundTab.onClick=[this]{page=0;showPage();repaint();};
    sequenceTab.onClick=[this]{page=1;showPage();repaint();};
    effectsTab.onClick=[this]{page=2;showPage();repaint();};
    playMode.addItemList({"Regstreeks","Stappe","Arpeggio"},1);
    arpDirection.addItemList({"Op","Af","Op en af","Willekeurig","Speelvolgorde"},1);
    for(auto* c:{static_cast<juce::Component*>(&playMode),static_cast<juce::Component*>(&arpDirection),
                 static_cast<juce::Component*>(&holdButton),static_cast<juce::Component*>(&playModeLabel),
                 static_cast<juce::Component*>(&arpDirectionLabel)}) addAndMakeVisible(*c);
    playModeAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(sidProcessor.params,"speelwyse",playMode);
    arpDirectionAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(sidProcessor.params,"arprigting",arpDirection);
    holdAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(sidProcessor.params,"houvas",holdButton);
    seqDivision.addItemList({"1/4","1/8","1/16","1/32"},1);
    seqLength.addItemList({"16","32"},1);
    seqNote.setSliderStyle(juce::Slider::LinearHorizontal);
    seqNote.setTextBoxStyle(juce::Slider::TextBoxRight,false,48,24);
    for(auto* c:{static_cast<juce::Component*>(&seqEnabled),static_cast<juce::Component*>(&seqDivision),
                 static_cast<juce::Component*>(&seqLength),static_cast<juce::Component*>(&seqNote),
                 static_cast<juce::Component*>(&tempoLabel),static_cast<juce::Component*>(&seqHelp)}) addAndMakeVisible(*c);
    seqEnabledAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(sidProcessor.params,"reeksaan",seqEnabled);
    seqDivisionAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(sidProcessor.params,"reeksdeling",seqDivision);
    seqLengthAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(sidProcessor.params,"reekslengte",seqLength);
    seqNoteAttachment=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(sidProcessor.params,"reeksnoot",seqNote);
    for(int i=0;i<32;++i) {
        steps[static_cast<size_t>(i)].setButtonText(juce::String(i+1));
        addAndMakeVisible(steps[static_cast<size_t>(i)]);
        stepAttachments[static_cast<size_t>(i)]=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(sidProcessor.params,"stap"+juce::String(i),steps[static_cast<size_t>(i)]);
    }
    startTimerHz(12);
    updateLanguage();
    showPage();
}
void SIDEditor::updateLanguage() {
    const juce::StringArray modes[3]={{"Oorspronklik","Uitgebrei"},{"Oorspronkelijk","Uitgebreid"},{"Оригинал","Расширенный"}};
    const juce::StringArray waves[3]={{"Driehoek","Saagtand","Puls","Ruis"},{"Driehoek","Zaagtand","Puls","Ruis"},{"Треугольник","Пила","Импульс","Шум"}};
    const juce::StringArray labels[3]={{"Aanval","Verval","Aanhou","Vrylating"},{"Aanval","Verval","Vasthouden","Loslaten"},{"Атака","Спад","Уровень","Затухание"}};
    const int mi=mode.getSelectedId(), wi=wave.getSelectedId();
    mode.clear(juce::dontSendNotification); wave.clear(juce::dontSendNotification);
    mode.addItemList(modes[language],1); wave.addItemList(waves[language],1);
    mode.setSelectedId(mi,juce::dontSendNotification); wave.setSelectedId(wi,juce::dontSendNotification);
    const char* tabNames[3][3]={{"Klank","Reekse","Effekte"},{"Klank","Reeksen","Effecten"},{"Звук","Секвенции","Эффекты"}};
    soundTab.setButtonText(tabNames[language][0]);sequenceTab.setButtonText(tabNames[language][1]);effectsTab.setButtonText(tabNames[language][2]);
    modeLabel.setText(language==2?"Режим":(language==1?"Modus":"Klankwêreld"),juce::dontSendNotification);
    waveLabel.setText(language==2?"Волна":"Golfvorm",juce::dontSendNotification);
    for(int i=0;i<4;++i) envLabels[(size_t)i].setText(labels[language][i],juce::dontSendNotification);
    const char* chipNames[3][3]={{"Pulswydte","Filtergrens","Resonansie"},{"Pulsbreedte","Filtergrens","Resonantie"},{"Ширина импульса","Частота фильтра","Резонанс"}};
    const char* switchNames[3][3]={{"Filter aan","Ringmodulasie","Sinkronisasie"},{"Filter aan","Ringmodulatie","Synchronisatie"},{"Фильтр вкл.","Кольцевая модуляция","Синхронизация"}};
    for(int i=0;i<3;++i) { chipLabels[(size_t)i].setText(chipNames[language][i],juce::dontSendNotification);chipSwitches[(size_t)i].setButtonText(switchNames[language][i]); }
    playModeLabel.setText(language==2?"Режим":(language==1?"Speelmodus":"Speelwyse"),juce::dontSendNotification);
    arpDirectionLabel.setText(language==2?"Направление":(language==1?"Richting":"Rigting"),juce::dontSendNotification);
    holdButton.setButtonText(language==2?"Удерживать":(language==1?"Vasthouden":"Hou vas"));
    const int selectedMode=playMode.getSelectedId(), selectedDirection=arpDirection.getSelectedId();
    const juce::StringArray modeNames[3]={{"Regstreeks","Stappe","Arpeggio"},{"Direct","Stappen","Arpeggio"},{"Напрямую","Шаги","Арпеджио"}};
    const juce::StringArray directionNames[3]={{"Op","Af","Op en af","Willekeurig","Speelvolgorde"},{"Omhoog","Omlaag","Op en neer","Willekeurig","Speelvolgorde"},{"Вверх","Вниз","Вверх-вниз","Случайно","Порядок нажатия"}};
    playMode.clear(juce::dontSendNotification);playMode.addItemList(modeNames[language],1);playMode.setSelectedId(selectedMode,juce::dontSendNotification);
    arpDirection.clear(juce::dontSendNotification);arpDirection.addItemList(directionNames[language],1);arpDirection.setSelectedId(selectedDirection,juce::dontSendNotification);
    seqEnabled.setButtonText(language==2?"Включить ритм":(language==1?"Reeks aan":"Reeks aan"));
    seqHelp.setText(language==2?"Скорость от программы; шаги следуют позиции":
        (language==1?"Tempo volgt Logic Pro; stappen volgen de afspeelpositie":
                     "Tempo volg Logic Pro; stappe volg die speelposisie"),juce::dontSendNotification);
    for(int i=0;i<3;++i) languageButtons[(size_t)i].setColour(juce::TextButton::buttonColourId,i==language?blue:brown);
}
void SIDEditor::drawPanel(juce::Graphics& g,juce::Rectangle<int> r,const juce::String& title) {
    g.setColour(dark);g.fillRoundedRectangle(r.toFloat(),12.f);
    g.setColour(cream);g.setFont(15.f);g.drawText(title,r.removeFromTop(38).reduced(16,6),juce::Justification::centredLeft);
}
void SIDEditor::paint(juce::Graphics& g) {
    const auto w=getWidth(),h=getHeight();
    g.fillAll(juce::Colour(0xff272320));
    auto shell=getLocalBounds().reduced(10);
    g.setColour(beige);g.fillRoundedRectangle(shell.toFloat(),24.f);
    g.setColour(cream);g.drawRoundedRectangle(shell.toFloat().reduced(1),24.f,2.f);
    g.setColour(dark);
    for(int y=66;y<117;y+=10) {g.fillRect(24,y,w/3,5);g.fillRect(w*2/3,y,w/3-26,5);}
    g.setColour(brown);g.fillRoundedRectangle(juce::Rectangle<float>(w/2.f-185,24,370,113),12.f);
    g.setColour(blue);g.fillRoundedRectangle(juce::Rectangle<float>(w/2.f-177,32,354,97),8.f);
    g.setColour(cyan);g.setFont(juce::Font(juce::FontOptions(16.f).withStyle("bold")));
    g.drawText("**** SID STUDIO 64 ****",w/2-163,39,326,25,juce::Justification::centred);
    g.setFont(14.f);g.drawText("MOS 6581  /  Vliegbasis71",w/2-160,67,320,22,juce::Justification::centred);
    g.drawText("v0.3.5  |  C5A9E2D7",w/2-160,91,320,23,juce::Justification::centred);
    g.setColour(dark);g.setFont(juce::Font(juce::FontOptions(24.f).withStyle("bold")));
    g.drawText("64",w-105,139,65,43,juce::Justification::centred);
    auto r=juce::Rectangle<int>(26,215,w-52,h-252);
    if(page==0) {
        const int gap=12, col=(r.getWidth()-2*gap)/3;
        drawPanel(g,{r.getX(),r.getY(),col,r.getHeight()},language==2?"ГЕНЕРАТОР":(language==1?"KLANKBRON":"KLANKBRON"));
        drawPanel(g,{r.getX()+col+gap,r.getY(),col,r.getHeight()},language==2?"ОГИБАЮЩАЯ":(language==1?"KLANKVERLOOP":"KLANKVERLOOP"));
        drawPanel(g,{r.getX()+2*(col+gap),r.getY(),col,r.getHeight()},language==2?"ОБЗОР":"OORSIG");
        g.setColour(cream);g.setFont(14.f);
        g.drawFittedText(language==2?"6581: регистры и фильтр":(language==1?"6581: registers en filter":"6581: registers en filter"),r.getX()+2*(col+gap)+18,r.getY()+43,col-35,25,juce::Justification::centredLeft,1);
    } else if(page==1) {
        drawPanel(g,r,language==2?"ПОШАГОВЫЙ РИТМ":(language==1?"STAPPENREEKS":"STAPREEKS"));
        g.setColour(cyan);g.setFont(18.f);
        g.drawText(language==2?"Деление / Длина / Нота":(language==1?"Verdeling / Lengte / Noot":"Verdeling / Lengte / Noot"),r.getX()+30,r.getY()+102,420,26,juce::Justification::centredLeft);
    } else {
        drawPanel(g,r,page==1?(language==2?"СЕКВЕНЦИИ":(language==1?"REEKSEN":"REEKSE")):(language==2?"ЭФФЕКТЫ":(language==1?"EFFECTEN":"EFFEKTE")));
        g.setColour(cyan);g.setFont(20.f);
        g.drawText(language==2?"В разработке — пока без обработки звука":(language==1?"In ontwikkeling — nog geen geluidsverwerking":"In ontwikkeling — nog geen klankverwerking"),r.reduced(30),juce::Justification::centred);
    }
    g.setColour(dark);g.setFont(13.f);g.drawText("Vliegbasis71  •  6581-geïnspireerde prototipe",28,h-32,w-56,18,juce::Justification::centredLeft);
}
void SIDEditor::resized() {
    const int w=getWidth(),h=getHeight();
    for(int i=0;i<3;++i) languageButtons[(size_t)i].setBounds(w-206+i*57,152,52,27);
    soundTab.setBounds(28,153,130,35);sequenceTab.setBounds(164,153,130,35);effectsTab.setBounds(300,153,130,35);
    const int gap=12, col=(w-52-2*gap)/3, top=215;
    modeLabel.setBounds(44,top+66,col-40,23);mode.setBounds(44,top+95,col-40,32);
    waveLabel.setBounds(44,top+161,col-40,23);wave.setBounds(44,top+190,col-40,32);
    const int envX=26+col+gap;
    for(int i=0;i<4;++i) {
        const int x=envX+22+(i%2)*((col-38)/2),y=top+65+(i/2)*145;
        envLabels[(size_t)i].setBounds(x,y,(col-44)/2,23);
        envSliders[(size_t)i].setBounds(x,y+24,(col-44)/2,100);
        const bool visible=page==0;
        envLabels[(size_t)i].setVisible(visible);envSliders[(size_t)i].setVisible(visible);
    }
    const int chipX=26+2*(col+gap);
    for(int i=0;i<3;++i) {
        const int x=chipX+14+i*(col-20)/3;
        chipLabels[(size_t)i].setBounds(x,top+85,(col-20)/3,23);
        chipSliders[(size_t)i].setBounds(x,top+110,(col-20)/3,105);
        chipSwitches[(size_t)i].setBounds(chipX+20,top+238+i*43,col-40,33);
        chipLabels[(size_t)i].setVisible(page==0);
        chipSliders[(size_t)i].setVisible(page==0);
        chipSwitches[(size_t)i].setVisible(page==0);
    }
    playModeLabel.setBounds(52,258,155,24);playMode.setBounds(52,284,160,32);
    arpDirectionLabel.setBounds(232,258,175,24);arpDirection.setBounds(232,284,175,32);
    holdButton.setBounds(430,283,180,33);
    seqEnabled.setBounds(630,283,160,33);
    tempoLabel.setBounds(790,283,210,35);
    seqDivision.setBounds(50,340,130,32);
    seqLength.setBounds(200,340,130,32);
    seqNote.setBounds(360,340,260,32);
    seqHelp.setBounds(50,390,w-100,30);
    const int usable=w-110;
    for(int i=0;i<32;++i) {
        const int colIndex=i%16, row=i/16;
        const int cell=usable/16;
        steps[static_cast<size_t>(i)].setBounds(48+colIndex*cell,450+row*62,cell-2,45);
    }
    showPage();
}

void SIDEditor::showPage() {
    for(auto* c:{static_cast<juce::Component*>(&mode),static_cast<juce::Component*>(&wave),
                  static_cast<juce::Component*>(&modeLabel),static_cast<juce::Component*>(&waveLabel)}) c->setVisible(page==0);
    for(int i=0;i<4;++i) {envLabels[(size_t)i].setVisible(page==0);envSliders[(size_t)i].setVisible(page==0);}
    for(int i=0;i<3;++i) {chipLabels[(size_t)i].setVisible(page==0);chipSliders[(size_t)i].setVisible(page==0);chipSwitches[(size_t)i].setVisible(page==0);}
    for(auto* c:{static_cast<juce::Component*>(&playMode),static_cast<juce::Component*>(&arpDirection),
                 static_cast<juce::Component*>(&holdButton),static_cast<juce::Component*>(&playModeLabel),
                 static_cast<juce::Component*>(&arpDirectionLabel)}) c->setVisible(page==1);
    for(auto* c:{static_cast<juce::Component*>(&seqEnabled),static_cast<juce::Component*>(&seqDivision),
                 static_cast<juce::Component*>(&seqLength),static_cast<juce::Component*>(&seqNote),
                 static_cast<juce::Component*>(&tempoLabel),static_cast<juce::Component*>(&seqHelp)}) c->setVisible(page==1);
    const int length=sidProcessor.params.getRawParameterValue("reekslengte")->load()>0.5f?32:16;
    for(int i=0;i<32;++i) steps[static_cast<size_t>(i)].setVisible(page==1 && i<length);
}
void SIDEditor::timerCallback() {
    const auto bpm=sidProcessor.hostTempo.load(std::memory_order_relaxed);
    const auto playing=sidProcessor.hostPlaying.load(std::memory_order_relaxed);
    tempoLabel.setText(juce::String(bpm,1)+" BPM  |  "+(playing?(language==2?"ИГРАЕТ":(language==1?"SPEELT":"SPEEL")):(language==2?"СТОП":"STOP")),juce::dontSendNotification);
    if(page==1) showPage();
}
