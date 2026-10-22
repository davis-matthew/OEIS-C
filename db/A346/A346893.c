/*
 * A346893  Expansion of e.g.f. 1 / (1 - x^5 * exp(x) / 5!).
 * https://oeis.org/A346893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 41,
 *            big any n
 * Helpers:   binom_k_5, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A346893
 * Expansion of e.g.f. 1 / (1 - x^5 * exp(x) / 5!).
 * Recurrence: a(0) = 1
 *              a(n) = Σ_{k=5..n} binomial(n,k) * binomial(k,5) * a(n-k)
 *
 * The implementation memoizes a(k) for k ≤ n in a dynamic array.
 * binomial(k,5) is computed on the fly; no intermediate overflow
 * beyond the limits of seq_t is required. */

#include <stddef.h>
#include <stdlib.h>
/* Utility for reallocation of a seq_t array. */
#include "../utils/seq_realloc.h"
#include "../utils/binom_k_5.h"

/* Ensure that the cache is large enough for indices up to n.
 * The array is initialized with zeros except a_cache[0] = 1. */
static void
ensure_a_cache(idx_t n, seq_t **cache, size_t *cap, idx_t *len)
{
    if (*cap == 0) {
        *cap = 1;
        *cache = seq_realloc(NULL, 0, 1);
        (*cache)[0] = (seq_t)1;
        *len  = 0;                /* last computed index */
    }
    if ((size_t)n >= *cap) {
        size_t new_cap = (*cap) * 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        *cache = seq_realloc(*cache, *cap, new_cap);
        for (size_t i = *cap; i < new_cap; ++i)
            (*cache)[i] = (seq_t)0;
        *cap = new_cap;
    }
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;
    static size_t a_cap = 0;
    static idx_t a_len  = 0;  /* last index that has been computed */

    /* Expand cache if needed. */
    ensure_a_cache(n, &a_cache, &a_cap, &a_len);

    /* Compute missing terms up to n. */
    for (idx_t cur = a_len + 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;  /* binomial(cur,0) */

        for (idx_t k = 1; k <= cur; ++k) {
            /* Update binom to binomial(cur,k) */
            binom = binom * (seq_t)(cur - k + 1);
            binom = binom / (seq_t)k;

            if (k >= 5) {
                seq_t term = binom;
                term = term * binom_k_5(k);
                term = term * a_cache[cur - k];
                sum += term;
            }
        }
        a_cache[cur] = sum;
    }

    a_len = n;
    return a_cache[n];
}
