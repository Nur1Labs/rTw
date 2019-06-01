// Copyright (c) 2010-2019 AriA developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ARIA_ECCRYPTOVERIFY_H
#define ARIA_ECCRYPTOVERIFY_H

#include <cstdlib>
#include <vector>

class uint256;

namespace eccrypto
{
bool Check(const unsigned char* vch);
bool CheckSignatureElement(const unsigned char* vch, int len, bool half);

} // eccrypto namespace

#endif // ARIA_ECCRYPTOVERIFY_H
