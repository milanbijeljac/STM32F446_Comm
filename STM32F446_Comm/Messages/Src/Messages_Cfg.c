#include "Messages_Cfg.h"
#include "types.h"

Message_Cfg_t MessageCfg[NUMBER_OF_MESSAGES] =
{
		{{.identifier = 0x18AE2FE8, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 8, .Data = {1, 2, 3, 4, 5, 6, 7, 8        }}, .time = 1000},
		{{.identifier = 0x11DEAD12, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 6, .Data = {6, 7, 8, 9, 10 , 11           }}, .time = 900 },
		{{.identifier = 0x10BBAAA3, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 7, .Data = {0 ,8 ,7 ,5 ,4 ,7 ,9           }}, .time = 600  },
		{{.identifier = 0x15AEEAF1, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 5, .Data = {92, 51, 45, 77, 44            }}, .time = 700 },
		{{.identifier = 0x13A0AFE4, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 8, .Data = {71, 12, 15, 43, 71, 51, 37, 54}}, .time = 800  },
};
