// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2018-2021 The Cash2 developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "Common/int-util.h"
#include "crypto/hash.h"
#include "CryptoNoteConfig.h"
#include "Difficulty.h"

namespace CryptoNote {

  using std::uint64_t;
  using std::vector;

#if defined(__SIZEOF_INT128__)

  static inline void mul(uint64_t a, uint64_t b, uint64_t &low, uint64_t &high) {
    typedef unsigned __int128 uint128_t;
    uint128_t res = (uint128_t) a * (uint128_t) b;
    low = (uint64_t) res;
    high = (uint64_t) (res >> 64);
  }

#else

  static inline void mul(uint64_t a, uint64_t b, uint64_t &low, uint64_t &high) {
    low = mul128(a, b, &high);
  }

#endif

  static inline bool cadd(uint64_t a, uint64_t b) {
    return a + b < a;
  }

  static inline bool cadc(uint64_t a, uint64_t b, bool c) {
    return a + b < a || (c && a + b == (uint64_t) -1);
  }

  bool check_hash1(const Crypto::Hash &hash, difficulty_type difficulty) {
    uint64_t low, high, top, cur;
    // First check the highest word, this will most likely fail for a random hash.
    mul(swap64be(((const uint64_t *) &hash)[0]), difficulty, top, high);
    if (high != 0) {
      return false;
    }
    mul(swap64be(((const uint64_t *) &hash)[3]), difficulty, low, cur);
    mul(swap64be(((const uint64_t *) &hash)[2]), difficulty, low, high);
    bool carry = cadd(cur, low);
    cur = high;
    mul(swap64be(((const uint64_t *) &hash)[1]), difficulty, low, high);
    carry = cadc(cur, low, carry);
    carry = cadc(high, top, carry);
    return !carry;
  }

  bool check_hash2(const Crypto::Hash &hash, difficulty_type difficulty) {

    // GOAL : remove the first 10 zero characters from the hash
    
    // STEP 1 : make sure the first 10 characters of the hash are zeros 
    for (uint8_t i = 0; i < 5; i++)
    {
      if (hash.data[i] != 0)
      {
        return false;
      }
    }  

    // remove the first 10 zeros of hash to make hashTruncated

    Crypto::Hash hashTruncated = 
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00
    };

    uint8_t j = 0;
    for (uint8_t i = 5; i < 32; i++)
    {
      hashTruncated.data[j] = hash.data[i];
      j++;
    }

    uint64_t low, high, top, cur;
    // First check the highest word, this will most likely fail for a random hash.
    mul(swap64be(((const uint64_t *) &hashTruncated)[0]), difficulty, top, high);
    if (high != 0) {
      return false;
    }
    mul(swap64be(((const uint64_t *) &hashTruncated)[3]), difficulty, low, cur);
    mul(swap64be(((const uint64_t *) &hashTruncated)[2]), difficulty, low, high);
    bool carry = cadd(cur, low);
    cur = high;
    mul(swap64be(((const uint64_t *) &hashTruncated)[1]), difficulty, low, high);
    carry = cadc(cur, low, carry);
    carry = cadc(high, top, carry);
    return !carry;
  }
}
