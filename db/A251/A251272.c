/*
 * A251272  Number of (5+1)X(n+1) 0..1 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 46, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251272
   Recurrence:
     a(n) = 13*a(n-1) - 77*a(n-2) + 275*a(n-3) - 660*a(n-4)
            +1122*a(n-5) -1386*a(n-6) +1254*a(n-7) - 825*a(n-8)
            +385*a(n-9)  -121*a(n-10) +23*a(n-11) - 2*a(n-12)
   Initial values:
     a(1)=1049, a(2)=8812, a(3)=49872, a(4)=217634, a(5)=790734,
     a(6)=2510004, a(7)=7189937, a(8)=19018266, a(9)=47251067,
     a(10)=111715980, a(11)=253943272, a(12)=559554564
   The function works for any increasing or decreasing n ≥ 1
   and memoises previously computed terms for efficiency. */
#include <stdlib.h>
#include <stdint.h>

static const long coeff[12] = {
    13, -77, 275, -660, 1122, -1386,
    1254, -825, 385, -121, 23, -2
};

static const seq_t init[12] = {
    1049ULL, 8812ULL, 49872ULL, 217634ULL, 790734ULL, 2510004ULL,
    7189937ULL, 19018266ULL, 47251067ULL, 111715980ULL,
    253943272ULL, 559554564ULL
};

seq_t a(idx_t n)
{
    /* Return 0 for out-of-range indices; the sequence is 1‑based. */
    if (n < 1) return 0;

    /* Memoisation: cache already computed terms. */
    static seq_t *cache = NULL;      /* holds a(1)…a(computed_len)   */
    static idx_t cache_cap = 0;      /* allocated elements            */
    static idx_t computed_len = 0;   /* highest n computed so far     */

    /* Initialise cache with the first 12 terms on the first call. */
    if (cache_cap == 0) {
        cache_cap = 12;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        for (int i = 0; i < 12; ++i) cache[i] = init[i];
        computed_len = 12;
    }

    /* Ensure capacity for all requested terms. */
    if (n > cache_cap) {
        size_t new_cap = (size_t)cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }

    /* Return cached value if already available. */
    if (n <= computed_len) return cache[n - 1];

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = computed_len + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        /* Terms a(i-1)…a(i-12) are stored at indices i-2 … i-13. */
        for (int k = 0; k < 12; ++k) {
            seq_t term = cache[i - 2 - k];
            long c = coeff[k];
            if (c >= 0)
                pos += (seq_t)c * term;
            else
                neg += (seq_t)(-c) * term;
        }
        cache[i - 1] = pos - neg;   /* pos ≥ neg for all n ≥ 1  */
    }

    computed_len = n;
    return cache[n - 1];
}
