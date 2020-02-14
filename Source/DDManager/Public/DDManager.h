//
//  DDBase.h
//  uShellTest
//
//  Created by Peter Gusev on 2/11/20.
//  Copyright 2020 Regents of the University of California
//

#pragma once

#include <stdio.h>
#include <string>
#include <vector>

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "DDHelpers.h"

class IDDModuleManagerInterface {
public:
    virtual void registerModule(IDDModuleInterface* module) = 0;
};

class DLLEXPORT FDDModuleManager : public IModuleInterface, public IDDModuleManagerInterface
{
public:
    static IDDModuleManagerInterface* getSharedInstance();

    void registerModule(IDDModuleInterface *module) override;

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    std::vector<IDDModuleInterface*> registeredModules_;
};

