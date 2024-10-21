// vtex_dll.cpp
#include "pch.h"
#include "../../../vtfcmd/stdafx.h"
#include "../../../vtflib/vtflib.h"
#include <stdio.h>
#include <string.h>
typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
class IVTex
{
public:
    virtual void FUCKOFF();
    // For use by command-line tools
    virtual int VTex(int argc, char** argv) = 0;
};

// Simplified VTex function - only handles TGA to VTF conversion
int ProcessVTexConversion(int argc, char* argv[]) {
    if (argc < 2) { // Need at least the input file
        return -1; // Indicate error (you might want to define specific error codes)
    }

    vlInitialize();
    ilInit();

    vlUInt imageHandle = 0;
    if (vlCreateImage(&imageHandle) != vlTrue) {
        return -1;
    }


    // Simplified loading and saving (no error checking for brevity - add it in a real application)
    ilInit();
    ilLoadImage(argv[argc - 1]); // Last argument is the input file
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    SVTFCreateOptions options;
    vlImageCreateDefaultCreateStructure(&options);
    options.ImageFormat = IMAGE_FORMAT_DXT5; // Or choose the format you need
    vlImageCreateSingle(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetData(), &options);

    char outputFile[512];
    strcpy_s(outputFile, sizeof(outputFile), argv[argc - 1]);
    char* ext = strrchr(outputFile, '.');
    if (ext) {
        strcpy_s(ext, sizeof(outputFile) - (ext - outputFile), ".vtf");
    }
    vlImageSave(outputFile);

    vlDeleteImage(imageHandle);
    vlShutdown();
    ilShutDown();

    return 0; // Success
}



// IVTex implementation
class CVTex : public IVTex {
public:
    virtual void FUCKOFF() override {

    }
    virtual int VTex(int argc, char** argv) override {
        return ProcessVTexConversion(argc, argv);
    }
};


// Interface export
extern "C" IVTex* CreateVTexInterface(CreateInterfaceFn factory, const char* version) {
    if (strcmp(version, "VTEX_003") == 0) {
        return new CVTex;
    }
    return nullptr;
}

// CreateInterface for VTEX_003
#define VTEX_INTERFACE_VERSION "VTEX_003"
extern "C" __declspec(dllexport) void* CreateInterface(const char* pName, int* pReturnCode) {
    if (pReturnCode) {
        *pReturnCode = 0;
    }
    if (strcmp(pName, VTEX_INTERFACE_VERSION) == 0) {
        return CreateVTexInterface(nullptr, "VTEX_003"); // Return IVTex interface
    }
    return nullptr;
}

void IVTex::FUCKOFF()
{
}
