/*
 * A257531  If 2^(n-1) mod n = 1, then 1 else 0.
 * https://oeis.org/A257531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if 2^(n-1) mod n = 1, otherwise 0
 * Range:     u8 n <= 68101, u16 n <= 68100, u32 n <= 68101, u64 n <= 68101,
 *            u128 n <= 68101, big any n
 * Validated: standard, pass, 120 terms through n = 9868 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    /* 2^(n-1) mod n */
    seq_t mod = (seq_t)n;
    seq_t result = powmod((seq_t)2, (uint64_t)(n - 1), mod);

    /* compare with 1 */
    return (result == (seq_t)1) ? (seq_t)1 : (seq_t)0;
}
