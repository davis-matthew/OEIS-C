/*
 * A398312  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - 2*x)) / (1 - x)^2.
 * https://oeis.org/A398312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A398312
 *  a(0) = 1, a(1) = 1, a(2) = 3
 *  for n ≥ 3:
 *      a(n) = 2·a(n−1) − a(n−2) + ∑_{k=1}^{n−1} C(n−2,k−1)·2^{\,n−k−1}·a(k)
 *  (C denotes the binomial coefficient)
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

static seq_t *cache = NULL;       /* storage for computed terms         */
static idx_t  cache_cap = 0;      /* current capacity of 'cache'        */
static idx_t  computed_max = (idx_t)-1;  /* largest index that has been computed */

/* Ensure that the cache can store indices up to 'max_idx'. */
static void ensure_cache(idx_t max_idx)
{
    if (cache_cap > max_idx) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= max_idx) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Initialise the known initial terms (a(0), a(1), a(2)). */
static void init_base(void)
{
    if (computed_max != (idx_t)-1) return;   /* already initialised */
    /* guarantee capacity for indices 0..2 */
    if (cache_cap < 3) {
        idx_t old_cap = cache_cap;
        ensure_cache(2);
        /* preserve any existing values */
        for (idx_t i = 0; i < old_cap && i < 3; ++i) cache[i] = cache[i];
    }
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    computed_max = 2;
}

seq_t a(idx_t n)
{
    /* guard against an unlikely wrap‑around in the caller */
    if (n == (idx_t)-1) return (seq_t)0;

    /* ensure capacity at least for indices up to max(n,2) */
    idx_t need = (n > 2) ? n : (idx_t)2;
    ensure_cache(need);
    init_base();

    if (n <= computed_max) return cache[n];

    /* compute terms progressively up to 'n' */
    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;

        /* binomial coefficient C(i-2,0) = 1 */
        seq_t bin = (seq_t)1;
        /* 2^(i-2) */
        seq_t power = pow2(i - 2);

        for (idx_t k = 1; k <= i - 1; ++k)
        {
            /* term = C(i-2,k-1) * 2^(i-k-1) * a(k) */
            seq_t term = bin * power * cache[k];
            sum += term;

            /* update binomial and power for next k, unless we are at the end */
            if (k < i - 1)
            {
                /* C' = C * (i-2-(k-1)) / k */
                seq_t numer = (seq_t)((i - 2) - (k - 1));
                bin = (bin * numer) / (idx_t)k;
                /* 2^(i-(k+1)-1) = power / 2 */
                power >>= 1;
            }
        }

        seq_t val = cache[i - 1] * (seq_t)2;
        val -= cache[i - 2];
        val += sum;
        cache[i] = val;
    }

    computed_max = n;
    return cache[n];
}
