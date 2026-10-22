/*
 * A394282  a(n) is the largest prime p such that A284665(n) = p^i + p^j with
 *          i, j > 0.
 * https://oeis.org/A394282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime p such that the n‑th even integer that can
 *            be written as a sum of two positive prime powers (i,j>0) equals
 *            p^i + p^j. The function memoises results and works for any order
 *            of calls
 * Range:     u8 n <= 109, u16 n <= 6926, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_power_of_uint64.h"

/* ---- helpers ---------------------------------------------------- */

/* Factor n into distinct prime factors.  The array fac must be big
   enough to hold all distinct primes of n.  Returns the number of
   distinct prime factors in *cnt.  All values fit in uint64_t. */
static void factor_uint64(uint64_t n, uint64_t fac[20], size_t *cnt)
{
    size_t c = 0;
    if (n % 2ULL == 0) {
        fac[c++] = 2ULL;
        while (n % 2ULL == 0)
            n /= 2ULL;
    }
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) {
            fac[c++] = d;
            while (n % d == 0)
                n /= d;
        }
    }
    if (n > 1ULL)
        fac[c++] = n;
    *cnt = c;
}

/* For a given even integer X, return the largest prime p such that
   X = p^i + p^j with i,j > 0.  Returns 0 if no such representation exists. */
static seq_t largest_prime_representation_uint64(uint64_t X)
{
    uint64_t fac[20];
    size_t fac_cnt;
    factor_uint64(X, fac, &fac_cnt);
    seq_t best = (seq_t)0;

    for (size_t k = 0; k < fac_cnt; ++k) {
        uint64_t p = fac[k];
        uint64_t power = p;
        while (power < X) {
            uint64_t rem = X - power;
            if (rem % p == 0) {
                if (is_power_of_uint64(rem / p, p)) {
                    if ((seq_t)p > best)
                        best = (seq_t)p;
                    break;          /* representation found for this p */
                }
            }
            if (power > X / p)
                break;                  /* avoid overflow */
            power *= p;
        }
    }
    return best;
}

/* ---- memoised sequence ---------------------------------------- */

static seq_t *cache   = NULL;   /* array of already computed a(n) values    */
static size_t cache_cap = 0;    /* allocated capacity of cache array        */
static size_t cache_len = 0;    /* number of filled entries in cache        */
static uint64_t next_even = 2ULL;   /* last even integer examined (starts at 2) */

/* Ensure cache has room for at least new_cap entries.  Doubles size as needed. */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_size = cache_cap ? cache_cap << 1 : 64;
    if (new_size < new_cap)
        new_size = new_cap;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_ptr)               /* memory exhaustion is unlikely; leave unchanged */
        return;
    cache     = new_ptr;
    cache_cap = new_size;
}

/* Public function requested by OEIS. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; undefined for n==0 */
        return (seq_t)0;

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        next_even += 2ULL;   /* only even integers */
        seq_t candidate = largest_prime_representation_uint64(next_even);
        if (candidate != (seq_t)0) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = candidate;
        }
    }
    return cache[n - 1];
}
