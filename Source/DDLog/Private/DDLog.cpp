//
// DDLog.cpp
//
//  Generated on January 27 2020
//  Template created by Peter Gusev on 27 January 2020.
//  Copyright 2013-2019 Regents of the University of California
//

#include "DDLog.h"

#define LOCTEXT_NAMESPACE "FDDLogModule"

using namespace std;
using namespace ddlog;

FDDLogModule::FDDLogModule() {}
FDDLogModule::~FDDLogModule() {}
void FDDLogModule::StartupModule()
{
    initLogger("DDLog");
    
#if 0 // "how to log" demo
    // C++ logging with different macro:

    DLOG_INFO("Game Instance Network Version {}", networkVersion);
    // 1603125771.242920 [GrasshopperAR] [info    ] [thread 6141809] UGHGameInstance() : Game Instance Network Version 2416766325

    DLOG_DEBUG("GH GAME MODE CTOR")
    // 1603125771.242941 [GrasshopperAR] [debug   ] [thread 6141809] AGHGameMode() : GH GAME MODE CTOR

    DLOG_MODULE_DEBUG(FiducialRelocalizer, "Added fiducial {}. Has FAnchor: {}",
                      TCHAR_TO_ANSI(*trackedFiducial->getName()),
                      (fanchor ? "YES" : "NO"));
    // 1603128633.309707 [FiducialRelocalizer] [debug   ] [thread 5674216] AddNewFiducial() : Added fiducial ftanks. Has FAnchor: YES

    DLOG_MODULE_DEBUG(PlaybackCtrl, "plugin panel {}", pluginPanelName);
    // 1603128596.528327 [PlaybackCtrl] [debug   ] [thread 5674216] initWidgetPanel() : plugin panel BP_PlaybackCtrlModulePanel_C
#endif
}
void FDDLogModule::ShutdownModule() {}

void FDDLogModule::initLogger(std::string loggerName)
{
    newLogger(loggerName);
    logger_ = ddlog::getLogger(loggerName);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDDLogModule, DDLog)
