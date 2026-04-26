#pragma once

enum BridgeMode { LOCAL, REMOTE };

void InitBridge(enum BridgeMode BridgeMode);
void ClientMainloop();
void CloseBridge();