/*
 * A299090  Number of "digits" in the binary representation of the multiset of
 *          prime factors of n.
 * https://oeis.org/A299090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ⌊log₂(max exponent in prime factorization of n)⌋ + 1
 *            (a(1)=0). That is, the number of digits in the binary
 *            representation of the multiset of prime factors of n
 * Range:     u8 n <= 9533, u16 n <= 9533, u32 n <= 9533, u64 n <= 9533,
 *            u128 n <= 9533, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= (idx_t)1)            /* n == 1 */
        return (seq_t)0;

    uint64_t m = (uint64_t)n;     /* work on a 64‑bit copy of n */
    uint64_t maxexp = 0;
    uint64_t exp, p;

    /* factor 2 separately */
    if ((m & 1ULL) == 0) {
        exp = 0;
        do {
            m >>= 1;
            exp++;
        } while ((m & 1ULL) == 0);
        if (exp > maxexp)
            maxexp = exp;
    }

    /* odd prime factors, trial division up to sqrt(m) */
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp > maxexp)
                maxexp = exp;
        }
    }

    /* if anything left, it's a prime with exponent 1 */
    if (m > 1 && maxexp < 1)
        maxexp = 1;

    /* number of binary digits = floor(log2(maxexp)) + 1 */
    seq_t digits = 0;
    while (maxexp > 0) {
        digits++;      /* each shift removes one binary digit */
        maxexp >>= 1;
    }
    return digits;
}
