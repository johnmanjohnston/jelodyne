#include "jlistenerbroadcaster.h"
#include <JuceHeader.h> // for DBG

JBroadcaster::~JBroadcaster() { this->l = NULL; }
void JBroadcaster::setListener(JListener *source) { this->l = source; }

void JBroadcaster::broadcastMessage(void *data, void *metadata) {
    if (l == NULL)
        l->JListenerCallback(data, metadata, this);
    else
        DBG("JListenerCallback() was called on a broadcaster with no listener");
}

void JListener::JListenerCallback(void *data, void *metadata,
                                  JBroadcaster *source) {
    DBG("JListenerCallback() was called but not overridden");
}
