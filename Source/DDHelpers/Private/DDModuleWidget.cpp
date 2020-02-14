// Fill out your copyright notice in the Description page of Project Settings.


#include "DDModuleWidget.h"
#include "DDBase.h"

FName
UDDModuleWidget::getModuleName()
{
    if (module_)
        return FName(*module_->getModuleName());

    return FName(TEXT("n/a"));
}

void
UDDModuleWidget::setModule(IDDModuleInterface *module)
{
    module_ = module;
}
