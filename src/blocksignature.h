// Copyright (c) 2010-2019 AriA developers

#ifndef ARIA_BLOCKSIGNATURE_H
#define ARIA_BLOCKSIGNATURE_H

#include "key.h"
#include "primitives/block.h"
#include "keystore.h"

bool SignBlockWithKey(CBlock& block, const CKey& key);
bool SignBlock(CBlock& block, const CKeyStore& keystore);
bool CheckBlockSignature(const CBlock& block);

#endif //ARIA_BLOCKSIGNATURE_H
