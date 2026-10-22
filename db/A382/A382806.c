/*
 * A382806  a(n) = Sum_{k=0..n} (k!)^2 * binomial(k+2,2) * Stirling1(n,k)^2.
 * https://oeis.org/A382806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k!)^2 * binomial(k+2,2) * Stirling1(n,k)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global caches (shared across calls) */
static idx_t alloc_size = 0;
static idx_t max_cached_n = 0;
static seq_t *a_cache = NULL;
static seq_t *fact_cache = NULL;
static seq_t *s_prev = NULL;
static seq_t *s_cur = NULL;

/* Ensure the arrays are large enough for indices up to 'needed' */
static void ensure_capacity(idx_t needed)
{
    if (needed < alloc_size) return;
    idx_t new_size = alloc_size ? alloc_size * 2 : 1;
    while (new_size <= needed) new_size <<= 1;

    a_cache   = (seq_t*)realloc(a_cache,   sizeof(seq_t) * new_size);
    fact_cache= (seq_t*)realloc(fact_cache,sizeof(seq_t) * new_size);
    s_prev    = (seq_t*)realloc(s_prev,   sizeof(seq_t) * new_size);
    s_cur     = (seq_t*)realloc(s_cur,    sizeof(seq_t) * new_size);

    for (idx_t i = alloc_size; i < new_size; ++i) {
        a_cache[i]   = 0;
        fact_cache[i] = 0;
        s_prev[i]    = 0;
        s_cur[i]     = 0;
    }
    alloc_size = new_size;
}

seq_t a(idx_t n)
{
    /* Initialise on first use */
    if (alloc_size == 0) {
        ensure_capacity(1);
        a_cache[0]   = 1;          /* a(0) = 1 */
        fact_cache[0] = 1;         /* 0! = 1 */
        s_prev[0]    = 1;          /* Stirling row for n=0: s(0,0)=1 */
        s_cur[0]     = 0;
        max_cached_n = 0;
    }

    if (n >= alloc_size) {
        ensure_capacity(n);
    }

    /* Compute values up to requested index */
    for (idx_t m = max_cached_n + 1; m <= n; ++m) {
        /* Compute factorials up to m */
        for (idx_t i = max_cached_n + 1; i <= m; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }

        /* Compute the Stirling numbers for n = m (unsigned version) */
        s_cur[0] = 0;
        seq_t mult_factor = (seq_t)(m - 1);
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term1 = s_prev[k - 1];
            seq_t term2 = 0;
            if (k <= m - 1) term2 = s_prev[k];
            s_cur[k] = term1 + term2 * mult_factor;
        }

        /* Sum the contribution for this n */
        seq_t a_m = 0;
        for (idx_t k = 0; k <= m; ++k) {
            seq_t fac_k = fact_cache[k];
            /* binomial(k+2,2) = (k+2)*(k+1)/2 */
            seq_t binfact = (seq_t)(((uint64_t)(k + 2) * (uint64_t)(k + 1)) / 2);
            seq_t weight  = fac_k * fac_k * binfact;
            seq_t s2      = s_cur[k] * s_cur[k];
            a_m += weight * s2;
        }
        a_cache[m] = a_m;

        /* Prepare for next iteration */
        seq_t *tmp = s_prev;
        s_prev = s_cur;
        s_cur = tmp;
        max_cached_n = m;
    }

    return a_cache[n];
}
