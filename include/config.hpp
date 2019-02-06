//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-			config.hpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

#ifndef ILRD_CONFIG_HPP
#define ILRD_CONFIG_HPP

namespace ilrd {

// common definitions and declarations for the project

// the port number in the server to listen for incoming connections
// and also the port number to connect to in the client
const char TCP_LISTEN_PORT[] = "10000";

// number of allowed pending requests to the tcp socket we're listening to
const int TCP_BACKLOG_SIZE = 100;

// Logger logging level. DEBUG > INFO > ERROR. individual logs lower than the
// log level will not be written
enum log_level { ERROR = 0, INFO = 1, DEBUG = 2 };
const enum log_level LOG_LEVEL = INFO;

const char LOG_FILENAME[] = "log.txt";
} // namespace ilrd
#endif // ILRD_CONFIG_HPP
