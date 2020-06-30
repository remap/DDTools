// Fill out your copyright notice in the Description page of Project Settings.


#include "DDBlueprintLibrary.h"
#include "DDLog.h"
#include "DDManager.h"
#include <Misc/NetworkVersion.h>

using namespace std;

void UDDBlueprintLibrary::LogInfo(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    DLOG_INFO(text);
}

void UDDBlueprintLibrary::LogWarning(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    DLOG_WARN(text);
}

void UDDBlueprintLibrary::LogError(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    DLOG_ERROR(text);
}

void UDDBlueprintLibrary::LogDebug(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    DLOG_DEBUG(text);
}

void UDDBlueprintLibrary::LogTrace(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    DLOG_TRACE(text);
}

FString UDDBlueprintLibrary::getGameNetMode()
{
    UWorld *world = FDDModuleManager::getSharedInstance()->getLastWorldCreated();
    
    if (world)
    {
        ENetMode mode = world->GetNetMode();
        
        switch (mode) {
            case NM_Standalone:
                return FString("NM_Standalone");
            case NM_DedicatedServer:
                return FString("NM_DedicatedServer");
            case NM_ListenServer:
                return FString("NM_ListenServer");
            case NM_Client:
                return FString("NM_Client");
            default:
                break;
                
        }
    }
    
    return FString("Unknown");
}

FString UDDBlueprintLibrary::getNetworkVersion()
{
    // this is ridiculous -- blueprints don't support uint32 so
    // we do printf of uint to char* then convert to FString
    // why, Epic?...
    
    char networkVersion[256];
    memset(networkVersion, 0, 256);
    
    unsigned int nwVersion =  FNetworkVersion::GetLocalNetworkVersion(true);
    sprintf(networkVersion, "%u", nwVersion);
    
    return FString(networkVersion);
}

FString UDDBlueprintLibrary::getBuildType()
{
    return FDDModuleManager::getSharedInstance()->getBuildType();
}

FString UDDBlueprintLibrary::getLoggingLevel()
{
    return FString(spdlog::level::to_short_c_str(ddlog::getMainLogger()->level()));
}

FString UDDBlueprintLibrary::getServerIp()
{
    UWorld *world = FDDModuleManager::getSharedInstance()->getLastWorldCreated();
     
     if (world)
     {
         ENetMode mode = world->GetNetMode();
         
         switch (mode) {
             case NM_Standalone:
                 return FString();
             default:
                 return world->URL.Host;
         }
     }
    
    return FString();
}

int UDDBlueprintLibrary::getServerPort()
{
    UWorld *world = FDDModuleManager::getSharedInstance()->getLastWorldCreated();
    
    if (world)
        return world ->URL.Port;
    
    return 0;
}

FString UDDBlueprintLibrary::GetCrossPlatformWriteableFolder()
{
#if PLATFORM_ANDROID
    char cmdLine[512];
    FILE *f = fopen("/proc/self/cmdline", "r");
    if (f)
    {
        size_t c = fread(cmdLine, 512, 1, f);
    }
    else
        DLOG_ERROR("FAILED TO READ /proc/self/cmdline");
    
    return FString(("/data/data/"+string(cmdLine)).c_str());
#elif PLATFORM_IOS
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    return FString([documentsDirectory UTF8String]);
#else
    return FPaths::ProjectUserDir();
#endif
}

ULevel* UDDBlueprintLibrary::getActorLevel(AActor* actor)
{
    return actor->GetLevel();
}
