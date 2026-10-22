/*
 * A349022  G.f. satisfies A(x) = 1/(1 - x/(1 - x*A(x))^3)^4.
 * https://oeis.org/A349022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A349022
 *
 * Definition (OEIS formula):
 *   a(n) = Sum_{k=0..n} C(4*n - 3*k + 3, k) *
 *                C(n + 2*k - 1, n - k) / (n - k + 1)
 *
 * This implementation uses the provided binomial helper
 * (../utils/binomial.h) for exact combinatorial values.
 * It caches previously computed values so that repeated
 * or out‑of‑order calls are O(1).
 */

#include <stdlib.h>                 /* calloc, realloc */
#include "../utils/binomial.h"      /* seq_t binomial(idx_t, idx_t) */

static seq_t *cache = NULL;         /* dynamic array of cached a(n) */
static idx_t cache_cap = 0;         /* current size of the cache */

/* Ensure the cache array can hold at least n+1 entries.
 * Newly allocated elements are zeroed (meaning “not computed”).
 */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        cache_cap = (n + 1) * 2;
        if (cache_cap < 2) cache_cap = 2;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    } else if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        while (n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;             /* allocation failed – keep old cache */
        /* initialize newly allocated slots to 0 */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }
    if (n == 0 && cache[0] == 0) cache[0] = 1; /* a(0) = 1 */
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* explicit base case */

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];   /* memoised result */

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t t1 = binomial(4 * n - 3 * k + 3, k);
        if (t1 == 0) continue;            /* skip zero contribution */
        seq_t t2 = binomial(n + 2 * k - 1, n - k);
        if (t2 == 0) continue;
        seq_t denom = (seq_t)(n - k + 1);   /* cast idx_t to seq_t for division */
        seq_t prod = (t1 * t2) / denom;     /* exact division – guaranteed integer */
        sum += prod;
    }

    cache[n] = sum;                      /* store for future calls */
    return sum;
}
