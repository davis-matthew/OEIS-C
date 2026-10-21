/*
 * A257797  Permutation of natural numbers: a(1) = 1, a(triangular(n)) =
 *          1+(2*a(n-1)), a(nontriangular(n)) = 2*n, where triangular =
 *          A000217, nontriangular = A014132.
 * https://oeis.org/A257797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 Let t(m) = m(m+1)/2. For n > 1, find k = max{m : t(m) ≤
 *            n}. Then if t(k) = n (n is triangular) → a(n) = 1 + 2 * a(k−1)
 *            else (n is nontriangular) → a(n) = 2 * a(n−k)
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 8256, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt_seq.h"   /* or use custom isqrt implementation */

static seq_t *a_cache = NULL;      /* cache for computed a(n), indices 1.. */
static size_t a_cache_cap = 0;     /* current capacity of a_cache */

/* Ensure that a_cache has room for at least n values, zero‑initialising
   any new entries. */
static void ensure_cache(idx_t n)
{
    if (n <= a_cache_cap) return;
    seq_t *new_cache = (seq_t *) realloc(a_cache, n * sizeof(seq_t));
    if (new_cache == NULL) {
        /* allocation failed – abort in this context */
        abort();
    }
    /* zero initialisation for new part */
    for (idx_t i = a_cache_cap; i < n; ++i) {
        new_cache[i] = (seq_t)0;
    }
    a_cache = new_cache;
    a_cache_cap = n;
}

/* Recursive computation with memoisation */
static seq_t compute_a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* compute k = max{m : t(m) ≤ n} */
    seq_t temp = (seq_t)8 * (seq_t)n + (seq_t)1;   /* 8*n + 1 */
    seq_t sqrt8n1 = isqrt_seq(temp);               /* floor sqrt(8n+1) */
    idx_t k = (idx_t)((sqrt8n1 - (seq_t)1) / (seq_t)2);  /* largest m with t(m) ≤ n */

    /* triangular test: t(k) = k(k+1)/2 ? */
    seq_t tk = (seq_t)k * ((seq_t)k + (seq_t)1) / (seq_t)2;
    bool is_triangular = (tk == (seq_t)n);

    /* cache access */
    ensure_cache(n);
    if (a_cache[n - 1] != (seq_t)0) return a_cache[n - 1];

    seq_t result;
    if (is_triangular) {
        /* a(n) = 1 + 2 * a(k-1) */
        result = (seq_t)1 + (seq_t)2 * compute_a(k - 1);
    } else {
        /* a(n) = 2 * a(n-k) */
        result = (seq_t)2 * compute_a(n - k);
    }

    a_cache[n - 1] = result;
    return result;
}

seq_t a(idx_t n)
{
    return compute_a(n);
}
