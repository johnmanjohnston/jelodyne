#pragma once

class JListener;

class JBroadcaster {
  public:
    void setListener(JListener *listener);
    void broadcastMessage(void *data, void *metadata);

    JListener *l;
};

class JListener {
  public:
    virtual ~JListener() = default;
    virtual void JListenerCallback(void *data, void *metadata,
                                   JBroadcaster *source);
};
