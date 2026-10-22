/*
 * A375670  The maximum exponent in the prime factorization of the largest
 *          5-rough divisor of n.
 * https://oeis.org/A375670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A375670: 
   a(n) = maximum exponent among prime factors ≥ 5 of the largest 5‑rough divisor of n.
   (The 5‑rough part of n is obtained by removing all 2's and 3's from n.)
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Work on an unsigned 64‑bit value – the input index is also uint64_t. */
    uint64_t m = (uint64_t)n;

    /* Remove all factors of 2 and 3 – they do not belong to any 5‑rough divisor. */
    while ((m & 1ULL) == 0)   /* m even */
        m >>= 1;
    while (m % 3ULL == 0)
        m /= 3ULL;

    seq_t max_exp = 0;

    /* If nothing remains after stripping 2 and 3, the answer is 0. */
    if (m == 1ULL)
        return 0;

    /* Trial‑division factorisation of the remaining part. */
    uint64_t p;
    for (p = 5; p <= m / p; ++p)       /* avoid overflow in p*p <= m */
    {
        /* Skip numbers divisible by 2 or 3; they cannot be primes ≥ 5. */
        if ((p & 1ULL) == 0 || p % 3ULL == 0)
            continue;

        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t se = (seq_t)exp;
            if (se > max_exp)
                max_exp = se;
        }
    }

    /* If a prime factor > sqrt(original m) remains, it occurs once. */
    if (m > 1ULL) {
        /* m itself is a prime ≥ 5, exponent = 1. */
        if ((seq_t)1 > max_exp)
            max_exp = 1;
    }

    return max_exp;
}
