/*
 * A317685  Number of partitions of n into a prime and two positive squares.
 * https://oeis.org/A317685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = Sum_{prime p} A025426(n-p)
 * Range:     u8 n <= 2540, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n <= UINT64_MAX. */
static inline idx_t int_sqrt_idx(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        r++;
    return r;
}

/* seq_t type is defined by the build system. */
seq_t a(idx_t n)
{
    /* Static cache for primes up to current maximum n */
    static idx_t max_cached_n = 0;
    static idx_t *prime_list = NULL;   /* primes up to max_cached_n */
    static size_t prime_count  = 0;     /* number of primes stored */
    static seq_t *ways = NULL;         /* ways[m] = # of pairs (a,b) with a^2+b^2=m, a<=b */
    static size_t ways_size = 0;        /* length of ways array (max_cached_n+1) */

    /* If we need to handle a bigger n, rebuild the cache */
    if (n > max_cached_n) {
        /* Free previous cache (if any) – the test harness doesn't need it */
        /* Sieve of Eratosthenes up to n to generate the primes */
        idx_t limit = n;
        bool *is_composite = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
        if (is_composite == NULL) return 0;  /* allocation failure – should not happen */

        /* Reallocate prime list to hold all possible numbers up to limit + 1 */
        prime_list = (idx_t *)realloc(prime_list, (size_t)(limit + 1) * sizeof(idx_t));
        if (prime_list == NULL) {
            free(is_composite);
            return 0;
        }
        prime_count = 0;
        for (idx_t i = 2; i <= limit; i++) {
            if (!is_composite[i]) {
                prime_list[prime_count++] = i;
                if (i * i <= limit) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        is_composite[j] = true;
                }
            }
        }
        free(is_composite);

        /* Allocate and compute the ways array */
        ways_size = (size_t)(n + 1);
        ways = (seq_t *)realloc(ways, ways_size * sizeof(seq_t));
        if (ways == NULL) return 0;
        for (size_t i = 0; i < ways_size; i++)
            ways[i] = 0;

        /* Count unordered pairs of positive squares (a <= b) */
        idx_t max_a = int_sqrt_idx(n);
        for (idx_t a = 1; a <= max_a; a++) {
            idx_t a_sq = a * a;
            for (idx_t b = a; ; b++) {
                idx_t sum = a_sq + b * b;
                if (sum > n) break;
                ways[sum] += 1;
            }
        }

        max_cached_n = n;
    }

    /* Compute a(n) by summing ways[n-p] over all primes p <= n */
    seq_t result = 0;
    for (size_t i = 0; i < prime_count; i++) {
        idx_t p = prime_list[i];
        if (p > n) break;
        idx_t r = n - p;
        result += ways[(size_t)r];
    }
    return result;
}
