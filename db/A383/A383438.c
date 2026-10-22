/*
 * A383438  a(n) = Sum_{k=1..n} Product_{p|k, p prime} k/p.
 * https://oeis.org/A383438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Product_{p|k, p prime} k/p (OEIS A383438).
 *            Computation uses a simple sieve to obtain the smallest prime
 *            factor of every number up to n, then sums the required terms. No
 *            static caching is used, so the function works for calls in any
 *            order
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Allocate array of smallest prime factors (spf) up to n. */
    uint64_t *spf = (uint64_t*)calloc((size_t)n + 1, sizeof(uint64_t));
    if (spf == NULL)
        return (seq_t)0;          /* allocation failed; return 0 as safe fallback */

    /* Sieve of Eratosthenes filling spf. */
    for (uint64_t i = 2; i * i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            for (uint64_t j = i * i; j <= n; j += i)
                if (spf[j] == 0)
                    spf[j] = i;
        }
    }
    for (uint64_t i = 2; i <= n; ++i)
        if (spf[i] == 0)
            spf[i] = i;

    seq_t sum = (seq_t)0;

    /* Compute the cumulative sum. */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = (seq_t)1;
        uint64_t t = (uint64_t)k;
        while (t > 1) {
            uint64_t p = spf[t];
            term *= ((seq_t)k / (seq_t)p);
            while (t % p == 0)
                t /= p;      /* skip multiplicities: distinct primes only */
        }
        sum += term;
    }

    free(spf);
    return sum;
}
