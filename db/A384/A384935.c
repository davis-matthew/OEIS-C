/*
 * A384935  Consecutive states of the linear congruential pseudo-random number
 *          generator 513*s mod (2^31-1) when started at s=1.
 * https://oeis.org/A384935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 513 * a(n-1) mod (2^31-1); a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    const seq_t MOD   = (seq_t)0x7FFFFFFFu;   /* 2^31 - 1 */
    const seq_t MULT  = (seq_t)513u;         /* the multiplier */

    if (n <= 1)
        return n == 1 ? (seq_t)1 : (seq_t)0;  /* fallback for n = 0 */

    /* a(n) = MULT^(n-1) mod MOD, with a(1)=1 */
    return mod_pow(MULT, n-1, MOD);
}
