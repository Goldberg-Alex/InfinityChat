//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-			gateway.hpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

#ifndef ILRD_GATEWAY_HPP
#define ILRD_GATEWAY_HPP

#include "command.hpp"    //CommandParams
#include "req_engine.hpp" //Gateway
#include "socket.hpp"     //Socket

namespace ilrd {
class ClientGateway : public RequestEngineGateway<std::string, CommandParams> {
public:
    ClientGateway();

    ~ClientGateway();

     std::unique_ptr<RequestEngineRequest<std::string, CommandParams> >
    create() override;

private:
    Socket m_socket;
};

} // namespace ilrd
#endif // ILRD_GATEWAY_HPP
