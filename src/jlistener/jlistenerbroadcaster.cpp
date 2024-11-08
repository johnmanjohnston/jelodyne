#include "jlistenerbroadcaster.h"
#include <iostream>

void JBroadcaster::setListener(JListener *source) { this->l = source; }

void JBroadcaster::broadcastMessage(void *data, void *metadata) {
    l->JListenerCallback(data, metadata, this);
}

void JListener::JListenerCallback(void *data, void *metadata,
                                  JBroadcaster *source) {
    std::cout << "JListenerCallback() was called, but was not overridden";
}
