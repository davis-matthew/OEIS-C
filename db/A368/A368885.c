/*
 * A368885  The number of unitary divisors of n that are squares of a
 *          squarefree number (A062503).
 * https://oeis.org/A368885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over all prime powers p^e || n of (e==2 ? 2 : 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    uint64_t m = (uint64_t)n;
    size_t   k = 0;                       /* number of distinct primes with exponent exactly 2 */

    /* factor out the power of 2 first */
    if ((m & 1U) == 0) {
        size_t e = 0;
        while ((m & 1U) == 0) {
            m >>= 1;
            ++e;
        }
        if (e == 2) ++k;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e == 2) ++k;
        }
    }

    /* remaining factor is a prime > sqrt(original n) with exponent 1 */
    /* it does not contribute, so we ignore it */

    /* compute 2^k */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < k; ++i) {
        result *= (seq_t)2;
    }
    return result;
}
