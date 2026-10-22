/*
 * A318305  a(n) = Product_{primes p dividing n} p - Product_{primes p
 *          dividing n} (p-1).
 * https://oeis.org/A318305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{prime p | n} p – Product_{prime p | n} (p-1)
 * Range:     u8 n <= 389, u16 n <= 8517, u32 n <= 8517, u64 n <= 8517,
 *            u128 n <= 8517, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is defined as 0: the product over an empty set is 1,
       hence 1-1 = 0.  For any other n we factor and compute. */
    if (n == 1)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;          /* local copy for factoring */
    seq_t rad  = (seq_t)1;             /* product of distinct primes */
    seq_t prod = (seq_t)1;             /* product of (p-1) values */

    /* handle factor 2 separately to keep the loop over odd numbers */
    if ((m & 1ULL) == 0) {
        rad  *= (seq_t)2;
        prod *= (seq_t)1;              /* 2-1 = 1 */
        while ((m & 1ULL) == 0)
            m >>= 1;
    }

    /* trial division by odd candidates up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            rad  *= (seq_t)p;
            prod *= (seq_t)(p - 1);
            while (m % p == 0)
                m /= p;
        }
    }

    /* if anything remains it's a prime factor > sqrt(original n) */
    if (m > 1) {
        rad  *= (seq_t)m;
        prod *= (seq_t)(m - 1);
    }

    return rad - prod;
}
