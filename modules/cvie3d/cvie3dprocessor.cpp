#include "cvie3dprocessor.h"
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

ProcessorClassName(CVIE3DProcessor, "CVIE3DProcessor"); 
ProcessorCategory(CVIE3DProcessor, "Context Vision");
ProcessorCodeState(CVIE3DProcessor, CODE_STATE_EXPERIMENTAL);

CVIE3DProcessor::CVIE3DProcessor()
    : Processor(),
    inport_("volume.inport"),
    outport_("volume.outport"),
    enabled_("enabled", "Filtering Enabled", false),
    confFile_("confFile", "Configuration file", IVW_DIR+"modules/cvie3D/ext/CVIE3DSDK/bin/CVIE3D.conf"),
    parameterFile_("parameterFile", "Parameter file", IVW_DIR+"modules/cvie3D/ext/CVIE3DSDK/par/default.gop"),
    parameterSetting_("parameterSetting", "Parameter setting", 0)
{
    addPort(inport_);
    addPort(outport_);

    addProperty(enabled_);

    //confFile_.onChange(this, &CVIE3DProcessor::updateConfigurationFile);
    addProperty(confFile_);

    //parameterFile_.onChange(this, &CVIE3DProcessor::updateParameterFile);
    addProperty(parameterFile_);

    addProperty(parameterSetting_);
}

CVIE3DProcessor::~CVIE3DProcessor() {}

void CVIE3DProcessor::initialize() {
    Processor::initialize();
    //createCVIE3DInstance();
}

void CVIE3DProcessor::deinitialize() {
    Processor::deinitialize();
    //destroyCVIE3DInstance();
}

void CVIE3DProcessor::process() {
    if(enabled_.get()){
        bool success = true;

        //Create CVIE3D Instance with Configuration 
        success = createCVIE3DInstance();

        if(success){
            //Set Parameter File of CVIE3D Instance
            success = updateParameterFile();

            if(success){
                //Setup CVIE3D Enhancement
                success = setupEnhancement();

                if(success){
                    //Run CVIE3D Enhancement
                    success = runEnhancement();

                    if(success){
                        //Destroy CVIE3D Instance
                        destroyCVIE3DInstance();
                    }
                }
            }
        }
    }
    else
        outport_.setData(const_cast<Volume*>(inport_.getData()), false);
}

bool CVIE3DProcessor::createCVIE3DInstance(){
    ECVIE3D cvieError = CVIE3DCreate(&cvieHandle_, confFile_.get().c_str(), NULL);

    if (cvieError != ECVIE3D_CudaInitializedOk && cvieError != ECVIE3D_CpuInitializedOk) {
        std::string msg;
        char errstr[512];
        if (cvieError == ECVIE3D_LicenseError) {
            msg = "License error.\n";
        }
        else {
            msg = CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr));
        }

        LogError("Error in createCVIE3DInstance: " << msg.c_str());
        destroyCVIE3DInstance();
        return false;
    }

    return true;
}

void CVIE3DProcessor::destroyCVIE3DInstance(){
    CVIE3DDestroy(&cvieHandle_);
}

bool CVIE3DProcessor::setupEnhancement(){
    uvec3 volSize = inport_.getData()->getDimension();
    ECVIE3D cvieError = CVIE3DSetupEnhance(cvieHandle_, static_cast<int>(volSize.x), static_cast<int>(volSize.y), static_cast<int>(volSize.z), CVIE3D_DATA_U8, CVIE3D_Volume, parameterSetting_.get());

    if (cvieError != ECVIE3D_Ok) {
        char errstr[512];
        LogError("Error in CVIE3DSetupEnhance: " << CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr)));
        destroyCVIE3DInstance();
        return false;
    }

    return true;
}

bool CVIE3DProcessor::runEnhancement(){
    const VolumeRAM* volIn = inport_.getData()->getRepresentation<VolumeRAM>();
    bool allocateNewVolume = (inport_.getData()->getDimension() != outport_.getData()->getDimension());
    VolumeRAM* volOut;

    if(allocateNewVolume)
        volOut = static_cast<VolumeRAM*>(volIn->clone());
    else
        volOut = outport_.getData()->getEditableRepresentation<VolumeRAM>();

    ECVIE3D cvieError = CVIE3DEnhance(cvieHandle_, const_cast<void*>(volIn->getData()), volOut->getData(), 0);

    if (cvieError != ECVIE3D_Ok) {
        char errstr[512];
        LogError("Error in CVIE3DEnhance: " << CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr)));
        destroyCVIE3DInstance();
        return false;
    }

    if(allocateNewVolume)
        outport_.setData(new Volume(volOut));

    return true;
}

void CVIE3DProcessor::updateConfigurationFile(){
    //destroyCVIE3DInstance();
    //createCVIE3DInstance();
}

bool CVIE3DProcessor::updateParameterFile(){
    // Set parameter file
    int nSettings = 0;
    ECVIE3D cvieError = CVIE3DSetParameterFile(cvieHandle_, parameterFile_.get().c_str(), &nSettings);

    if (cvieError != ECVIE3D_Ok) {
        char errstr[512];
        LogError("Error in CVIE3DSetParameterFile: " << CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr)));
        destroyCVIE3DInstance();
        return false;
    }

    return true;
}

} // namespace