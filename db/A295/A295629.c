/*
 * A295629  Number of partitions of n into two parts such that not both are
 *          prime.
 * https://oeis.org/A295629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) - Σ_{i=1..⌊n/2⌋} [is_prime(i) &&
 *            is_prime(n-i)]
 * Range:     u8 n <= 512, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the number of partitions of n into two parts such that not both are prime. */

#include <stdlib.h>
#include <stdbool.h>

/* Cache for primality tests up to the largest n seen so far */
static bool *prime_cache = NULL;    /* boolean array: prime_cache[i] == true iff i is prime */
static idx_t  cache_limit = 0;      /* current maximum n for which prime_cache is valid */

/* Build or extend the sieve up to n */
static void ensure_prime_cache(idx_t n)
{
    if (n <= cache_limit)
        return;                                      /* cache already covers n */

    free(prime_cache);                               /* discard old cache */
    size_t sz = (size_t)(n + 1);
    prime_cache = (bool *)malloc(sz * sizeof(bool));
    if (!prime_cache) {                              /* fallback to zero on allocation failure */
        prime_cache = NULL;
        cache_limit = 0;
        return;
    }

    /* Initialize all entries to true; 0 and 1 are not prime */
    for (size_t i = 0; i < sz; ++i)
        prime_cache[i] = true;
    if (sz > 0) prime_cache[0] = false;
    if (sz > 1) prime_cache[1] = false;

    /* Eratosthenes sieve */
    for (idx_t p = 2; (idx_t)p * p <= n; ++p) {
        if (prime_cache[p]) {
            for (idx_t m = p * p; m <= n; m += p)
                prime_cache[m] = false;
        }
    }
    cache_limit = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure we have the sieve ready for this n */
    ensure_prime_cache(n);
    if (!prime_cache) {
        /* If sieve allocation failed, fall back to a simple, slower check */
        seq_t res = (seq_t)(n / 2);
        for (idx_t i = 1; i <= n / 2; ++i) {
            bool pi, ni;
            /* Simple primality test for i and n-i */
            pi = (i > 1);
            for (idx_t d = 2; d * d <= i; ++d) {
                if (i % d == 0) { pi = false; break; }
            }
            idx_t j = n - i;
            ni = (j > 1);
            for (idx_t d = 2; d * d <= j; ++d) {
                if (j % d == 0) { ni = false; break; }
            }
            if (pi && ni) res -= (seq_t)1;
        }
        return res;
    }

    idx_t half = n / 2;
    seq_t res = (seq_t)half;                     /* floor(n/2) */
    for (idx_t i = 1; i <= half; ++i)
        if (prime_cache[i] && prime_cache[n - i])
            res -= (seq_t)1;
    return res;
}
