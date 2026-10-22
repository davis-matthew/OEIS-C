/*
 * A398302  In the canonical representation of n as a product of powers of
 *          prime numbers, a(n) is the position of the last prime with an
 *          exponent greater than 1, or 0 if n is squarefree.
 * https://oeis.org/A398302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the last prime factor of n with exponent > 1
 *            * in its canonical factorization, or 0 if n is squarefree
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety for indices below the offset */

    uint64_t tmp = (uint64_t)n;   /* work on a 64‑bit copy */
    idx_t    pos  = 0;            /* 1‑based position of the current distinct prime */
    idx_t    last = 0;            /* position of the last prime with exp > 1 */

    /* Trial division up to sqrt(tmp).  Avoid overflow by comparing p <= tmp/p. */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++pos;                /* new distinct prime encountered */

            idx_t exp = 0;
            do {
                tmp /= p;
                ++exp;
            } while (tmp % p == 0);

            if (exp > 1) {
                last = pos;       /* remember this prime as the new candidate */
            }
        }
    }

    /* If a prime factor larger than sqrt(tmp) remains, it is a single exponent. */
    if (tmp > 1) {
        ++pos;                     /* account for the final distinct prime */
        /* exp is 1, so 'last' stays unchanged */
    }

    return (seq_t)last;
}
