/*
 * A331136  a(n) = Sum_{primes p < n} r_2(n-p)/4, where r_2(n) = A004018(n).
 * https://oeis.org/A331136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{primes p < n} r_2(n-p)/4, where r_2(m) = A004018(m)
 * Range:     u8 n <= 1196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n <= 1)
        return 0;

    /* -------------------------------------------------------------- */
    /* 1. compute diff[m] = d1(m) - d3(m) for 1 <= m < n               */
    /* -------------------------------------------------------------- */
    seq_t *diff = (seq_t *)calloc((size_t)n, sizeof(seq_t));
    if (!diff)
        return 0; /* allocation failure */

    for (idx_t d = 1; d < n; ++d)
    {
        if ((d & 1ULL) == 0)          /* only odd divisors matter */
            continue;

        int sgn = (d % 4ULL == 1ULL) ? 1 : -1;

        for (idx_t j = d; j < n; j += d)
        {
            if (sgn == 1)
                diff[j] += (seq_t)1;
            else
                diff[j] -= (seq_t)1;   /* safe: a +1 from divisor 1 always present */
        }
    }

    /* -------------------------------------------------------------- */
    /* 2. sieve primes up to n-1                                       */
    /* -------------------------------------------------------------- */
    /* simple sieve: array of bool of size n */
    bool *is_prime = (bool *)malloc((size_t)n);
    if (!is_prime)
    {
        free(diff);
        return 0;
    }

    for (idx_t i = 0; i < n; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    idx_t limit = 0;
    while ((limit + 1) * (limit + 1) <= n - 1)
        ++limit;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!is_prime[i])
            continue;
        for (idx_t j = i * i; j < n; j += i)
            is_prime[j] = false;
    }

    /* collect primes into array */
    uint64_t *primes = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (!primes)
    {
        free(diff);
        free(is_prime);
        return 0;
    }
    size_t prime_cnt = 0;
    for (idx_t i = 2; i < n; ++i)
    {
        if (is_prime[i])
            primes[prime_cnt++] = (uint64_t)i;
    }

    /* -------------------------------------------------------------- */
    /* 3. accumulate the sum over primes                              */
    /* -------------------------------------------------------------- */
    seq_t total = 0;
    for (size_t k = 0; k < prime_cnt; ++k)
    {
        idx_t p = (idx_t)primes[k];
        idx_t m = n - p;          /* 1 <= m < n */
        total += diff[m];
    }

    /* clean up */
    free(diff);
    free(is_prime);
    free(primes);

    return total;
}
