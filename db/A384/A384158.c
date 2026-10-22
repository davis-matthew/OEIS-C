/*
 * A384158  Consecutive states of the linear congruential pseudo-random number
 *          generator for 16-bit WATFOR/WATFIV when started at 1.
 * https://oeis.org/A384158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n)=253*a(n-1) mod 2^15 (the modulus is 32768)
 * Range:     u8 n <= 2, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n<1, return 0 */
    const uint64_t mod   = 32768ULL;      /* 2^15 */
    const uint64_t mult  = 253ULL;        /* multiplier */
    uint64_t out   = 1;                   /* 253^0 */
    uint64_t base  = mult;
    idx_t exp = n - 1;
    while (exp > 0) {
        if (exp & 1ULL)
            out = (out * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return (seq_t)out;
}
