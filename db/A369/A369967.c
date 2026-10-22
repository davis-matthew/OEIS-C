/*
 * A369967  a(n) = 1 if the arithmetic derivative of n is a multiple of 5,
 *          otherwise 0.
 * https://oeis.org/A369967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative of n is a multiple of 5,
 *            otherwise 0
 * Range:     u8 n <= 256, u16 n <= 9725, u32 n <= 9725, u64 n <= 9725,
 *            u128 n <= 9725, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the arithmetic derivative via the formula n' = Σ_{p|n} (n * e_p / p)   */
/* where n = ∏ p^{e_p}.  It then tests divisibility by 5. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n = 0 or 1  -> derivative 0, divisible by 5 */
    if (n <= 1)
        return 1;

    idx_t temp = n;
    seq_t N = (seq_t)n;   /* base value of n */
    seq_t der = 0;        /* running sum of partial derivatives */

    /* factor 2 separately to allow odd increment */
    if (temp % 2 == 0) {
        unsigned e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        /* partial = (N / 2) * e */
        der += (N / 2) * e;
    }

    /* trial division by odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* partial = (N / p) * e */
            der += (N / p) * e;
        }
    }

    /* if remainder is a prime > sqrt(temp) */
    if (temp > 1) {
        /* e = 1 */
        der += N / temp;
    }

    /* check divisibility by 5 */
    return (der % 5 == 0) ? 1 : 0;
}
