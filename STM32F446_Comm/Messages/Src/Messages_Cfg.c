#include "Messages_Cfg.h"
#include "types.h"

Message_Cfg_t MessageCfg[NUMBER_OF_MESSAGES] =
{
		{{.identifier = 0x00123456, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 8, .Data = {1, 2, 3, 4, 5, 6, 7, 8        }}, .time = 1000},
		{{.identifier = 0x03333333, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 6, .Data = {6, 7, 8, 9, 10 , 11           }}, .time = 2500 },
		{{.identifier = 0x02222222, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 7, .Data = {0 ,8 ,7 ,5 ,4 ,7 ,9           }}, .time = 3500  },
		{{.identifier = 0x18AEEAF1, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 5, .Data = {92, 51, 45, 77, 44            }}, .time = 2000 },
		{{.identifier = 0x18A0AFE4, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 8, .Data = {71, 12, 15, 43, 71, 51, 37, 54}}, .time = 3000  },
		{{.identifier = 0x18A0AFE4, .remoteTransmissionRequest = 0, .identifierExtension = 1, .DLC = 8, .Data = {71, 12, 15, 43, 71, 51, 37, 54}}, .time = 4000  },
};
