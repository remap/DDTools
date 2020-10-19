// Fill out your copyright notice in the Description page of Project Settings.


#include "DDLogHelperComponent.h"
#include "DDLog.h"
#include "logging.hpp"
#include "DDManager.h"
#include <list>
#include <memory>
#include <mutex>
#include <functional>
#include <set>
#include <sstream>

#define MESSAGE_BUFFER_SIZE 1000

using namespace std;
using namespace ddlog;

class DDLoggerInspector : enable_shared_from_this<DDLoggerInspector> {
public:
    static shared_ptr<DDLoggerInspector> getSharedInstance();

    struct LogEntry {
        helpers::log_level lvl_;
        string module_;
        FString message_;
    };
    
    struct LogFilter {
        set<helpers::log_level> logLevels_;
        set<string> moduleNames_;
        
        bool checkEntry(const helpers::log_level& lvl,
                   const string& moduleName,
                   const FString& msg)
        {
            return logLevels_.find(lvl) != logLevels_.end() &&
            (moduleNames_.size() == 0 ||
             moduleNames_.find(moduleName) != moduleNames_.end());
        }
    };
    
    ~DDLoggerInspector();
    
    bool init();
    TArray<FString> getUnread(int limit = 0);
    void setFilter(const LogFilter& filter);
    
private:
    static shared_ptr<DDLoggerInspector> sharedInstance_;
    
    DDLoggerInspector(size_t);
    
    shared_ptr<helpers::logger> logger_;
    size_t bufferSize_;
    list<LogEntry> buffer_;
    mutex bufferMt_;
    list<LogEntry>::iterator unreadBegin_;
    LogFilter filter_;
    
    void onNewLogMessage(string module,
                         const string& message,
                         const helpers::log_level& lvl);
    
    void addToBuffer(LogEntry&& entry);
};

shared_ptr<DDLoggerInspector>
DDLoggerInspector::getSharedInstance()
{
    static shared_ptr<DDLoggerInspector> shared;
    static once_flag f;
    call_once(f, [&](){
        shared.reset(new DDLoggerInspector(MESSAGE_BUFFER_SIZE));
    });
    
    return shared;
}

DDLoggerInspector::DDLoggerInspector(size_t bufferSize) :
bufferSize_(bufferSize)
{
    newLogger("DDLoggerInspector");
    logger_ = getLogger("DDLoggerInspector");
    
    unreadBegin_ = buffer_.end();
}

DDLoggerInspector::~DDLoggerInspector()
{
    
}

bool
DDLoggerInspector::init()
{
    IDDModuleManagerInterface *manager = FDDModuleManager::getSharedInstance();
    
    if (!manager)
    {
        DLOG_LOGGER_TRACE(logger_, "DDLoggerInspector: Module Manager is null. can't proceed");
        return false;
    }
    else
    {
        vector<IDDModuleInterface*> modules = manager->getRegisteredModules();
        shared_ptr<DDLoggerInspector> me = DDLoggerInspector::getSharedInstance();
        
        registerCallback(getMainLogger(),
                         bind(&DDLoggerInspector::onNewLogMessage,
                                                        this, getMainLogger()->name(),
                                                        std::placeholders::_1, std::placeholders::_2));
        
        for (auto &m : modules)
        {
            shared_ptr<helpers::logger> l = getLogger(TCHAR_TO_ANSI(*m->getModuleName()));

            if (l)
            {
                registerCallback(l,
                                 bind(&DDLoggerInspector::onNewLogMessage,
                                      this, l->name(),
                                      std::placeholders::_1, std::placeholders::_2));
            }
        }
    }
    
    return true;
}

void
DDLoggerInspector::onNewLogMessage(string module,
                                   const string& message,
                                   const helpers::log_level& lvl)
{
    addToBuffer({lvl, module, FString(message.c_str())});
}

void
DDLoggerInspector::addToBuffer(LogEntry&& entry)
{
    lock_guard<mutex> scopedLock(bufferMt_);
    
    buffer_.push_front(entry);
    
    while (buffer_.size() > bufferSize_)
    {
        if (unreadBegin_ == --buffer_.end())
            unreadBegin_ = buffer_.end();
        buffer_.pop_back();
    }
}

TArray<FString>
DDLoggerInspector::getUnread(int limit)
{   
    lock_guard<mutex> scopedLock(bufferMt_);
    TArray<FString> unread;
    
    while (unreadBegin_ != buffer_.begin())
    {
        unreadBegin_--;
        
        LogEntry& le = *unreadBegin_;
        if (filter_.checkEntry(le.lvl_, le.module_, le.message_))
            unread.Add(le.message_);
    }
    
    return unread;
}

void
DDLoggerInspector::setFilter(const DDLoggerInspector::LogFilter& f)
{
    stringstream ss;
    ss << "log levels ";
    for (auto &l : f.logLevels_) ss << spdlog::level::to_short_c_str(l) << " ";
    ss << "module names ";
    for (auto &m : f.moduleNames_) ss << m << " ";
    DLOG_LOGGER_TRACE(logger_, "set new filter: {}", ss.str());
    
    filter_ = f;
}

//******************************************************************************

// Sets default values for this component's properties
UDDLogHelperComponent::UDDLogHelperComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDDLogHelperComponent::BeginPlay()
{
	Super::BeginPlay();
    DDLoggerInspector::getSharedInstance()->init();
}

// Called every frame
void UDDLogHelperComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TArray<FString> unreadLogMessages = DDLoggerInspector::getSharedInstance()->getUnread();

    if (unreadLogMessages.Num())
    {
        OnNewLogMessages.Broadcast(unreadLogMessages);
    }
}

void UDDLogHelperComponent::SetFilter(TSet<FString> logLevels, TSet<FString> modules)
{
    DDLoggerInspector::LogFilter filter;
    
    for (auto &e : logLevels)
    {
        helpers::log_level lvl = spdlog::level::from_str(TCHAR_TO_ANSI(*e.ToLower()));
        filter.logLevels_.insert(lvl);
    }
    
    for (auto &m : modules)
        filter.moduleNames_.insert(TCHAR_TO_ANSI(*m));
    
    DDLoggerInspector::getSharedInstance()->setFilter(filter);
}
