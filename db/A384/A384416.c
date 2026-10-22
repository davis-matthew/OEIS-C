/*
 * A384416  Consecutive internal states of the linear congruential
 *          pseudo-random number generator of the HP 48 series calculators
 *          when started at 1.
 * https://oeis.org/A384416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = 2851130928467 * a(n-1) mod 10^15
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

static const seq_t MOD  = (seq_t)1000000000000000ULL; /* 10^15 */
static const seq_t MULT = (seq_t)2851130928467ULL;

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */
    /* a(n) = MULT^(n-1) mod MOD */
    return mod_pow(MULT, n - 1, MOD);
}
