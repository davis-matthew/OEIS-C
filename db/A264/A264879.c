/*
 * A264879  Number of (2+1)X(n+1) arrays of permutations of 0..n*3+2 with each
 *          element having directed index change 0,1 0,-1 -2,0 or 1,1.
 * https://oeis.org/A264879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 49, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A264879
 * Formula: a(n) = 10*a(n-2) + 6*a(n-3) - 42*a(n-4) - 40*a(n-5)
 *            + 117*a(n-6) + 143*a(n-7) - 222*a(n-8) - 266*a(n-9)
 *            + 272*a(n-10) + 266*a(n-11) - 222*a(n-12) - 143*a(n-13)
 *            + 117*a(n-14) + 40*a(n-15) - 42*a(n-16) - 6*a(n-17)
 *            + 10*a(n-18) - a(n-20)
 * Initial values:
 * a(1)=1, a(2)=0, a(3)=1, a(4)=1, a(5)=7, a(6)=20, a(7)=49, a(8)=175,
 * a(9)=323, a(10)=1085, a(11)=2290, a(12)=6489, a(13)=15751, a(14)=41749,
 * a(15)=105514, a(16)=276079, a(17)=706396, a(18)=1825615, a(19)=4714184,
 * a(20)=12103729 */

#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------*/
/* Base values for n = 1 .. 20                                        */
static const seq_t base_vals[20] = {
    (seq_t)1,  (seq_t)0,  (seq_t)1,  (seq_t)1,
    (seq_t)7,  (seq_t)20, (seq_t)49, (seq_t)175,
    (seq_t)323, (seq_t)1085, (seq_t)2290, (seq_t)6489,
    (seq_t)15751, (seq_t)41749, (seq_t)105514, (seq_t)276079,
    (seq_t)706396, (seq_t)1825615, (seq_t)4714184, (seq_t)12103729
};

/* Offsets and coefficients of the recurrence (18 terms).           */
static const int offsets[18] = {
    2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17, 18, 20
};
static const int coeffs[18] = {
    10, 6, -42, -40, 117, 143, -222, -266,
    272, 266, -222, -143, 117, 40, -42, -6,
    10, -1
};

/*------------------------------------------------------------------*/
/* Cached values: 1‑based indexing.  The array is grown on demand.   */
static seq_t *cache = NULL;      /* cache itself */
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_len = 0;    /* highest index computed (1‑based) */

/*------------------------------------------------------------------*/
static void ensure_cache_size(idx_t n)
{
    /* Allocate the cache if it does not exist yet. */
    if (cache == NULL) {
        cache_cap = 64;                      /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Copy the known base values. */
        size_t copy_len = (n < 20) ? (size_t)n : 20;
        memcpy(cache, base_vals, copy_len * sizeof(seq_t));
        /* Zero any remaining elements in the initial capacity. */
        if (cache_cap > copy_len)
            memset(cache + copy_len, 0,
                   (cache_cap - copy_len) * sizeof(seq_t));
        cache_len = copy_len;
    }
    /* Grow the capacity if required. */
    else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while ((size_t)n > new_cap) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_cap * sizeof(seq_t));
        /* Zero the newly allocated part. */
        memset(new_cache + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms that are not yet in the cache. */
    if ((size_t)n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t pos = 0;   /* sum of positive contributions   */
            seq_t neg = 0;   /* sum of negative contributions   */
            for (int k = 0; k < 18; ++k) {
                idx_t idx = i - offsets[k];           /* 1‑based index */
                seq_t term = cache[idx - 1];
                int   c    = coeffs[k];
                seq_t mult;
                if (c >= 0) {
                    mult = term * (seq_t)c;
                    pos += mult;
                } else {
                    mult = term * (seq_t)(-c);
                    neg += mult;
                }
            }
            /* pos is guaranteed to be >= neg for this sequence. */
            cache[i - 1] = pos - neg;
        }
        cache_len = (size_t)n;
    }
}

/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* outside the defined offset */
    if (n <= 20) return base_vals[n - 1];/* directly from the base values */

    ensure_cache_size(n);
    return cache[n - 1];
}
