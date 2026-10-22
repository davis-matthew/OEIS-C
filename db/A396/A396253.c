/*
 * A396253  Sum of Sum_{k=1..n} S(k) over all completely multiplicative sign
 *          sequences S of length n.
 * https://oeis.org/A396253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000196(n) * 2^A000720(n) i.e. a(n) = floor(sqrt(n)) *
 *            pow2(pi(n))
 * Range:     u8 n <= 15, u16 n <= 42, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/pow2.h"      /* seq_t pow2(idx_t exp);  */
#include "../utils/floor_sqrt.h" /* seq_t floor_sqrt(seq_t x); */

static idx_t *pi_cache = NULL;
static size_t pi_cache_size = 0;

/* Compute the prime counting function pi(n) on demand and
   cache the results in a prefix array.  The cache is rebuilt
   from scratch each time it is enlarged; n will stay below
   about 10 000 in the tests, so this is fast enough. */
static void ensure_pi_cache(idx_t n)
{
    if (n < pi_cache_size) return;          /* already enough */

    /* create fresh sieve up to n */
    size_t sz = (size_t)n + 1;
    bool *prime = (bool *)malloc(sz * sizeof(bool));
    if (!prime) return;                      /* allocation failed – leave cache unchanged */
    /* initially assume all numbers >=2 are prime */
    memset(prime, true, sz * sizeof(bool));
    prime[0] = prime[1] = false;
    for (size_t i = 2; i * i <= n; ++i)
        if (prime[i])
            for (size_t j = i * i; j <= n; j += i)
                prime[j] = false;

    /* rebuild pi table */
    idx_t *new_cache = (idx_t *)malloc(sz * sizeof(idx_t));
    if (!new_cache) {
        free(prime);
        return;
    }
    idx_t count = 0;
    new_cache[0] = 0;
    for (size_t i = 1; i <= n; ++i) {
        if (prime[i]) ++count;
        new_cache[i] = count;
    }

    free(prime);
    free(pi_cache);
    pi_cache = new_cache;
    pi_cache_size = sz;
}

static idx_t pi_value(idx_t n)
{
    ensure_pi_cache(n);
    return pi_cache[n];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety for offset 1 */
    seq_t sq = floor_sqrt((seq_t)n);       /* A000196(n) = floor(sqrt(n)) */
    seq_t pi = (seq_t)pi_value(n);          /* A000720(n) = pi(n) */
    return sq * pow2((idx_t)pi);            /* multiply by 2^pi(n) */
}
