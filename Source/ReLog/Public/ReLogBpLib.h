// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReLogBpLib.generated.h"

/**
 * 
 */
UCLASS()
class RELOG_API UReLogBpLib : public UBlueprintFunctionLibrary
{
public:
    UFUNCTION(BlueprintCallable, Category = "ReLog")
    static void LogInfo(FString text);
    
    UFUNCTION(BlueprintCallable, Category = "ReLog")
    static void LogWarning(FString text);
    
    UFUNCTION(BlueprintCallable, Category = "ReLog")
    static void LogError(FString text);
    
    UFUNCTION(BlueprintCallable, Category = "ReLog")
    static void LogDebug(FString text);
    
    UFUNCTION(BlueprintCallable, Category = "ReLog")
    static void LogTrace(FString text);
    
	GENERATED_BODY()
	
};
