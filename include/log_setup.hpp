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

const size_t max_random_family_length = 1000;
const size_t random_string_length = 15;
const size_t max_families = 100;
const size_t key_width = 3;


namespace log_setup {
void init(const std::string&);
}

#endif  // KV_STORAGE_LOG_SETUP_HPP
