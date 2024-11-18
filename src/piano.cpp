#include "piano.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
using namespace juce;

jelodyne::piano::pianoRoll::pianoRoll(MidiKeyboardState &state,
                                      Orientation orientation)
    : MidiKeyboardComponent(state, orientation) {}

void jelodyne::piano::pianoRoll::drawWhiteNote(int midiNoteNumber, Graphics &g,
                                               Rectangle<float> area,
                                               bool isDown, bool isOver,
                                               Colour lineColour,
                                               Colour textColour) {
    auto c = Colours::transparentWhite;

    if (isDown)
        c = Colour(200, 200, 200);
    if (isOver)
        c = Colour(210, 210, 210);

    if (pianoScale.isNoteInScale(midiNoteNumber)) {
        c = juce::Colours::red;
    }

    g.setColour(c);
    g.fillRect(area);

    const auto currentOrientation = getOrientation();

    auto text = getWhiteNoteText(midiNoteNumber);

    if (text.isNotEmpty()) {
        g.setColour(textColour);
        g.setFont(12.f);

        switch (currentOrientation) {
        case horizontalKeyboard:
            g.drawText(text, area.withTrimmedLeft(1.0f).withTrimmedBottom(2.0f),
                       Justification::centredBottom, false);
            break;
        case verticalKeyboardFacingLeft:
            g.drawText(text, area.reduced(2.0f), Justification::centredLeft,
                       false);
            break;
        case verticalKeyboardFacingRight:
            g.drawText(text, area.reduced(2.0f), Justification::centredRight,
                       false);
            break;
        default:
            break;
        }
    }

    if (!lineColour.isTransparent()) {
        g.setColour(lineColour);

        switch (currentOrientation) {
        case horizontalKeyboard:
            g.fillRect(area.withWidth(1.0f));
            break;
        case verticalKeyboardFacingLeft:
            g.fillRect(area.withHeight(1.0f));
            break;
        case verticalKeyboardFacingRight:
            g.fillRect(area.removeFromBottom(1.0f));
            break;
        default:
            break;
        }

        if (midiNoteNumber == getRangeEnd()) {
            switch (currentOrientation) {
            case horizontalKeyboard:
                g.fillRect(area.expanded(1.0f, 0).removeFromRight(1.0f));
                break;
            case verticalKeyboardFacingLeft:
                g.fillRect(area.expanded(0, 1.0f).removeFromBottom(1.0f));
                break;
            case verticalKeyboardFacingRight:
                g.fillRect(area.expanded(0, 1.0f).removeFromTop(1.0f));
                break;
            default:
                break;
            }
        }
    }
}

void jelodyne::piano::pianoRoll::drawBlackNote(int midiNoteNumber, Graphics &g,
                                               Rectangle<float> area,
                                               bool isDown, bool isOver,
                                               Colour noteFillColour) {
    auto c = Colour(70, 70, 70);

    if (isDown)
        c = Colour(30, 30, 30);
    if (isOver)
        c = Colour(34, 34, 34);

    if (pianoScale.isNoteInScale(midiNoteNumber)) {
        c = juce::Colours::red;
    }

    g.setColour(c);
    g.fillRect(area);

    if (isDown) {
        g.setColour(noteFillColour);
        g.drawRect(area);
    } else {
        g.setColour(c.brighter());
    }
}

void jelodyne::piano::pianoSynth::init(juce::String sample_path) {
    this->piano_sample_path = sample_path;

    for (int i = 0; i < 5; i++)
        this->addVoice(new juce::SamplerVoice());

    if (afm.findFormatForFileExtension("wav") == nullptr)
        afm.registerBasicFormats();

    BigInteger noteRange;
    noteRange.setRange(0, 128, true);

    juce::File file(piano_sample_path);
    reader =
        std::unique_ptr<juce::AudioFormatReader>(afm.createReaderFor(file));

    this->addSound(new juce::SamplerSound("default", *reader, noteRange, 60,
                                          0.0, 0.1, 9.0));
}
