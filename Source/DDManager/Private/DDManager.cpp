//
//  DDVase.cpp
//  uShellTest
//
//  Created by Peter Gusev on 2/11/20.
//  Copyright 2020 Regents of the University of California
//

#include "DDManager.h"
#include "DDLog.h"
#include "DDModuleWidget.h"

#include <AssetRegistryModule.h>

#define BUILD_TYPE_FULL(BuildType)(BuildType "")
#define BUILD_TYPE "Unknown"

#if WITH_EDITOR
    #undef BUILD_TYPE_FULL
    #define BUILD_TYPE_FULL(BuildType)(BuildType " Editor")
#endif

#if UE_BUILD_DEBUG
    #undef BUILD_TYPE
    #define BUILD_TYPE "Debug"
#elif UE_BUILD_DEVELOPMENT
    #undef BUILD_TYPE
    #define BUILD_TYPE "Development"
#elif UE_BUILD_TEST
    #undef BUILD_TYPE
    #define BUILD_TYPE "Test"
#elif UE_BUILD_SHIPPING
    #undef BUILD_TYPE
    #define BUILD_TYPE "Shipping"
#endif

#define LOCTEXT_NAMESPACE "FDDModuleManager"

using namespace std;

//******************************************************************************
//
static IDDModuleManagerInterface *SingletonInstance = nullptr;

IDDModuleManagerInterface* FDDModuleManager::getSharedInstance()
{
    if (!SingletonInstance) DLOG_ERROR("DDModuleManager is not initialized. This may result in crash");

    return SingletonInstance;
}

void FDDModuleManager::onPostWorldCreation(UWorld *world)
{
    if (world)
    {
        DLOG_DEBUG("Set world to {}", TCHAR_TO_ANSI(*world->GetMapName()));
        
        FDDModuleManager *singleton = (FDDModuleManager *)FDDModuleManager::getSharedInstance();
        
        singleton->notifyPostWorldCreation(world);
    }
    else
        DLOG_ERROR("the world is null");
}

void FDDModuleManager::onPostWorldInitialization(UWorld *world, UWorld::InitializationValues iValues)
{
    if (world)
    {
        DLOG_DEBUG("World initialized: {}", TCHAR_TO_ANSI(*world->GetMapName()));
        
        FDDModuleManager *singleton = (FDDModuleManager *)FDDModuleManager::getSharedInstance();
        
        singleton->lastWorldCreated_ = world;
        singleton->notifyPostWorldInitialization(world);
    }
    else
        DLOG_ERROR("the world is null");
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

const std::vector<IDDModuleInterface*>& FDDModuleManager::getRegisteredModules()
{
    return registeredModules_;
}

IDDModuleInterface* FDDModuleManager::getModule(FString moduleName) const
{
    for (auto ddModule : registeredModules_)
    {
        if (ddModule->getModuleName().Equals(moduleName))
            return ddModule;
    }
    
    return nullptr;
}

FString FDDModuleManager::getBuildType() const
{
    return FString(BUILD_TYPE_FULL(BUILD_TYPE));
}

UDDModuleWidget* FDDModuleManager::createWidget(UUserWidget* parentWidget, FString moduleName) const
{
    IDDModuleInterface* m = getModule(moduleName);
    if (m)
    {
        if (m->getWidgetBlueprint())
        {
            DLOG_PLUGIN_DEBUG(DDManager, "creating widget for module {}",
                TCHAR_TO_ANSI(*m->getModuleName()));

            UDDModuleWidget* w = CreateWidget<UDDModuleWidget>(parentWidget,
                m->getWidgetBlueprint());

            if (w)
            {
                w->setModule(m);

                DLOG_PLUGIN_DEBUG(DDManager, "successfully created widget for module {}",
                    TCHAR_TO_ANSI(*ddModule->getModuleName()));

                return w;
            }
            else
                DLOG_PLUGIN_WARN(DDManager, "Failed to create widget for module {}",
                    TCHAR_TO_ANSI(*ddModule->getModuleName()));
        }
    }

    return nullptr;
}

void FDDModuleManager::StartupModule()
{
    checkf(!SingletonInstance, TEXT("DD Module Manager singleton is already initialized"));
    SingletonInstance = this;
 
    DLOG_DEBUG("DD Module Manager Startup");
    
    FWorldDelegates::OnPostWorldCreation.AddStatic( &FDDModuleManager::onPostWorldCreation);
    FWorldDelegates::OnPostWorldInitialization.AddStatic(&onPostWorldInitialization);
}

void FDDModuleManager::ShutdownModule()
{
    DLOG_DEBUG("DD Module Manager Shutdown");
}

void FDDModuleManager::notifyPostWorldCreation(UWorld *world)
{
    //DLOG_DEBUG("notify {} modules", registeredModules_.size());
    
    for (auto ddModule : registeredModules_)
        ddModule->onPostWorldCreation(world);
}

void FDDModuleManager::notifyPostWorldInitialization(UWorld *world)
{
    //DLOG_DEBUG("notify {} modules", registeredModules_.size());
    
    for (auto ddModule : registeredModules_)
        ddModule->onPostWorldInitialization(world);
}

//******************************************************************************

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDDModuleManager, DDManager)
