#include "modbusmanager.h"

ModbusManager::ModbusManager()
    :_MBclient(NULL)
    ,_MBbridge(NULL)
    ,_config(NULL)
{}

void ModbusManager::start(Config *config)
{
    _config = config;
    // MBUlogLvl = LOG_LEVEL_WARNING; Is it used?
    RTUutils::prepareHardwareSerial(modbusSerial);

#if defined(RX_PIN) && defined(TX_PIN)
    // use rx and tx-pins if defined in platformio.ini
    modbusSerial.begin(_config->getModbusBaudRate(), _config->getModbusConfig(), RX_PIN, TX_PIN);
    dbgln("Use user defined RX/TX pins");
#else
    // otherwise use default pins for hardware-serial2
    modbusSerial.begin(_config->getModbusBaudRate(), _config->getModbusConfig());
    dbgln("Default RX/TX pins");
#endif

  // Mode 0: RTU client -> TCP Master
  if (_config->getBridgeMode() == 0) {
    _MBclient = new ModbusClientRTU(_config->getModbusRtsPin());
    _MBclient->setTimeout(1000);
    _MBclient->begin(modbusSerial, 1);

    _MBbridge = new ModbusBridgeWiFi();
    for (uint8_t i = 1; i < 248; i++)
    {
      _MBbridge->attachServer(i, i, ANY_FUNCTION_CODE, _MBclient);
    }
    _MBbridge->start(_config->getTcpPort(), 10, _config->getTcpTimeout());
  }
  // Mode 1: TCP client -> RTU Master
  else
  {
    _MBclient = new ModbusClientRTU(_config->getModbusRtsPin());
    _MBclient->setTimeout(1000);
    _MBclient->begin(modbusSerial, 1);

    _MBbridge = new ModbusBridgeWiFi();
    for (uint8_t i = 1; i < 248; i++)
    {
      _MBbridge->attachServer(i, i, ANY_FUNCTION_CODE, _MBclient);
    }
    _MBbridge->start(_config->getTcpPort(), 10, _config->getTcpTimeout());
  }

}
