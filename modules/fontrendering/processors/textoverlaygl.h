/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Johan Noren
 *
 *********************************************************************************/

#ifndef IVW_TEXTOVERLAYGL_H
#define IVW_TEXTOVERLAYGL_H

#include <modules/fontrendering/fontrenderingmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace inviwo {

class IVW_MODULE_FONTRENDERING_API TextOverlayGL : public ProcessorGL {
public:
    TextOverlayGL();
    ~TextOverlayGL();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    void render_text(const char* text, float x, float y, float sx, float sy, unsigned int unitNumber);

protected:
    virtual void process();

private:
    ImageInport inport_;
    ImageOutport outport_;
    FT_Library fontlib_;
    FT_Face fontface_;
    StringProperty textStringProperty_;

    unsigned int font_size_;
    float xpos_;
    float ypos_;
    
    FloatVec4Property floatColor_;
    OptionPropertyInt optionPropertyIntFontSize_;
    FloatVec2Property floatVec2FontPos_;

    Shader* copyShader_;
    Shader* textShader_;

    GLuint vboCharacter_;
    GLuint texCharacter_;
};

} // namespace

#endif // IVW_TEXTOVERLAYGL_H