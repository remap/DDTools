//
//  uHelpers.h
//  uShellTest
//
//  Created by Peter Gusev on 2/11/20.
//  Copyright 2020 Regents of the University of California
//

#pragma once

#include <stdio.h>
#include "Object.h"
#include "ScriptMacros.h"
#include "Blueprint/UserWidget.h"
#include <UObject/Class.h>
#include <UObject/UObjectIterator.h>

// common interface for DD modules
class IDDModuleInterface
{
public:
    virtual FString getModuleName() const = 0;
    virtual FString getModuleVersion() const = 0;
    virtual FString getBuildType() const = 0;
    virtual FString getNetworkMode() const = 0;
    virtual TSubclassOf<UUserWidget> getWidgetBlueprint() = 0;
    virtual void onPostWorldCreation(class UWorld* world) = 0;
    virtual void onPostWorldInitialization(class UWorld* world) = 0;
};

class FDDHelpersModule : public IModuleInterface
{
};

namespace ddhelpers
{

int DLLEXPORT getWidgets(FString widgetBaseClass, FString const& path,
                         TArray<TAssetSubclassOf<UObject>>& widgets);

}
