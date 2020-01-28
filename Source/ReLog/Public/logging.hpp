//
// logging.hpp
//
//  Created by Peter Gusev on 27 January 2020.
//  Copyright 2013-2019 Regents of the University of California
//

#ifndef __logging_hpp__
#define __logging_hpp__

#ifdef UE_BUILD_DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_WARN
#endif

#include <spdlog/spdlog.h>

#define RLOG_TRACE SPDLOG_TRACE
#define RLOG_DEBUG SPDLOG_DEBUG
#define RLOG_INFO SPDLOG_INFO
#define RLOG_WARN SPDLOG_WARN
#define RLOG_ERROR SPDLOG_ERROR

#define RLOG_LOGGER_TRACE SPDLOG_LOGGER_TRACE
#define RLOG_LOGGER_DEBUG SPDLOG_LOGGER_DEBUG
#define RLOG_LOGGER_INFO SPDLOG_LOGGER_INFO
#define RLOG_LOGGER_WARN SPDLOG_LOGGER_WARN
#define RLOG_LOGGER_ERROR SPDLOG_LOGGER_ERROR
#define RLOG_LOGGER_CRITICAL SPDLOG_LOGGER_CRITICAL

#define RLOG_TRACE_TAG(tag, ...) (RLOG_TRACE(tag##__VA_ARGS__))

namespace relog {
    namespace helpers {
        typedef spdlog::logger logger;
        typedef spdlog::level::level_enum log_level;
        typedef std::function<void(const std::string&, const log_level&)> LogCallback;
    }

    void newLogger(std::string loggerName);
    std::shared_ptr<helpers::logger> getLogger(std::string loggerName);
    void flushLogger(std::string loggerName);
    void registerCallback(std::shared_ptr<helpers::logger>, helpers::LogCallback);
}

#endif
