#ifndef IVW_IMAGECLGL_H
#define IVW_IMAGECLGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opengl/glwrap/texture2d.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API ImageCLGL : public ImageRepresentation {

public:
    ImageCLGL(uvec2 dimensions = uvec2(64), ImageType type = COLOR_DEPTH, const DataFormatBase* format = DataFormatBase::get(), const Texture2D* data = NULL);
    virtual ~ImageCLGL();
    virtual std::string getClassName() const { return "ImageCLGL"; }
    virtual void initialize(){};
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
    
    void initialize(const Texture2D* texture);

    virtual void setDimensions(uvec2 dimensions) { dimensions_ = dimensions; deinitialize(); initialize(texture_); }
    virtual void resize(uvec2 dimensions);
    virtual bool copyAndResizeImage(DataRepresentation* target);

    const cl::Image2DGL& getImage() const { return *(image2D_); }
    const Texture2D* getTexture() const { return texture_; }

    void aquireGLObject(std::vector<cl::Event>* syncEvents = NULL) const {
        std::vector<cl::Memory> syncImages(1, *image2D_); 
        OpenCL::instance()->getQueue().enqueueAcquireGLObjects(&syncImages, syncEvents);
    }
    void releaseGLObject(std::vector<cl::Event>* syncEvents = NULL, cl::Event* event= NULL) const {
        std::vector<cl::Memory> syncImages(1, *image2D_); 
        OpenCL::instance()->getQueue().enqueueReleaseGLObjects(&syncImages, syncEvents, event);
    }

protected:
    cl::Image2DGL* image2D_;
    const Texture2D* texture_;
};

} // namespace

namespace cl {

// Kernel argument specializations for ImageCLGL type 
// (enables calling cl::Queue::setArg with ImageCLGL)
template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::ImageCLGL& value);

} // namespace cl



#endif // IVW_IMAGECLGL_H