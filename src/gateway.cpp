//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is a template for a cpp program
#include "gateway.hpp" //ClientGateway

#include "code_utils.hpp"
//------------------------------------------------------------------------------
namespace ilrd {

ClientGateway::ClientGateway() : RequestEngineGateway(-1), m_socket(-1)
{}

ClientGateway::~ClientGateway()
{}
std::unique_ptr<RequestEngineRequest<std::string, CommandParams> >
ClientGateway::create()
{}
} // namespace ilrd
//------------------------------------------------------------------------------
