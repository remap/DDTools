//
//  uHelpers.cpp
//  uShellTest
//
//  Created by Peter Gusev on 2/11/20.
//  Copyright 2020 Regents of the University of California
//

#include "DDHelpers.h"
#include "DDLog.h"
#include "DDModuleWidget.h"

#include <AssetRegistryModule.h>
#include <ARFilter.h>
#include <UObject/Class.h>
#include <UObject/UObjectIterator.h>

#define LOCTEXT_NAMESPACE "FDDHelpersModule"

#if WITH_EDITOR
#include "KismetEditorUtilities.h"
#endif

namespace ddhelpers
{

// helpful info https://github.com/kamrann/KantanCodeExamples/blob/master/Source/A1_GatherSubclasses/Runtime/A1_GatherSubclasses.h
int
getWidgets(FString widgetBaseClass, FString const& path,
           TArray<TAssetSubclassOf<UObject>>& widgets)
{
    if (widgetBaseClass.IsEmpty())
        widgetBaseClass = FString("UserWidget");

    // step 1 -- get all asset of asset class "WidgetBlueprint"
    
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >(FName("AssetRegistry"));
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
    TArray< FString > ContentPaths;
    
    if (!&AssetRegistry)
    {
        DLOG_ERROR("AssetRegistry is invalid");
        return 0;
    }
    
    if (!path.IsEmpty())
    {
        DLOG_TRACE("Add search in {}", TCHAR_TO_ANSI(*path));
        ContentPaths.Add(path);
    }
    else
        ContentPaths.Add(TEXT("/Game"));
    
    
    AssetRegistry.ScanPathsSynchronous(ContentPaths);
    FARFilter Filter;
    
    FName AssetClassName = "WidgetBlueprint";
    Filter.ClassNames.Add(AssetClassName);
    Filter.bRecursiveClasses = true;
    Filter.bRecursivePaths = true;
    
    if(!path.IsEmpty())
        Filter.PackagePaths.Add(*path);
    
    TArray< FAssetData > AssetList;
    AssetRegistry.GetAssets(Filter, AssetList);
    
    DLOG_TRACE("loaded AssetList, n items {}", AssetList.Num());
    
    static const FName ClassFlagsTag = TEXT("ClassFlags");
    
    // step 2 -- go through the found widgets, inspect its' ParentClass and leave only those that
    // have proper class
    for(auto const& Asset : AssetList)
    {
        DLOG_TRACE(" --- item {} class name {}",
                   TCHAR_TO_ANSI(*Asset.AssetName.ToString()),
                   TCHAR_TO_ANSI(*Asset.AssetClass.ToString()));

        static const FName ParentClassTag = TEXT("ParentClass");
        static const FName NativeParentClassTag = TEXT("NativeParentClass");

        auto res = Asset.TagsAndValues.FindTag(ParentClassTag);
        
        if (res.IsSet())
        {
            
            const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(*res.GetValue());
            
            DLOG_TRACE("item parent class object path {}",
                       TCHAR_TO_ANSI(*ClassObjectPath));
            
            const FString ClassName = FPackageName::ObjectPathToObjectName(ClassObjectPath);
            DLOG_TRACE("item parent class name {}",
                       TCHAR_TO_ANSI(*ClassName));
            
            
            if (ClassName.Equals(widgetBaseClass))
            {
                // step 3 -- extract Blueprint generated class from the asset and add an asset
                // reference to the final array
                static const FName GeneratedClassTag = TEXT("GeneratedClass");
                
                auto findResult = Asset.TagsAndValues.FindTag(GeneratedClassTag);
                if(findResult.IsSet())
                {
                    auto GeneratedClassPathPtr = findResult.GetValue();
                    const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(*GeneratedClassPathPtr);
                    const FString ClassName = FPackageName::ObjectPathToObjectName(ClassObjectPath);
                    
                    DLOG_TRACE(" --- generated class object path {} generated class name {}",
                               TCHAR_TO_ANSI(*ClassObjectPath),
                               TCHAR_TO_ANSI(*ClassName));
                    
                    widgets.Add(TAssetSubclassOf< UObject >(FStringAssetReference(ClassObjectPath)));
                }
                else
                    DLOG_DEBUG("item has no generated class");
            }
        }
        else
            DLOG_TRACE("item has no parent class tag");
    } // for
    
    return widgets.Num();
}

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDDHelpersModule, DDHelpers)
