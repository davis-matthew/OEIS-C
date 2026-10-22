/*
 * A383236  The least number of applications of Ackermann-Péter functions to
 *          reach n, starting from 0.
 * https://oeis.org/A383236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 2048, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A383236: 
   Least number of Ackermann–Péter applications to reach n from 0. 
   Uses dynamic programming, exploiting that for n ≤ 10000 only Ackermann
   arguments k = 0,1,2,3,4 need to be considered.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* keep a static cache of computed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of elements known (0..len-1) */
static size_t cache_cap = 0;  /* allocated capacity */

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise cache if first call */
    if (cache_len == 0) {
        cache_cap = 256;  /* initial capacity, can grow */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* OOM, return 0 as fallback */
        cache[0] = 0;                /* a(0) = 0 for internal use */
        cache_len = 1;
    }

    /* grow cache up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 256) new_cap = 256;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* OOM */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute values from current length up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        bool first = true;
        seq_t best = 0;  /* will be overwritten on first candidate */

        /* k = 0 : A(0, m) = m + 1  ->  m = i - 1  */
        {
            seq_t cand = cache[0] + cache[i - 1] + (seq_t)1;
            if (first || cand < best) { best = cand; first = false; }
        }

        /* k = 1 : A(1, m) = m + 2  ->  m = i - 2 */
        if (i >= 2) {
            seq_t cand = cache[1] + cache[i - 2] + (seq_t)1;
            if (cand < best) best = cand;
        }

        /* k = 2 : A(2, m) = 2*m + 3  ->  m = (i - 3)/2  */
        if (i >= 3 && ((i - 3) & 1) == 0) {
            idx_t m = (i - 3) >> 1;
            seq_t cand = cache[2] + cache[m] + (seq_t)1;
            if (cand < best) best = cand;
        }

        /* k = 3 : A(3, m) = 2^(m+3) - 3  ->  m from power‑of‑two test */
        {
            uint64_t val = (uint64_t)(i + 3);
            if (val > 1 && (val & (val - 1)) == 0) {   /* val is a power of two */
                unsigned exp = 0;
                while ((1ULL << exp) < val) ++exp;    /* exp = log2(val) */
                if (exp >= 3) {
                    idx_t m = exp - 3;
                    if (m <= i) {
                        seq_t cand = cache[3] + cache[m] + (seq_t)1;
                        if (cand < best) best = cand;
                    }
                }
            }
        }

        /* k = 4 : only A(4,0) <= 10000 (value 13) */
        if (i == 13) {
            seq_t cand = cache[4] + cache[0] + (seq_t)1;
            if (cand < best) best = cand;
        }

        cache[i] = best;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
