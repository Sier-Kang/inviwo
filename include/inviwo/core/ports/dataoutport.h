#ifndef IVW_DATAOUTPORT_H
#define IVW_DATAOUTPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/outport.h>

namespace inviwo {

template<typename T>
class DataOutport : public Outport {

public:
    DataOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~DataOutport();

    void initialize();
    void deinitialize();

    virtual T* getData();
    virtual const T* getConstData() const;

    void setData(T* data, bool ownsData = true);
    void setConstData(const T* data);

    bool hasData() const;
    bool isDataOwner() const;

	virtual bool isReady() const { return isConnected(); }

    virtual void dataChanged() {}

protected:
    T* data_;
    bool ownsData_;
};

template <typename T>
DataOutport<T>::DataOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
    : Outport(identifier, invalidationLevel),
      data_(NULL), ownsData_(true)
{
}

template <typename T>
DataOutport<T>::~DataOutport() {
    if(ownsData_ && data_)
        delete data_;
}

template <typename T>
void DataOutport<T>::initialize(){}

template <typename T>
void DataOutport<T>::deinitialize(){}

template <typename T>
T* DataOutport<T>::getData() {
    return data_;
}

template <typename T>
const T* DataOutport<T>::getConstData() const{
    return const_cast<const T*>(data_);
}

template <typename T>
void DataOutport<T>::setData(T* data, bool ownsData) {
    if(ownsData_ && data_) {
        //Delete old data
        delete data_;
    }
    ownsData_ = ownsData;
    //Add reference to new data
    data_ = data;
    dataChanged();
}

template <typename T>
void DataOutport<T>::setConstData(const T* data) {
    if(ownsData_ && data_) {
        //Delete old data
        delete data_;
    }
    ownsData_ = false;
    //Add reference to new data
    data_ = const_cast<T*>(data);
    dataChanged();
}

template <typename T>
bool DataOutport<T>::hasData() const {
    return (data_ != NULL);
}

template <typename T>
bool DataOutport<T>::isDataOwner() const {
    return ownsData_;
}

} // namespace

#endif // IVW_DATAOUTPORT_H
