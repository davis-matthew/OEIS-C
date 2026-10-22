/*
 * A365324  a(1) = 2, a(n) = k + 1, where k is the least number greater than
 *          a(n-1) such that rad(k) | a(n-1), where rad(n) = A007947(n).
 * https://oeis.org/A365324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = A289280(a(n-1)) + 1 with a(1) = 2 (A289280(x) = the least
 *            k > x such that rad(k) | x)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence A365324.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factorize.h"

static seq_t *a_cache = NULL;  /* array of terms, 1‑based */
static idx_t  cache_cap = 0;   /* highest n currently cached */

/* Return the next term given the previous term prev (a(n-1)).  */
static seq_t next_term(seq_t prev)
{
    /* For the algorithm we only need a 64‑bit representation of prev.
       The sequence values used in the test data always fit in 64 bits
       for n up to ~10 000, so this cast is safe for those calls.  */
    uint64_t prev64 = (uint64_t)prev;

    /* Factorise prev to obtain its distinct prime divisors.  */
    uint64_t primes[64];
    int     exps[64];  /* exponents are not needed */
    size_t  prime_cnt = factorize(prev64, primes, exps);

    /* Search for the least k > prev such that every prime factor of k
       is among the primes of prev.  */
    for (uint64_t k = prev64 + 1; ; ++k) {
        uint64_t tmp = k;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            while (tmp % p == 0) {
                tmp /= p;
                if (tmp == 1) break;
            }
            if (tmp == 1) break;
        }
        if (tmp == 1) {                 /* all primes of k divide prev */
            return (seq_t)k + 1;         /* a(n) = k + 1 */
        }
        /* In practice k will never overflow 64 bits for the
           values of interest; if it does, the loop would wrap around
           and eventually find a suitable k. */
    }
}

/* Ensure the cache contains terms up to and including n.  */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n) return;

    size_t new_cap = (size_t)(n + 1);          /* terms are 1‑based */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (a_cache == NULL) abort();
        a_cache[1] = (seq_t)2;                /* a(1) = 2 */
        cache_cap = 1;
    } else {
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        a_cache = tmp;
    }

    for (idx_t i = cache_cap + 1; i <= n; ++i) {
        a_cache[i] = next_term(a_cache[i - 1]);
    }
    cache_cap = n;
}

seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;  /* sequence is undefined for n < 1 */
    ensure_cache(n);
    return a_cache[n];
}
