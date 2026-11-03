/*
 * A320465  a(n) = 2^n - (2^(n-1) mod n), where "mod" is the nonnegative
 *          remainder operator.
 * https://oeis.org/A320465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - (2^(n-1) mod n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 863 (2026-11-01)
 * Helpers:   pow2, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t pown = pow2(n);                      /* 2^n            */
    seq_t rem  = powmod((seq_t)2, n-1, (seq_t)n);   /* 2^(n-1) mod n */
    return pown - rem;
}
