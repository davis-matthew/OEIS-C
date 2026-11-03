/*
 * A355114  Expansion of e.g.f. 6 / (7 - 6*x - exp(6*x)).
 * https://oeis.org/A355114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = n*a(n-1) + Σ_{k=1..n}
 *            binomial(n,k)*6^(k-1)*a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Ensure cache capacity up to index n */
static void cache_ensure(idx_t n, seq_t **cache, size_t *cap)
{
    size_t needed = (size_t)n + 1;
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 1 : *cap << 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *cache, new_cap);
        if (!tmp) exit(1);            /* out of memory */
        *cache = tmp;
        *cap   = new_cap;
    }
}

/* Multiplicative binomial coefficient */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  computed_max = (idx_t)-1; /* no values yet */

    /* Prepare storage */
    cache_ensure(n, &cache, &cap);

    /* Initial value a(0) */
    if (computed_max == (idx_t)-1) {
        cache[0] = 1;
        computed_max = 0;
    }

    /* Return cached result if already known */
    if (n <= computed_max) return cache[n];

    /* Compute terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)i * cache[i - 1];
        seq_t sum = 0;
        seq_t pow6 = 1;   /* 6^(k-1) */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t t = binom(i, k);
            t *= pow6;           /* 6^(k-1) */
            t *= cache[i - k];   /* a(i-k) */
            sum += t;
            pow6 *= (seq_t)6;    /* next power of 6 */
        }

        cache[i] = term1 + sum;
    }

    computed_max = n;
    return cache[n];
}
