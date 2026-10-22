/*
 * A385985  a(0) = 1; a(n) = Sum_{k=0..n-1} (2^k + 1) * binomial(n-1,k) * a(k)
 *          * a(n-1-k).
 * https://oeis.org/A385985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385985: a(0)=1, a(n)=Σ_{k=0}^{n-1} (2^k+1)*C(n-1,k)*a(k)*a(n-1-k) */
/* Incremental DP with memoisation; works for built‑in integer types and
   for arbitrary‑precision seq_t (class type). */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array */
static idx_t cache_cap = 0;      /* number of cached elements (0..cache_cap-1) */

/* Compute the A385985 value a(n) for any non‑negative integer n. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = (seq_t)1;
        cache_cap = 1;
    }

    if (n >= cache_cap) {
        /* Allocate space up to n. */
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;

        /* Compute missing values from cache_cap up to n. */
        for (idx_t i = cache_cap; i <= n; ++i) {
            if (i == 0) continue;               /* already set */

            idx_t m = i - 1;                     /* sum over k = 0..m */
            seq_t sum = 0;
            seq_t pow2k = (seq_t)1;              /* 2^0 */
            seq_t C = (seq_t)1;                  /* binomial coefficient C(m,0) */
            seq_t two = (seq_t)2;

            for (idx_t k = 0; k <= m; ++k) {
                seq_t term = (pow2k + (seq_t)1) * C;
                term = term * cache[k];
                term = term * cache[m - k];
                sum = sum + term;

                /* Prepare for next k. */
                pow2k = pow2k * two;             /* 2^(k+1) */
                if (k < m) {
                    seq_t num = (seq_t)(m - k);
                    seq_t den = (seq_t)(k + 1);
                    C = (C * num) / den;        /* C(m,k+1) */
                }
            }
            cache[i] = sum;
        }
        cache_cap = n + 1;
    }

    return cache[n];
}
