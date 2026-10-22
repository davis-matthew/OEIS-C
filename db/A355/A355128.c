/*
 * A355128  E.g.f. A(x) satisfies A(x) = 1 + x * A(2 * (exp(x) - 1)).
 * https://oeis.org/A355128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A355128
 *  Recurrence:  a(0) = 1;
 *               a(n) = n * Σ_{k=0}^{n-1} 2^k * Stirl2(n-1,k) * a(k),  n ≥ 1
 *
 *  This implementation uses a memoising cache and pre‑computes the required
 *  Stirling numbers of the second kind on demand.  All arithmetic is performed
 *  in seq_t, respecting the restrictions on the available operators.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- cache for computed sequence values ---------- */
    static seq_t *cache = NULL;      /* array of a(0..cache_size-1)          */
    static size_t  cache_cap  = 0;   /* allocated length of cache            */
    static idx_t   cache_size = 0;   /* number of initialised elements       */

    /* ---------- grow cache to include index n ---------- */
    if (n >= (idx_t)cache_size) {
        size_t needed = (size_t)(n + 1);      /* n+1 elements needed */

        if (needed > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = needed;
        }

        /* initialise a(0) if this is the first call */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;   /* a(0) = 1 */
            cache_size = 1;
        }

        /* ---------- cache for Stirling numbers S(m,k) --------- */
        static seq_t *S_row = NULL;      /* holds S(m,k) for current m */
        static size_t  S_cap = 0;        /* allocated length of S_row */
        static idx_t   S_m   = 0;        /* current m value stored */

        if (S_row == NULL) {
            S_cap = 1;
            S_row = (seq_t *)malloc(S_cap * sizeof(seq_t));
            if (!S_row) abort();
            S_row[0] = (seq_t)1;   /* S(0,0) = 1 */
            S_m = 0;
        }

        /* Compute missing values from cache_size up to n */
        for (idx_t m = cache_size; m <= n; ++m) {
            idx_t target_m = m - 1;   /* we need S(target_m,k) for k=0..m-1 */

            /* grow Stirling row to target_m */
            if (target_m > S_m) {
                /* ensure capacity for new indices */
                while (S_cap <= (size_t)target_m) {
                    size_t new_cap = S_cap * 2;
                    if (new_cap == 0) new_cap = 1;
                    seq_t *tmp = (seq_t *)realloc(S_row, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    S_row = tmp;
                    S_cap = new_cap;
                }

                /* incrementally compute rows from S_m+1 to target_m */
                for (idx_t cur = S_m + 1; cur <= target_m; ++cur) {
                    /* S(cur,0) = 0 for cur > 0, will be set after loop */
                    for (idx_t k = cur; k >= 1; --k) {
                        seq_t prev_k  = S_row[k];
                        seq_t prev_km = S_row[k - 1];
                        S_row[k] = (seq_t)k * prev_k + prev_km;
                    }
                    S_row[0] = (seq_t)0;   /* S(cur,0) = 0 */
                    S_m = cur;
                }
            }

            /* Now S_row holds S(target_m,k) for k=0..m-1 */
            seq_t sum  = (seq_t)0;
            seq_t pow2 = (seq_t)1;
            for (idx_t k = 0; k <= m - 1; ++k) {
                seq_t term = S_row[k] * cache[k]; /* Stirling * a(k) */
                term *= pow2;                      /* multiply by 2^k */
                sum += term;
                pow2 *= (seq_t)2;                  /* next power of 2 */
            }
            cache[m] = (seq_t)m * sum;            /* a(m) = m * sum */
        }

        cache_size = n + 1;
    }

    return cache[n];
}
