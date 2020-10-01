// Fill out your copyright notice in the Description page of Project Settings.


#include "DDBlueprintLibrary.h"
#include "DDLog.h"
#include "DDManager.h"
#include <Misc/NetworkVersion.h>

#if PLATFORM_MAC | PLATFORM_IOS
#import <Foundation/Foundation.h>
#endif

using namespace std;

void UDDBlueprintLibrary::LogInfo(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    DLOG_INFO(text);
    
    // in-editor printing
    if(GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, fText);
}

void UDDBlueprintLibrary::LogWarning(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    DLOG_WARN(text);
    
    // in-editor printing
    if(GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, fText);
}

void UDDBlueprintLibrary::LogError(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    DLOG_ERROR(text);
    
    // in-editor printing
    if(GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, fText);
}

void UDDBlueprintLibrary::LogDebug(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    DLOG_DEBUG(text);
    
    // in-editor printing
    if(GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, fText);
}

void UDDBlueprintLibrary::LogTrace(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    DLOG_TRACE(text);
    
    // in-editor printing
    if(GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, fText);
}

FString UDDBlueprintLibrary::getGameNetMode()
{
    if (FDDModuleManager::getSharedInstance())
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
    if (ddlog::getMainLogger())
        return FString(spdlog::level::to_short_c_str(ddlog::getMainLogger()->level()));
    return FString("n/a");
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

FString UDDBlueprintLibrary::getBundleVersionString()
{
#if PLATFORM_MAC | PLATFORM_IOS
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    return FString([version UTF8String]);
#else
    return FString();
#endif
}
