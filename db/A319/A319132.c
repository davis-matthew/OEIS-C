/*
 * A319132  a(n) = Sum_{d|n} Sum_{j|d} mu(j)^2*j, where mu = Möbius function
 *          (A008683).
 * https://oeis.org/A319132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = \prod_{p^e||n} ((p+1)*e + 1)
 * Range:     u8 n <= 65, u16 n <= 9613, u32 n <= 9613, u64 n <= 9613,
 *            u128 n <= 9613, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    uint64_t tmp = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Handle factor 2 */
    uint64_t exp = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        exp++;
    }
    if (exp) {
        seq_t term = ((seq_t)2 + (seq_t)1) * (seq_t)exp + (seq_t)1;
        result *= term;
    }

    /* Handle odd factors */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            seq_t term = ((seq_t)p + (seq_t)1) * (seq_t)exp + (seq_t)1;
            result *= term;
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (tmp > 1) {
        seq_t term = ((seq_t)tmp + (seq_t)1) * (seq_t)1 + (seq_t)1;
        result *= term;
    }

    return result;
}
