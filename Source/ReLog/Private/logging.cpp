//
// logging.cpp
//
//  Created by Peter Gusev on 12 November 2019.
//  Copyright 2013-2020 Regents of the University of California
//
#include "logging.hpp"

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Kismet/KismetSystemLibrary.h"

using namespace std;
using namespace relog;

#define DEFAULT_FORMAT "%E.%f [%12n] [%^%-8l%$] [thread %t] %!() : %v"

// logging level
// could be either:
// - "trace"
// - "debug"
// - "info"
// - "warn"
// - "err"
// - "critical"
#define LOG_LEVEL_ENV "RELOG_LOG_LEVEL"
#define LOG_FORMAT_ENV "RELOG_LOG_FMT"
#define LOG_FILE_ENV "RELOG_LOG_FILE"

shared_ptr<spdlog::logger> mainLogger;
string logFile = "";
string logLevel = "";
once_flag onceFlag;

void initMainLogger();
void initLogger(shared_ptr<helpers::logger>);

// init logging upon library loading
struct _LibInitializer {
    _LibInitializer() {
        call_once(onceFlag, bind(initMainLogger));
    }
} libInitializer = {};

//******************************************************************************
// callback sink implementation
// @see https://github.com/gabime/spdlog/wiki/4.-Sinks#implementing-your-own-sink
#include <spdlog/sinks/base_sink.h>

template<typename Mutex>
class CallbackSink : public spdlog::sinks::base_sink <Mutex>
{

public:
    CallbackSink(helpers::LogCallback logCallback) :
    logCallback_(logCallback)
    {}

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        fmt::basic_memory_buffer<char, 250> formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        
        pair<helpers::log_level,string> p(msg.level, fmt::to_string(formatted));
        msgs_.push_back(p);
    }

    void flush_() override
    {
        if (msgs_.size())
        {
            for (auto m:msgs_)
                logCallback_(m.second, m.first);
            msgs_.clear();
        }
    }

private:
    helpers::LogCallback logCallback_;
    vector<pair<helpers::log_level, string>> msgs_;
};

#include <spdlog/details/null_mutex.h>
#include <mutex>
using CallbackSinkMt = CallbackSink<mutex>;
using CallbackSinkSt = CallbackSink<spdlog::details::null_mutex>;

//******************************************************************************
namespace relog
{

void newLogger(string loggerName)
{
    shared_ptr<spdlog::logger> logger;

    if (logFile != "")
        logger = spdlog::basic_logger_mt<spdlog::async_factory>(loggerName, logFile);
    else
        logger = spdlog::stdout_color_mt(loggerName);

    initLogger(logger);
}

shared_ptr<spdlog::logger> getLogger(string loggerName)
{
    auto logger = spdlog::get(loggerName);
    return logger;
}

void flushLogger(string loggerName)
{
    spdlog::get(loggerName)->flush();
}

void registerCallback(shared_ptr<helpers::logger> logger, helpers::LogCallback callback)
{
    logger->sinks().push_back(make_shared<CallbackSinkMt>(callback));
}

}

string getDefaultLoggerName() {
   return TCHAR_TO_UTF8(*UKismetSystemLibrary::GetGameName());
}

string getDefaultLogFile()
{
    string gameName(TCHAR_TO_UTF8(*UKismetSystemLibrary::GetGameName()));
    // TODO: make this platform-specific
    return "/tmp/"+gameName+".log";
}

void initMainLogger()
{
    logLevel = getenv(LOG_LEVEL_ENV) ? string(getenv(LOG_LEVEL_ENV)) : "";
    logFile = getenv(LOG_FILE_ENV) ? string(getenv(LOG_FILE_ENV)) : getDefaultLogFile();

    if (logFile != "")
        mainLogger = spdlog::basic_logger_mt<spdlog::async_factory>(getDefaultLoggerName(), logFile);
    else
        mainLogger = spdlog::stdout_color_mt(getDefaultLoggerName());

    spdlog::flush_every(chrono::seconds(1));
    spdlog::set_default_logger(mainLogger);
    spdlog::set_pattern(getenv(LOG_FORMAT_ENV) ? getenv(LOG_FORMAT_ENV) : DEFAULT_FORMAT);
    initLogger(mainLogger);
}

void initLogger(shared_ptr<helpers::logger> logger)
{
    logger->flush_on(spdlog::level::err);
    if (logLevel == "")
#ifdef UE_BUILD_DEBUG
        logLevel = "trace";
        logger->flush_on(spdlog::level::debug);
#else
        logLevel = "info";
        logger->flush_on(spdlog::level::warn);
#endif

    logger->set_level(spdlog::level::from_str(logLevel));
    logger->info("Initialized logger {}: level {} file {}",  logger->name(),
        spdlog::level::to_short_c_str(logger->level()), logFile);
    logger->flush();
    
    // register UE_LOG sink by default
    registerCallback(logger, [](const string& msg, const helpers::log_level& lvl){
        switch (lvl) {
            case helpers::log_level::warn:
                UE_LOG(LogTemp, Warning, TEXT("%s"), msg.c_str());
                break;
            case helpers::log_level::err:
                UE_LOG(LogTemp, Error, TEXT("%s"), msg.c_str());
                break;
            case helpers::log_level::info:
                UE_LOG(LogTemp, Display, TEXT("%s"), msg.c_str());
                break;
            case helpers::log_level::debug:
                UE_LOG(LogTemp, Log, TEXT("%s"), msg.c_str());
                break;
            case helpers::log_level::trace: // fall through
            default:
                UE_LOG(LogTemp, Verbose, TEXT("%s"), msg.c_str());
                break;
        }
    });
}
