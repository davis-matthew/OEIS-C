/*
 * A382535  Consecutive states of Lehmer's original linear congruential
 *          pseudo-random number generator 23*s mod (10^8+1) when started at
 *          s=1.
 * https://oeis.org/A382535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=23*a(n-1) mod 100000001 */
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    const seq_t MOD  = (seq_t)100000001ULL;
    const seq_t BASE = (seq_t)23ULL;
    return mod_pow(BASE, (uint64_t)(n - 1), MOD);
}
