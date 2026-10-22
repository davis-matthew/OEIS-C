/*
 * A348281  Sum of the arithmetic derivatives of the squarefree divisors of n:
 *          a(n) = Sum_{d|n} d' * mu(d)^2.
 * https://oeis.org/A348281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p|n} Π_{q|n, q≠p} (1 + q) (sum over the distinct prime
 *            divisors of n. For n = 1 the value is 0
 * Range:     u8 n <= 209, u16 n <= 8641, u32 n <= 8641, u64 n <= 8641,
 *            u128 n <= 8641, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* indices start at 1; for n < 1 return 0 */
    if (n < 1)
        return 0;

    /* factor n into distinct primes */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    size_t pcount = 0;

    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            primes[pcount++] = p;
            while (nn % p == 0)
                nn /= p;
        }
    }
    if (nn > 1)
        primes[pcount++] = nn;

    /* n = 1 has no prime divisors, return 0 */
    if (pcount == 0)
        return 0;

    /* compute product Π (1 + p) over all distinct primes */
    seq_t prod = 1;
    for (size_t i = 0; i < pcount; ++i)
        prod *= (seq_t)(primes[i] + 1);

    /* sum prod / (1 + p) over all primes */
    seq_t sum = 0;
    for (size_t i = 0; i < pcount; ++i)
        sum += prod / (seq_t)(primes[i] + 1);

    return sum;
}
