// Fill out your copyright notice in the Description page of Project Settings.


#include "DDBlueprintLibrary.h"
#include "logging.hpp"

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
