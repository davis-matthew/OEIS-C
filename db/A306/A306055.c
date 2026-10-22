/*
 * A306055  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          4, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A306055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence * a(n) = a(n‑1) + 2*a(n‑2) +
 *            11*a(n‑3) + 3*a(n‑4) * - 13*a(n‑5) - 46*a(n‑6) - 37*a(n‑7) +
 *            8*a(n‑8) * + 45*a(n‑9) + 39*a(n‑10) + 9*a(n‑11) - a(n‑12) -
 *            3*a(n‑13) * with initial values a(1) = 4, a(2) = 12, …, a(14) =
 *            1625872. * The function computes a(n) for all n ≥ 1,...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[15] = {
    0,                      /* dummy for 0‑index */
    4, 12, 27, 58, 189, 526, 1344, 3772,
    10515, 28556, 78229, 215789, 592845, 1625872
};

static seq_t *seq_cache = NULL;   /* 0‑based array of a(1)…a(cache_len) */
static idx_t   cache_len = 0;     /* number of terms stored so far   */
static idx_t   cache_cap = 0;     /* allocated capacity              */

/* Ensure seq_cache contains values up to a(n).  Computes missing terms on demand. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;          /* already computed */

    /* Expand storage if necessary (power‑of‑two growth). */
    if (cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Initialise first 14 terms exactly once. */
    if (cache_len == 0) {
        for (idx_t i = 1; i <= 14; ++i) seq_cache[i-1] = init_vals[i];
        cache_len = 14;
    }

    /* Compute terms from (cache_len+1) up to n inclusive. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        idx_t i = k;             /* 1‑based index for a(k)          */
        seq_t sum = seq_cache[i-2];
        sum += seq_cache[i-3] * (seq_t)2;
        sum += seq_cache[i-4] * (seq_t)11;
        sum += seq_cache[i-5] * (seq_t)3;
        sum -= seq_cache[i-6] * (seq_t)13;
        sum -= seq_cache[i-7] * (seq_t)46;
        sum -= seq_cache[i-8] * (seq_t)37;
        sum += seq_cache[i-9] * (seq_t)8;
        sum += seq_cache[i-10] * (seq_t)45;
        sum += seq_cache[i-11] * (seq_t)39;
        sum += seq_cache[i-12] * (seq_t)9;
        sum -= seq_cache[i-13];
        sum -= seq_cache[i-14] * (seq_t)3;
        seq_cache[k-1] = sum;
    }

    cache_len = n;
}

/* Public function per the specification. */
seq_t a(idx_t n)
{
    if (n <= 14) return init_vals[n];
    ensure_cache(n);
    return seq_cache[n-1];
}
