/*
 * A384546  Consecutive states of the linear congruential pseudo-random number
 *          generator 1732073221*s mod 2^32 when started at s=1.
 * https://oeis.org/A384546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = 1732073221 * a(n-1) mod 2^32
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for the linear congruential generator */
seq_t a(idx_t n)
{
    /* initial value */
    if (n == 1) return (seq_t)1;

    /* constants */
    const seq_t BASE = (seq_t)1732073221ULL;
    /* mask for 32‑bit modulus (mod = 2^32) */
    const seq_t MASK = (seq_t)0xFFFFFFFFULL;

    /* exponent reduction using period 2^30 */
    const idx_t PERIOD = ((idx_t)1 << 30);
    idx_t exp = (n - 1) % PERIOD;

    /* fast exponentiation modulo 2^32 */
    seq_t result = (seq_t)1;
    seq_t power = BASE;

    while (exp > 0) {
        if (exp & 1) {
            /* multiplication modulo 2^32: keep low 32 bits */
            result = (result * power) & MASK;
        }
        exp >>= 1;
        if (exp) { /* avoid unnecessary square on last iteration */
            power = (power * power) & MASK;
        }
    }

    return result;
}
