//
// Created by jjangchan on 2022/01/02.
//

#ifndef BOOSTLOGGER_H
#define BOOSTLOGGER_H

#include <memory>
#include <boost/log/common.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/keywords/format.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
//#include <boost/log/sinks/text_file_backend.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

//    BOOST_LOG_TRIVIAL(trace) << "This is a trace severity message";
//    BOOST_LOG_TRIVIAL(debug) << "This is a debug severity message";
//    BOOST_LOG_TRIVIAL(info) << "This is an informational severity message";
//    BOOST_LOG_TRIVIAL(warning) << "This is a warning severity message";
//    BOOST_LOG_TRIVIAL(error) << "This is an error severity message";
//    BOOST_LOG_TRIVIAL(fatal) << "and this is a fatal severity message";

class BoostLogger{
private:
    BoostLogger() {
        sink_console_log = nullptr;
        sink_file_log = nullptr;
        init();
    }
    ~BoostLogger() {
        if(sink_console_log)
            sink_console_log->flush();
        if(sink_file_log)
            sink_file_log->flush();
        logging::core::get()->remove_all_sinks();
    }
    BoostLogger(const BoostLogger &) = delete;
    BoostLogger &operator=(const BoostLogger &) = delete;
private:
    boost::shared_ptr<sinks::synchronous_sink<sinks::text_ostream_backend>> sink_console_log;
    boost::shared_ptr<sinks::synchronous_sink<sinks::text_file_backend>> sink_file_log;
private:
    void init() {
        sink_console_log = logging::add_console_log(
                std::cout,
                keywords::auto_flush = true,
                boost::log::keywords::format = ">> %Message%");
        sink_file_log = logging::add_file_log(
                keywords::target = "logs/", keywords::file_name = "%Y%m%d_%3N.log",
                keywords::rotation_size = 10 * 1024 * 1024, // rolling with every 10MB
                keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                keywords::auto_flush = true,
                keywords::scan_method = sinks::file::scan_matching,
                keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%]: %Message%");

        logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);

//        logger::add_common_attributes();

//        boost::filesystem::path::imbue(std::locale("C"));
//        boost::shared_ptr<logging::core> core = logging::core::get();
//
//        boost::shared_ptr<sinks::text_file_backend> backend =
//                boost::make_shared<sinks::text_file_backend>(
//                        keywords::target = "logs/",
//                        keywords::file_name = "%Y%M%D_%3N.log",
//                        keywords::rotation_size = 5 * 1024 * 1024,
//                        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
//                        keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%]: %Message%"
//                        );
//        typedef sinks::synchronous_sink<sinks::text_file_backend> sink_t;
//        boost::shared_ptr<sink_t> sink(new sink_t(backend));
//        core->add_sink(sink);
    }
public:
    inline static BoostLogger &get_instance() {
        static BoostLogger instance;
        return instance;
    }
    void set_filter(const logging::trivial::severity_level &level) {
        logging::core::get()->set_filter(logging::trivial::severity >= level);
    }
private:
};
#endif //SEGMENT_MEASUREMENT_SEARCH_BOOSTLOGGER_H
