#include "jlistenerbroadcaster.h"
#include <JuceHeader.h> // for DBG

JBroadcaster::~JBroadcaster() { this->l = NULL; }
void JBroadcaster::setListener(JListener *source) { this->l = source; }

void JBroadcaster::broadcastMessage(void *data, void *metadata) {
    l->JListenerCallback(data, metadata, this);
}

void JListener::JListenerCallback(void *data, void *metadata,
                                  JBroadcaster *source) {
    DBG("JListenerCallback() was called but not overridden");
}
