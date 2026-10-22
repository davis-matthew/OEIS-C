/*
 * A384675  Consecutive states of the linear congruential pseudo-random number
 *          generator 7^13*s mod 10^11 when started at 1.
 * https://oeis.org/A384675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = 7^13 * a(n-1) mod 10^11 (equivalently a(n) =
 *            96889010407^(n-1) mod 10^11)
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"


seq_t a(idx_t n)
{
    /* 7^13 = 96889010407  and 10^11 = 100000000000 */
    static const seq_t base = 96889010407ULL;
    static const seq_t mod  = 100000000000ULL;

    /* exponent is (n-1); if n==0 the underflow produces a huge
       exponent but the result is irrelevant for offset < 1. */
    idx_t exp = n - 1U;
    return mod_pow(base, (uint64_t)exp, mod);
}
