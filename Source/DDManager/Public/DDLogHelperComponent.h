// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDLog.h"
#include "DDLogHelperComponent.generated.h"

UCLASS(ClassGroup=(DDLogInspector),Blueprintable, meta=(BlueprintSpawnableComponent))
class DDLOG_API ULogEntryItem : public UObject {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    FString logMessage;
    
    // TODO: add these properties
//    UPROPERTY(BlueprintReadWrite)
//    FString logLevel;
//
//    UPROPERTY(BlueprintReadWrite)
//    FString moduleName;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewLogMessages,
                                  const TArray<FString>&, messages);

UCLASS( ClassGroup=(DDLogInspector), meta=(BlueprintSpawnableComponent) )
class DDLOG_API UDDLogHelperComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDDLogHelperComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(BlueprintCallable, BlueprintAssignable)
    FOnNewLogMessages OnNewLogMessages;
    
    UFUNCTION(BlueprintCallable)
    void SetFilter(TSet<FString> logLevels, TSet<FString> modules);
    
private:
    // loggers callback
    void onNewLogMessage(std::string module,
                         const std::string& message,
                         const ddlog::helpers::log_level& lvl);
		
};
