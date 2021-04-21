//
// Created by lamp on 4/21/21.
//

#ifndef KV_STORAGE_LOG_SETUP_HPP
#define KV_STORAGE_LOG_SETUP_HPP

#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace log_setup {
void init(const std::string&);
}

#endif  // KV_STORAGE_LOG_SETUP_HPP
