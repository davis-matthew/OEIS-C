/*
 * A384548  Consecutive states of the linear congruential pseudo-random number
 *          generator 2304580733*s mod 2^32 when started at s=1.
 * https://oeis.org/A384548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2304580733 * a(n-1) mod 2^32, with a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* multiplier modulo 2^32 */
    const unsigned int mult32 = 2304580733u;
    if (n <= 1)
        return (seq_t)1;           /* a(1) = 1, a(0) is undefined but harmless */

    seq_t val = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        /* get current value in 0..2^32-1 */
        uint32_t v32 = (uint32_t)val;
        /* multiply using 64‑bit to avoid overflow, then keep low 32 bits */
        uint32_t prod32 = (uint32_t)((uint64_t)mult32 * v32);
        val = (seq_t)prod32;      /* cast back to seq_t */
    }
    return val;
}
