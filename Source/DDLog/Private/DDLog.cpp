//
// DDLog.cpp
//
//  Generated on January 27 2020
//  Template created by Peter Gusev on 27 January 2020.
//  Copyright 2013-2019 Regents of the University of California
//

#include "DDLog.h"
#include <Engine/World.h>
#include <GameFramework/PlayerController.h>
#include <AssetRegistryModule.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include "DDModuleWidget.h"
#include "DDHelpers.h"

#define LOCTEXT_NAMESPACE "FDDLogModule"

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

#define NETMODE_WORLD (((GEngine == nullptr) || (GetWorld() == nullptr)) ? "Unknown" \
: (GEngine->GetNetMode(GetWorld()) == NM_Client) ? "Client" \
: (GEngine->GetNetMode(GetWorld()) == NM_ListenServer) ? "ListenServer" \
: (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer) ? "DedicatedServer" \
: "Standalone")

using namespace std;
using namespace ddlog;

FDDLogModule::FDDLogModule() {}
FDDLogModule::~FDDLogModule() {}

void FDDLogModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("StartupModule() called"));

    // TODO: set it up identical to boilerplate code
    DLOG_INFO("DDLOG MODULE STARTUP START");

    initModule("DDLog", "0.0.1");
    DLOG_INFO("DDLOG MODULE STARTUP FINISH");
}

void FDDLogModule::ShutdownModule(){}

FString FDDLogModule::getModuleName() const
{
    return FString(moduleName_.c_str());
}

FString FDDLogModule::getModuleVersion() const
{
    return FString(pluginVersion_.c_str());
}

FString FDDLogModule::getBuildType() const
{
    return FString(BUILD_TYPE_FULL(BUILD_TYPE));
}

FString FDDLogModule::getNetworkMode() const
{
    // TODO: Fix NETMODE_WORLD (GetWorld() call) for non-actor class...
    return FString("TBD"); //NETMODE_WORLD);
}

void FDDLogModule::initModule(string moduleName, string pluginVersion)
{
    moduleName_ = moduleName;
    pluginVersion_ = pluginVersion;
    initLogger(moduleName_);

    DLOG_PLUGIN_INFO("{} module initialized. Version {}. Build Type {}. Network mode {}",
                     moduleName,
                     TCHAR_TO_ANSI(*getModuleVersion()),
                     TCHAR_TO_ANSI(*getBuildType()),
                     TCHAR_TO_ANSI(*getNetworkMode()));

    initWidgetPanel();
}

void FDDLogModule::initLogger(std::string loggerName)
{
    newLogger(loggerName);
    logger_ = getLogger(loggerName);
}

void FDDLogModule::initWidgetPanel()
{
    string packagePath = "/"+moduleName_;
    TArray<TAssetSubclassOf<UObject>> widgets;
    ddhelpers::GetAllBlueprintSubclasses(widgets,
                                        FName("DDModuleWidget"),
                                        false,
                                        FString(packagePath.c_str()));

    if (widgets.Num())
    {
//        widgetOwner_ = NewObject<UDDWidgetOwner>();
//        infoPanel_ = CreateWidget<UDDModuleWidget>(widgetOwner_, widgets[0].Get());

        if (!infoPanel_)
            DLOG_PLUGIN_WARN("Can't load default widget");
        else
        {
            infoPanel_->setModule(this);

            DLOG_PLUGIN_INFO("Loaded default widget panel");
        }
    }
    else
    {
        DLOG_PLUGIN_WARN("No default widgets found");
        infoPanel_ = nullptr;
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDDLogModule, DDLog)
