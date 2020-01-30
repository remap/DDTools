//
// ReLog.cpp
//
//  Generated on January 27 2020
//  Template created by Peter Gusev on 27 January 2020.
//  Copyright 2013-2019 Regents of the University of California
//

#include "ReLog.h"

#define LOCTEXT_NAMESPACE "FReLogModule"

FReLogModule::FReLogModule() {}
FReLogModule::~FReLogModule() {}

void FReLogModule::StartupModule()
{
    
}

void FReLogModule::ShutdownModule()
{
    
}

void FReLogModule::initLogger(std::string loggerName)
{
    newLogger(loggerName);
    logger_ = getLogger(loggerName);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FReLogModule, ReLog)
