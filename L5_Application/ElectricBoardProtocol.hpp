#ifndef ELECTRICBOARD_PROTOCOL_HPP_
#define ELECTRICBOARD_PROTOCOL_HPP_

/* CMD PROTOCOL FORMAT: [CommandType][CommandArg] */
#include "wireless.h"

enum CommandType {
    UNDEFINED = 0,
    SET_DRIVE_POWER = 1,
    GET_DRIVE_POWER = 2
};

struct ElectricBoardCommand
{
    CommandType cmdType;
    uint8_t arg; 
};

ElectricBoardCommand ParsePayload(mesh_packet_t * packet);
bool InsertCommand(ElectricBoardCommand cmd, char * buf, uint8_t bufSize);

#endif /* ELECTRICBOARD_PROTOCOL_HPP_ */