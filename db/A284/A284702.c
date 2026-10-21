/*
 * A284702  Number of dominating sets in the n-prism graph.
 * https://oeis.org/A284702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 5*a(n-3) + a(n-4) + a(n-5) - a(n-6) -
 *            a(n-7); Initial values: a(1)=3, a(2)=11, a(3)=51, a(4)=183,
 *            a(5)=663, a(6)=2435, a(7)=8935
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_terms[8] = {
    0,          /* unused index 0 */
    3,          /* a(1) */
    11,         /* a(2) */
    51,         /* a(3) */
    183,        /* a(4) */
    663,        /* a(5) */
    2435,       /* a(6) */
    8935        /* a(7) */
};

static seq_t *cache = NULL;      /* 1‑indexed cache of results */
static size_t   cap   = 0;       /* allocated size of cache (>= required +1) */
static idx_t    max_cached = 0;  /* largest n already computed (>=0) */

/* Ensure cache has space for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL || cap <= n) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        /* realloc is safe in C++ and C11; cast for C++ compatibility */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but return 0 for safety */
    ensure_capacity(n);
    if (max_cached < n) {
        idx_t k;
        for (k = max_cached + 1; k <= n; ++k) {
            if (k <= 7) {
                cache[k] = base_terms[k];
            } else {
                /* recurrence: a(k) = 3*a(k‑1) + a(k‑2) + 5*a(k‑3)
                               + a(k‑4) + a(k‑5) – a(k‑6) – a(k‑7) */
                seq_t val = 3 * cache[k-1];
                val += cache[k-2];
                val += 5 * cache[k-3];
                val += cache[k-4];
                val += cache[k-5];
                val -= cache[k-6];
                val -= cache[k-7];
                cache[k] = val;
            }
        }
        max_cached = n;
    }
    return cache[n];
}
