/*
 * A360459  Two times the median of the multiset of prime factors of n; a(1) =
 *          2.
 * https://oeis.org/A360459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * median of prime factors of n (with multiplicity)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1, a(1) = 2 by definition. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle offset case */
    if (n == 1)
        return (seq_t)2;

    /* Factor n into primes (with multiplicity). */
    uint64_t m = (uint64_t)n;          /* Use uint64_t for internal arithmetic */
    seq_t factors[64];                 /* Maximum 63 prime factors for n < 2^64 */
    size_t cnt = 0;

    /* Factor out powers of 2 */
    while ((m & 1ULL) == 0) {          /* m % 2 == 0 */
        factors[cnt++] = (seq_t)2;
        m >>= 1;                      /* m /= 2 */
    }

    /* Odd prime factors up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) {
            factors[cnt++] = (seq_t)p;
            m /= p;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1)
        factors[cnt++] = (seq_t)m;

    /* Compute 2 * median (sum of two middles for even length) */
    if (cnt & 1) {                  /* odd number of factors */
        seq_t mid = factors[cnt / 2];
        return mid * 2;
    } else {                        /* even number of factors */
        return factors[cnt / 2 - 1] + factors[cnt / 2];
    }
}
