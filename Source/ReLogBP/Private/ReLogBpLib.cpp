// Fill out your copyright notice in the Description page of Project Settings.


#include "ReLogBpLib.h"
#include "logging.hpp"

using namespace std;

void UReLogBpLib::LogInfo(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    RLOG_INFO(text);
}

void UReLogBpLib::LogWarning(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    RLOG_WARN(text);
}

void UReLogBpLib::LogError(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    RLOG_ERROR(text);
}

void UReLogBpLib::LogDebug(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    RLOG_DEBUG(text);
}

void UReLogBpLib::LogTrace(FString fText)
{
    string text(TCHAR_TO_UTF8(*fText));
    RLOG_TRACE(text);
}

