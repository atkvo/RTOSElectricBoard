#include "ElectricBoardProtocol.hpp"

ElectricBoardCommand ParsePayload(mesh_packet_t * packet)
{
    ElectricBoardCommand cmd;
    switch (packet->data[0]) {
        case UNDEFINED:
            cmd.cmdType = UNDEFINED;
            cmd.arg = packet->data[1];
            break;
        case SET_DRIVE_POWER:
            cmd.cmdType = SET_DRIVE_POWER;
            cmd.arg = packet->data[1];
            break;
        case GET_DRIVE_POWER:
            cmd.cmdType = GET_DRIVE_POWER;
            cmd.arg = packet->data[1];
            break;
    }

    return cmd;
}

bool InsertCommand(ElectricBoardCommand cmd, char * buf, uint8_t bufSize)
{
    if (bufSize < 3)
    {
        return false;
    }

    switch(cmd.cmdType)
    {
        case SET_DRIVE_POWER:
            buf[0] = SET_DRIVE_POWER;
            buf[1] = (uint8_t)cmd.arg;
            break;
        case GET_DRIVE_POWER:
            buf[0] = GET_DRIVE_POWER;
            buf[1] = (uint8_t)cmd.arg;
            break;
        default:
            buf[0] = UNDEFINED;
            buf[1] = (uint8_t)cmd.arg;
            break;
    }

    return cmd.cmdType  == UNDEFINED ? false : true;
}