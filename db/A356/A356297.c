/*
 * A356297  a(n) = n! * Sum_{k=1..n} sigma_0(k)/k.
 * https://oeis.org/A356297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} sigma_0(k)/k (OEIS A356297)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached data for divisor counts and factorials. */
/* idx_t and seq_t are provided by the build environment. */
static idx_t max_n = 0;
static uint32_t *div_cnt = NULL;   /* divisor count for 0..max_n   */
static seq_t    *fact_cache = NULL;/* factorials 0! .. max_n!     */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* safety for out‑of‑bounds calls */

    /* Expand precomputed data when a larger n is requested. */
    if (n > max_n) {
        /* Reallocate divisor-count array. */
        size_t new_len = (size_t)(n + 1);
        uint32_t *tmp = (uint32_t *)realloc(div_cnt, new_len * sizeof(uint32_t));
        if (!tmp) return 0;            /* memory failure – unlikely */
        div_cnt = tmp;

        /* Reallocate factorial cache. */
        seq_t *tmpf = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));
        if (!tmpf) return 0;
        fact_cache = tmpf;

        /* Recompute all divisor counts up to n. */
        for (idx_t i = 0; i <= n; ++i) div_cnt[i] = 0;
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                ++div_cnt[j];
            }
        }

        /* Recompute factorials up to n. */
        if (max_n == 0) fact_cache[0] = (seq_t)1;  /* 0! = 1 */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }

        max_n = n;   /* update the maximum seen */
    }

    /* Compute a(n) = n! * Σ_{k=1..n} d(k)/k. */
    seq_t sum  = (seq_t)0;
    seq_t fact = fact_cache[n];
    for (idx_t k = 1; k <= n; ++k) {
        /* n! / k is exactly divisible, so integer arithmetic works. */
        seq_t term = fact / (seq_t)k;
        term *= (seq_t)div_cnt[k];
        sum += term;
    }
    return sum;
}
