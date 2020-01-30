//
// ReLog.h
//
//  Generated on January 27 2020
//  Template created by Peter Gusev on 27 January 2020.
//  Copyright 2013-2020 Regents of the University of California
//

// A Boilerplate code for a C++/BP plugin that handles third-party dependencies automatically.
// How to use:
// 1. Give a name to your plugin replacing placeholder <#Your Module Name#>
// 2. Rename all plugin files and folders according your plugin's name:
//      Boilerplate.h, Boilerplate.cpp, Boilerplate.Build.cs, Boilerplate.uplugin
// 3. Fix #include "Boilerplate.h" in .cpp file
//
// To add third-party dependencies:
// 1. Place your third-party folder (that has header files and binaries) in the "deps" subfolder.
// 2. Your third-party dependencies must have this folder structure:
//      <third-party lib name>
//          *- include      // headers should be placed here
//          *- lib          // (optional, can skip for header-only dependencies)
//              *- Android  // include only those platforms, that your third-party supports
//              *- IOS
//              *- macOS
//              *- Win64

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "logging.hpp"

#define RLOG_PLUGIN_TRACE(...) RLOG_LOGGER_TRACE(logger_, ##__VA_ARGS__)
#define RLOG_PLUGIN_DEBUG(...) RLOG_LOGGER_DEBUG(logger_, ##__VA_ARGS__)
#define RLOG_PLUGIN_INFO(...) RLOG_LOGGER_INFO(logger_, ##__VA_ARGS__)
#define RLOG_PLUGIN_WARN(...) RLOG_LOGGER_WARN(logger_, ##__VA_ARGS__)
#define RLOG_PLUGIN_ERROR(...) RLOG_LOGGER_ERROR(logger_, ##__VA_ARGS__)

class DLLEXPORT FReLogModule : public IModuleInterface
{
public:

    FReLogModule();
    virtual ~FReLogModule();
    
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
    
protected:
    std::shared_ptr<relog::helpers::logger> logger_;
    
    void initLogger(std::string loggerName);
};
