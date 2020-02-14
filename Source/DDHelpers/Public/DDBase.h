//
//  DDBase.h
//  uShellTest
//
//  Created by Peter Gusev on 2/11/20.
//  Copyright 2020 Regents of the University of California
//

#ifndef __ddbase_h__
#define __ddbase_h__

#include <stdio.h>

#include "Object.h"
#include "ScriptMacros.h"

class IDDModuleInterface {
public:
    virtual FString getModuleName() const = 0;
    virtual FString getModuleVersion() const = 0;
    virtual FString getBuildType() const = 0;
    virtual FString getNetworkMode() const = 0;
};

#endif
