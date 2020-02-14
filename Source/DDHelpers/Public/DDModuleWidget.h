// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDModuleWidget.generated.h"

class IDDModuleInterface;

/**
 *
 */
UCLASS()
class DDHELPERS_API UDDModuleWidget : public UUserWidget
{
public:
    UFUNCTION(BlueprintCallable, Category = "DD Module Widget")
    FName getModuleName();

    void setModule(IDDModuleInterface *module);

protected:
    const IDDModuleInterface *module_;

	GENERATED_BODY()

};
