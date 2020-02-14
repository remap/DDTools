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

// common interface for DD modules
class IDDModuleInterface
{
public:
    virtual FString getModuleName() const = 0;
    virtual FString getModuleVersion() const = 0;
    virtual FString getBuildType() const = 0;
    virtual FString getNetworkMode() const = 0;
};

// #ifndef __ddhelpers_h__
// #define __ddhelpers_h__

// adapted from https://github.com/kamrann/KantanCodeExamples/blob/master/Source/A1_GatherSubclasses/Runtime/A1_GatherSubclasses.h
namespace ddhelpers
{
    namespace Detail
    {
#if 1
    // causes link error
    //
    // Undefined symbols for architecture x86_64:
    //  "FKismetEditorUtilities::IsClassABlueprintSkeleton(UClass const*)", referenced from:
    //          ddhelpers::Detail::GetAllNativeSubclasses(TArray<TSoftClassPtr<UObject>, TSizedDefaultAllocator<32> >&, TSubclassOf<UObject>, bool) in Module.DDHelpers.cpp.o
    // ld: symbol(s) not found for architecture x86_64
    //
    // adding "Kismet" module as a dependency does not help
        DLLEXPORT void GetAllNativeSubclasses(TArray< TAssetSubclassOf< UObject > >& OutSubclasses, TSubclassOf< UObject > Base, bool bAllowAbstract);
#endif
        DLLEXPORT void GetAllBlueprintSubclasses(TArray< TAssetSubclassOf< UObject > >& OutSubclasses, FName BaseClassName/*TSubclassOf< UObject > Base*/, bool bAllowAbstract, FString const& Path);
    }

#if 1
    template < typename TBase >
    void GetAllNativeSubclasses(TArray< TAssetSubclassOf< TBase > >& Subclasses, TSubclassOf< TBase > Base, bool bAllowAbstract)
    {
        TArray< TAssetSubclassOf< UObject > > Classes;
        Detail::GetAllNativeSubclasses(Classes, Base, bAllowAbstract);
        for(auto const& Cls : Classes)
        {
            Subclasses.Add(Cls);
        }
    }
#endif

    //template < typename TBase >
    void GetAllBlueprintSubclasses(TArray< TAssetSubclassOf< UObject > >& Subclasses,
                                   FName BaseClassName /*TSubclassOf< TBase > Base*/,
                                   bool bAllowAbstract, FString const& Path)
    {
        TArray< TAssetSubclassOf< UObject > > Classes;
        Detail::GetAllBlueprintSubclasses(Classes, BaseClassName, bAllowAbstract, Path);
        for(auto const& Cls : Classes)
        {
            Subclasses.Add(Cls);
        }
    }
}

// #endif
