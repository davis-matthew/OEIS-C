/*
 * A390303  E.g.f. A(x) satisfies A(x) = exp( x/(1-x^2)^2 * A(x)^2 ).
 * https://oeis.org/A390303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (2*(n-2k)+1)^(n-2k-1) *
 *            binomial(2*n-3k-1 , k) / (n-2k)! (integer arithmetic only)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Cached factorials and sequence values */
static seq_t *fact = NULL;           /* factorial array (size = fact_cap) */
static size_t fact_cap = 0;          /* capacity of factorial array */
static size_t fact_len = 0;          /* number of elements stored (fact_len = current n+1) */

static seq_t *cache = NULL;          /* cached a(n) values (size = cache_cap) */
static size_t cache_cap = 0;         /* capacity of cache array */
static idx_t cache_len = 0;          /* number of values stored (cache_len = current n+1) */

/* Ensure factorial array has capacity at least n+1 */
static void ensure_fact_capacity(idx_t n)
{
    if ((size_t)(n + 1) > fact_cap) {
        size_t new_cap = fact_cap ? fact_cap << 1 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        size_t elems = new_cap * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(fact, elems);
        if (!tmp) abort();           /* allocation failure */
        fact = tmp;
        fact_cap = new_cap;
    }
}

/* Ensure cache array has capacity at least n+1 */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        size_t elems = new_cap * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, elems);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* ---------------- factorials ------------------- */
    ensure_fact_capacity(n);
    if (fact_len == 0) {          /* initialise first element */
        fact[0] = (seq_t)1;
        fact_len = 1;
    }
    for (idx_t i = fact_len; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_len = n + 1;

    /* ---------------- cache ------------------------ */
    ensure_cache_capacity(n);
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 1) {                 /* a(0) = a(1) = 1 */
            cache[i] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        idx_t maxk = i >> 1;          /* floor(i/2) */
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t m = i - 2 * k;      /* m = n-2k */
            if (m == 0)               /* term contributes 0 (binomial will be 0) */
                continue;

            /* binomial part */
            seq_t bin = binomial((idx_t)(2 * i - 3 * k - 1), k);
            if (bin == (seq_t)0)      /* binomial zero → skip */
                continue;

            /* power part: (2*m + 1)^(m-1) */
            seq_t base  = (seq_t)(2 * m + 1);
            seq_t power = big_pow(base, (seq_t)(m - 1));

            /* factorial ratio: i! / m! */
            seq_t ratio = fact[i] / fact[m];

            /* term = bin * power * ratio */
            seq_t term = bin * power;
            term = term * ratio;

            sum += term;
        }
        cache[i] = sum;
    }
    cache_len = n + 1;

    return cache[n];
}
