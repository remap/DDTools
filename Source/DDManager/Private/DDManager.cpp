//
//  DDVase.cpp
//  uShellTest
//
//  Created by Peter Gusev on 2/11/20.
//  Copyright 2020 Regents of the University of California
//

#include "DDManager.h"
#include "DDLog.h"

#include <AssetRegistryModule.h>

#define LOCTEXT_NAMESPACE "FDDModuleManager"

using namespace std;

//******************************************************************************
//
static IDDModuleManagerInterface *SingletonInstance = nullptr;

IDDModuleManagerInterface* FDDModuleManager::getSharedInstance()
{
    return SingletonInstance;
}

void FDDModuleManager::registerModule(IDDModuleInterface *module)
{
    if (module)
    {
        if (find(registeredModules_.begin(),registeredModules_.end(), module) == registeredModules_.end())
        {
            registeredModules_.push_back(module);
            DLOG_DEBUG("Successfully registered module {}",
                       TCHAR_TO_ANSI(*module->getModuleName()));
        }
        else
            DLOG_WARN("Module {} is already registered",
                      TCHAR_TO_ANSI(*module->getModuleName()));
    }
    else
        DLOG_ERROR("Bad module provided: can't register NULL module");
}

void FDDModuleManager::StartupModule()
{
    DLOG_DEBUG("DD Module Manager Startup");

    checkf(!SingletonInstance, TEXT("DD Module Manager singleton is already initialized"));
    SingletonInstance = this;
}

void FDDModuleManager::ShutdownModule()
{
    DLOG_DEBUG("DD Module Manager Shutdown");
}


//******************************************************************************

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDDModuleManager, DDManager)
