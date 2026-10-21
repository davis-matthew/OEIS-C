/*
 * A263385  Number of (n+1)X(1+1) arrays of permutations of 0..n*1-1 with each
 *          element moved a city block distance of exactly 2.
 * https://oeis.org/A263385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) + 15*a(n-3) + 35*a(n-4) + 25*a(n-5) + 5*a(n-6) -
 *            5*a(n-7) - 25*a(n-8) - 35*a(n-9) - 15*a(n-10) - 5*a(n-11) +
 *            a(n-13) Initial terms: a(1)=1, a(2)=4, a(3)=16, a(4)=36,
 *            a(5)=169, a(6)=576, a(7)=2025, a(8)=7056, a(9)=25600,
 *            a(10)=90000, a(11)=319225, a(12)=1132096, a(13)=4020025
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- helper allocation routine ------------------------------------ */
static inline seq_t *alloc_seq_arr(size_t n) {
    return (seq_t *)calloc(n, sizeof(seq_t));
}
static inline void free_seq_arr(seq_t *p) {
    free(p);
}

/* ---------- main sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache with first 13 values and memoisation */
    static seq_t *cache  = NULL;
    static idx_t  cap    = 0;      /* allocated capacity (in elements)    */
    static idx_t  maxidx = 0;      /* largest index computed so far      */

    /* base terms (1‑based indexing) */
    const seq_t base[13] = {
        1ULL,        /* a(1) */
        4ULL,        /* a(2) */
        16ULL,       /* a(3) */
        36ULL,       /* a(4) */
        169ULL,      /* a(5) */
        576ULL,      /* a(6) */
        2025ULL,     /* a(7) */
        7056ULL,     /* a(8) */
        25600ULL,    /* a(9) */
        90000ULL,    /* a(10) */
        319225ULL,   /* a(11) */
        1132096ULL,  /* a(12) */
        4020025ULL   /* a(13) */
    };

    /* initialise cache if needed, or enlarge it */
    if (!cache) {
        cap = n + 1;
        if (cap < 14) cap = 14;            /* space for the 13 base terms */
        cache = alloc_seq_arr(cap);
        for (idx_t i = 1; i <= 13; ++i)  cache[i] = base[i-1];
        maxidx = 13;
        if (n <= 13) return cache[n];
    } else if (n >= cap) {
        size_t newcap = cap ? cap : 2;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = alloc_seq_arr(newcap);
        for (size_t i = 0; i < (size_t)cap; ++i) tmp[i] = cache[i];
        free_seq_arr(cache);
        cache = tmp;
        cap   = newcap;
    } else if (n <= maxidx) {
        /* value already computed and cached */
        return cache[n];
    }

    /* compute all missing terms up to n (recurrence starts at 14) */
    for (idx_t i = maxidx + 1; i <= n; ++i) {
        cache[i] =
            cache[i-2]*5ULL + cache[i-3]*15ULL + cache[i-4]*35ULL
          + cache[i-5]*25ULL + cache[i-6]*5ULL
          - cache[i-7]*5ULL - cache[i-8]*25ULL - cache[i-9]*35ULL
          - cache[i-10]*15ULL - cache[i-11]*5ULL + cache[i-13];
    }
    maxidx = n;
    return cache[n];
}
