#include <inviwo/core/ports/port.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

Port::Port(std::string identifier,
           PropertyOwner::InvalidationLevel invalidationLevel)
    : identifier_(identifier),
      invalidationLevel_(invalidationLevel),
      processor_(0)
{}

Port::~Port() {}

uvec3 Port::getColorCode() const { 
    return uvec3(128,128,128); 
}

Processor* Port::getProcessor() const { 
    return processor_; 
}

std::string Port::getIdentifier() const {
    return identifier_; 
}

void Port::setIdentifier(const std::string& name) {
    identifier_ = name;
}

void Port::setProcessor(Processor* processor) { 
    processor_ = processor; 
}

void Port::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) { 
    invalidationLevel_ = std::max(invalidationLevel_, invalidationLevel);
    //TODO: for port properties Port::invalidate() should be called here
    processor_->invalidate(invalidationLevel);
}

void Port::serialize(IvwSerializer& s) const {
    s.serialize("identifier", identifier_, true);
    s.serialize("Processor", processor_);
}

void Port::deserialize(IvwDeserializer& d) {
    d.deserialize("identifier", identifier_, true);
    d.deserialize("Processor", processor_);
}

} // namespace
