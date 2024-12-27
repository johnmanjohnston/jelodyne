#pragma once
#include "noteComponent.h"
#include <JuceHeader.h>

namespace jelodyne {
class NoteInfoComponent : public juce::Component {
  public:
    NoteInfoComponent();
    ~NoteInfoComponent();

    NoteComponent *noteComponent = nullptr;

    void paint(juce::Graphics &g) override;
    void setCorrespondingNoteComponent(NoteComponent *nc);

    juce::Font
    getInterBoldItalic(); // TODO: move this into a static class so you
                          // dont need to define this function for every
                          // class you need getting this font fo
};
} // namespace jelodyne
