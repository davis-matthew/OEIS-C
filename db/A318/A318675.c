/*
 * A318675  Sum of squarefree divisors of n that have an odd number of prime
 *          factors.
 * https://oeis.org/A318675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, squarefree, Ω(d) odd} d * where Ω(d) is the
 *            number of prime factors of d counted with multiplicity. * This
 *            function uses a simple trial‑division factorisation and then *
 *            accumulates the sums of squarefree divisors with odd cardinality
 *            * of distinct prime factors
 * Range:     u8 n <= 209, u16 n <= 9908, u32 n <= 9908, u64 n <= 9908,
 *            u128 n <= 9908, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 */
    if (n == 1) return (seq_t)0;

    /* Convert the index to a native integer for factorisation */
    uint64_t t = (uint64_t)n;

    /* Array of distinct prime factors of n – 64 is more than enough for
     * any 64‑bit integer (the product of the first 64 primes exceeds 2^64). */
    uint64_t primes[64];
    size_t    m = 0;

    /* Trial division by 2 */
    if ((t & 1ULL) == 0) {
        primes[m++] = 2ULL;
        while ((t & 1ULL) == 0) t >>= 1;
    }

    /* Trial division by odd numbers */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            primes[m++] = p;
            while (t % p == 0) t /= p;
        }
    }

    /* If remainder > 1, it is a prime factor > sqrt(original n). */
    if (t > 1 && m < 64) primes[m++] = t;

    /* Compute the sum of products of odd‑sized subsets of distinct primes.
     * start with the empty subset (product 1, even size). */
    seq_t even = (seq_t)1;   /* sum of products of even‑size subsets */
    seq_t odd  = (seq_t)0;   /* sum of products of odd‑size subsets */

    for (size_t i = 0; i < m; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t new_even = even + odd * p;
        seq_t new_odd  = odd + even * p;
        even = new_even;
        odd  = new_odd;
    }

    return odd;
}
