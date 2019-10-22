// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2018-2019 The Cash2 developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "CryptoNoteCore/CoreConfig.h"
#include "Logging/ILogger.h"
#include "P2p/NodeServerConfig.h"
#include "RpcNodeConfiguration.h"

namespace PaymentService {

class ConfigurationManager {
public:
  ConfigurationManager();
  bool init(int argc, char** argv);

  std::string bindAddress;
  uint16_t bindPort;
  std::string containerFile;
  std::string containerPassword;
  CryptoNote::CoreConfig coreConfig;
  bool daemonize;
  bool generateNewContainer;
  size_t logLevel;
  std::string logFile;
  CryptoNote::NodeServerConfig nodeServerConfig;
  bool printAddresses;
  bool registerService;
  RpcNodeConfiguration remoteNodeConfig;
  std::string rpcConfigurationPassword;
  std::string serverRoot;
  std::string spendPrivateKey;
  bool startInprocess;
  bool testnet;
  bool unregisterService;
  std::string viewPrivateKey;
};

} // end namespace PaymentService
