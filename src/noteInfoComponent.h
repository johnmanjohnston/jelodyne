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
};
} // namespace jelodyne
