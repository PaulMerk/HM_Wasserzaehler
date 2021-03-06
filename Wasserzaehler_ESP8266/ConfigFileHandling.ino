bool loadSysConfig() {
  File configFile = SPIFFS.open("/" + configFilename, "r");
  if (!configFile) {
    DEBUG("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    DEBUG("Config file size is too large");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    DEBUG("Failed to parse config file");
    return false;
  }

#ifdef SERIALDEBUG
  json.printTo(Serial);
#endif
  ((json["ip"]).as<String>()).toCharArray(ip, IPSize);
  ((json["netmask"]).as<String>()).toCharArray(netmask, IPSize);
  ((json["gw"]).as<String>()).toCharArray(gw, IPSize);
  ((json["ccuip"]).as<String>()).toCharArray(ccuip, IPSize);
  ((json["variable"]).as<String>()).toCharArray(variable, variableSize);

  ZaehlerWert = json["ZaehlerWert"];

  return true;
}

bool saveSysConfig() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["ip"] = ip;
  json["netmask"] = netmask;
  json["gw"] = gw;

  json["ccuip"] = ccuip;
  json["variable"] = variable;

  json["ZaehlerWert"] = ZaehlerWert;

  File configFile = SPIFFS.open("/" + configFilename, "w");
  if (!configFile) {
    DEBUG("Failed to open config file for writing");
    return false;
  }
#ifdef SERIALDEBUG
  json.printTo(Serial);
#endif
  json.printTo(configFile);
  return true;
}
