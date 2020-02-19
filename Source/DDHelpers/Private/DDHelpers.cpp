//
//  uHelpers.cpp
//  uShellTest
//
//  Created by Peter Gusev on 2/11/20.
//  Copyright 2020 Regents of the University of California
//

#include "DDHelpers.h"

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
    namespace Detail
    {
#if 0
        void GetAllNativeSubclasses(TArray< TAssetSubclassOf< UObject > >& Subclasses, TSubclassOf< UObject > Base, bool bAllowAbstract)
        {
            check(Base);

            if(!Base->IsNative())
            {
                return;
            }

            for(TObjectIterator< UClass > ClassIt; ClassIt; ++ClassIt)
            {
                UClass* Class = *ClassIt;

                // Only interested in native C++ classes
                if(!Class->IsNative())
                {
                    continue;
                }

                // Ignore deprecated
                if(Class->HasAnyClassFlags(CLASS_Deprecated | CLASS_NewerVersionExists))
                {
                    continue;
                }

#if WITH_EDITOR
                // Ignore skeleton classes (semi-compiled versions that only exist in-editor)
                if(FKismetEditorUtilities::IsClassABlueprintSkeleton(Class))
                {
                    continue;
                }
#endif

                // Optionally ignore abstract classes
                if(!bAllowAbstract && Class->HasAnyClassFlags(CLASS_Abstract))
                {
                    continue;
                }

                // Check this class is a subclass of Base
                if(!Class->IsChildOf(Base))
                {
                    continue;
                }

                Subclasses.Add(Class);
            }
        }
#endif

        void GetAllBlueprintSubclasses(TArray< TAssetSubclassOf< UObject > >& Subclasses,
                                       FName BaseClassName /*TSubclassOf< UObject > Base*/,
                                       bool bAllowAbstract, FString const& Path)
        {
            /*
            For blueprint classes, things are complicated by the fact that the UClass may not have been loaded into memory yet.
            The approach taken here is a bit more complicated than it has to be, but allows us to gather the list of subclasses
            without force loading anything.
            */

            static const FName GeneratedClassTag = TEXT("GeneratedClass");
            static const FName ClassFlagsTag = TEXT("ClassFlags");

//            check(Base);

            // Load the asset registry module
            FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >(FName("AssetRegistry"));
            IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
            // The asset registry is populated asynchronously at startup, so there's no guarantee it has finished.
            // This simple approach just runs a synchronous scan on the entire content directory.
            // Better solutions would be to specify only the path to where the relevant blueprints are,
            // or to register a callback with the asset registry to be notified of when it's finished populating.
            TArray< FString > ContentPaths;
            ContentPaths.Add(TEXT("/Game"));
            if (!Path.IsEmpty())
            {
//                DLOG_INFO("Add search in {}", TCHAR_TO_ANSI(*Path));
                ContentPaths.Add(Path);
            }

            AssetRegistry.ScanPathsSynchronous(ContentPaths);

            //FName BaseClassName = Base->GetFName();

            // Use the asset registry to get the set of all class names deriving from Base
            TSet< FName > DerivedNames;
            {
                TArray< FName > BaseNames;
                BaseNames.Add(BaseClassName);
//                DLOG_INFO("searching for classes derived from {}",
//                          TCHAR_TO_ANSI(*BaseClassName.ToString()));

                TSet< FName > Excluded;
                AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);
            }

//            DLOG_INFO("derived class names num {}", DerivedNames.Num());
//            for (auto n : DerivedNames)
//                DLOG_INFO("DERIVED NAME {}", TCHAR_TO_ANSI(*n.ToString()));

            // Set up a filter and then pull asset data for all blueprints in the specified path from the asset registry.
            // Note that this works in packaged builds too. Even though the blueprint itself cannot be loaded, its asset data
            // still exists and is tied to the UBlueprint type.

            FARFilter Filter;
            Filter.ClassNames.Add(FName("WidgetBlueprint"));
            Filter.ClassNames.Add(BaseClassName);

//            for (auto cn : Filter.ClassNames)
//                DLOG_INFO(" -- filter ClassName {}", TCHAR_TO_ANSI(*cn.ToString()));

            //UBlueprint::StaticClass()->GetFName());
            Filter.bRecursiveClasses = true;
            if(!Path.IsEmpty())
            {
                Filter.PackagePaths.Add(*Path);
            }
            Filter.bRecursivePaths = true;

            TArray< FAssetData > AssetList;
            AssetRegistry.GetAssets(Filter, AssetList);

//            DLOG_INFO("loaded AssetList, n items {}", AssetList.Num());

            // Iterate over retrieved blueprint assets
            for(auto const& Asset : AssetList)
            {
//                DLOG_INFO("item {}",
//                          TCHAR_TO_ANSI(*Asset.AssetName.ToString()));

//                TArray<FName> tagKeys;
//                TArray<FName> tagValues;
//                Asset.TagsAndValues.GenerateKeyArray(tagKeys);
//                Asset.TagsAndValues.GenerateValueArray(tagValues);

//                DLOG_INFO("item tag keys");
//                for (auto tagKey : tagKeys)
//                    DLOG_INFO("{}", TCHAR_TO_ANSI(*tagKey.ToString()));
//                DLOG_INFO("item tag values");
//                for (auto tagValue : tagValues)
//                    DLOG_INFO("{}", TCHAR_TO_ANSI(*tagValue.ToString()));

                // Get the the class this blueprint generates (this is stored as a full path)
                auto findResult = Asset.TagsAndValues.FindTag(GeneratedClassTag);

//                DLOG_INFO("find result isSet {}", findResult.IsSet());

                if(findResult.IsSet())
                {
                    auto GeneratedClassPathPtr = findResult.GetValue();
                    // Optionally ignore abstract classes
                    // As of 4.12 I do not believe blueprints can be marked as abstract, but this may change so included for completeness.
//                    if(!bAllowAbstract)
//                    {
//                        auto findResult = Asset.TagsAndValues.FindTag(ClassFlagsTag);
//                        if(findResult.IsSet())
//                        {
//                            auto ClassFlagsPtr = findResult.GetValue();
//                            auto ClassFlags = FCString::Atoi(**ClassFlagsPtr);
//                            if((ClassFlags & CLASS_Abstract) != 0)
//                            {
//                                continue;
//                            }
//                        }
//                    }

                    // Convert path to just the name part
                    const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(*GeneratedClassPathPtr);
                    const FString ClassName = FPackageName::ObjectPathToObjectName(ClassObjectPath);

//                    DLOG_INFO("OBJECT PATH {} CLASS NAME {}",
//                              TCHAR_TO_ANSI(*ClassObjectPath),
//                              TCHAR_TO_ANSI(*ClassName));

                    // Check if this class is in the derived set
//                    if(!DerivedNames.Contains(*ClassName))
//                    {
//                        continue;
//                    }

                    // Store using the path to the generated class
                    Subclasses.Add(TAssetSubclassOf< UObject >(FStringAssetReference(ClassObjectPath)));
                }
            }
        }
    }

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

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDDHelpersModule, DDHelpers)
