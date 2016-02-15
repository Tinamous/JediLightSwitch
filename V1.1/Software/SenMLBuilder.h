 
#ifndef SenMLBuilder_H
#define SenMLBuilder_H

// Build a SenML json document
// Note: Max document length is 255 characters
// as set by the max data size for Particle.Publish()
// see https://docs.particle.io/reference/firmware/photon/#particle-publish-
// For senml format see http://tools.ietf.org/html/draft-jennings-senml-10
class SenMLBuilder {
public:
    SenMLBuilder();
    ~SenMLBuilder();
    
    
    // Start building the SenML string
    void begin();
    
    void add(String fieldName, String value);
    void add(String fieldName, int value);
    void add(String fieldName, float value);
    void add(String fieldName, uint16_t value);
    
    // Create the SenML document
    String toString();
    
    // Publish the senml document
    bool publish();
private:
    String _senml;
    bool _first = true;
    bool _terminated = false;
    void prependComma();
};

#endif