/*
 * A257531  If 2^(n-1) mod n = 1, then 1 else 0.
 * https://oeis.org/A257531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if 2^(n-1) mod n = 1, otherwise 0
 * Range:     u8 n <= 9868, u16 n <= 9868, u32 n <= 9868, u64 n <= 9868,
 *            u128 n <= 9868, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* 2^(n-1) mod n */
    seq_t mod = (seq_t)n;
    seq_t result = mod_pow((seq_t)2, (uint64_t)(n - 1), mod);

    /* compare with 1 */
    return (result == (seq_t)1) ? (seq_t)1 : (seq_t)0;
}
