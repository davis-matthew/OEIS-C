/*
 * A353898  a(n) is the number of divisors of n whose exponents in their prime
 *          factorizations are all powers of 2 (A138302).
 * https://oeis.org/A353898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e||n} (floor(log2(e)) + 2) (multiplicative: a(p^e) =
 *            ⌊log2(e)⌋ + 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return a(n) */
seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1) */
    if (n == 1) return (seq_t)1;

    uint64_t copy = (uint64_t)n;   /* work on a copy of n */
    seq_t prod = (seq_t)1;

    /* factor n by trial division */
    for (uint64_t p = 2; p <= copy / p; ++p) {
        if (copy % p == 0) {
            uint64_t e = 0;
            while (copy % p == 0) {
                copy /= p;
                ++e;
            }
            /* compute floor(log2(e)) + 2 */
            uint64_t t = e;
            int log2e = 0;
            /* e >= 1, so t > 0 */
            while (t > 1) {
                t >>= 1;
                ++log2e;
            }
            prod *= (seq_t)(log2e + 2);
        }
    }

    /* if a prime factor > sqrt(original n) remains */
    if (copy > 1) {
        /* exponent of remaining prime is 1 */
        prod *= (seq_t)2;   /* floor(log2(1))=0 + 2 */
    }

    return prod;
}
