#ifndef MODBUSMANAGER_H
    #define MODBUSMANAGER_H

    #include "config.h"

    #include <ModbusBridgeWiFi.h>
    #include <ModbusClientRTU.h>

class ModbusManager{
    private:
        ModbusClientRTU *_MBclient;
        ModbusBridgeWiFi *_MBbridge;
        Config *_config;
    public:
        ModbusManager();
        void start(Config *config);
};


#endif /* MODBUSMANAGER_H */
