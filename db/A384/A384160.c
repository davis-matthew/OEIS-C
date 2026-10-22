/*
 * A384160  Consecutive states of the linear congruential pseudo-random number
 *          generator for 36-bit WATFOR/WATFIV when started at 1.
 * https://oeis.org/A384160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 131069 * a(n-1) mod 2^35, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The multiplier and modulus are constants */
    const uint64_t MUL  = 131069ULL;
    const uint64_t MOD  = ((uint64_t)1 << 35) - 1U;  /* 2^35 - 1 */

    /* a(1) = 1 as specified */
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t val = (seq_t)1;   /* current term */
    for (idx_t i = 1; i < n; ++i) {
        /* product fits in 64 bits: 2^35 * 2^18 < 2^53 */
        uint64_t prod = (uint64_t)val * MUL;
        prod &= MOD;                    /* take lower 35 bits */
        val = (seq_t)prod;               /* store back in seq_t */
    }
    return val;
}
