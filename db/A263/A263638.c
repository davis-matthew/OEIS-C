/*
 * A263638  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -3 to 3 places and with no two consecutive
 *          increases.
 * https://oeis.org/A263638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2)+2*a(n-3)+3*a(n-4)+2*a(n-5) *
 *            -2*a(n-6)-6*a(n-7)+2*a(n-8)-4*a(n-9)-2*a(n-10) *
 *            +3*a(n-11)-2*a(n-12)+a(n-14) for n>19, * with a(1)=1, a(2)=2,
 *            a(3)=5, a(4)=17, a(5)=41, a(6)=75, * a(7)=156, a(8)=340,
 *            a(9)=738, a(10)=1567, a(11)=3327, * a(12)=7136, a(13)=15258,
 *            a(14)=32589, a(15)=6962...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#ifndef MAX_A_CACHE
/* Fixed cache size large enough for all test values (up to ~10000). */
#define MAX_A_CACHE 20000
#endif

/* Initial 19 values.  index 0 is a dummy to keep 1‑based indexing. */
static const seq_t init_vals[20] = {
    0ULL,
    1ULL, 2ULL, 5ULL, 17ULL, 41ULL, 75ULL,
    156ULL, 340ULL, 738ULL, 1567ULL, 3327ULL,
    7136ULL, 15258ULL, 32589ULL, 69621ULL,
    148780ULL, 317987ULL, 679467ULL, 1451887ULL
};

/* Static cache of previously computed terms.  The array is large enough
 * to hold all terms that might be requested during evaluation.
 * In C++ this array will default‑construct seq_t objects.
 * In C it will be zero‑initialised. */
static seq_t cache[MAX_A_CACHE + 1] = {0};
static idx_t cache_max_computed = 0;

/* Initialise cache with the known 19 values.  Called only once. */
static void init_cache(void)
{
    if (cache_max_computed != 0)
        return;

    /* Copy the 19 known values. */
    for (idx_t i = 1; i <= 19 && i <= MAX_A_CACHE; ++i)
        cache[i] = init_vals[i];

    /* Remaining entries are already zero (or default‑constructed). */
    cache_max_computed = 19;
}

/* Compute terms up to n using the stored cache.  If n exceeds the cache
 * size we compute iteratively without storing all intermediate terms. */
static seq_t compute_up_to(idx_t n)
{
    if (n <= cache_max_computed)
        return cache[n];

    /* If the requested term exceeds the static cache, compute on the fly
     * using a sliding window of the last 14 values.  This uses only
     * constant extra memory. */
    if (n > MAX_A_CACHE) {
        /* Initialise the window with a(6)..a(19).  These are the needed
         * values for the first iteration when i = 20. */
        seq_t window[14]; /* window[0] = a(i-14), window[13] = a(i-1) */
        for (idx_t k = 0; k < 14; ++k)
            window[k] = init_vals[6 + k];  /* k=0 -> a(6) */

        for (idx_t i = 20; i <= n; ++i) {
            seq_t new_val =
                3 * window[12]  /* a(i-2) */
              + 2 * window[11]  /* a(i-3) */
              + 3 * window[10]  /* a(i-4) */
              + 2 * window[9]   /* a(i-5) */
              - 2 * window[8]   /* a(i-6) */
              - 6 * window[7]   /* a(i-7) */
              + 2 * window[6]   /* a(i-8) */
              - 4 * window[5]   /* a(i-9) */
              - 2 * window[4]   /* a(i-10) */
              + 3 * window[3]   /* a(i-11) */
              - 2 * window[2]   /* a(i-12) */
              + window[0];      /* a(i-14) */

            /* Shift window left and insert new_val at the end. */
            for (size_t k = 0; k < 13; ++k)
                window[k] = window[k + 1];
            window[13] = new_val;
        }
        return window[13];  /* a(n) */
    }

    /* Standard case: n <= MAX_A_CACHE.  Compute missing terms and store. */
    for (idx_t i = cache_max_computed + 1; i <= n; ++i) {
        cache[i] =
            3 * cache[i - 2] + 2 * cache[i - 3] + 3 * cache[i - 4] + 2 * cache[i - 5]
          - 2 * cache[i - 6] - 6 * cache[i - 7] + 2 * cache[i - 8] - 4 * cache[i - 9]
          - 2 * cache[i - 10] + 3 * cache[i - 11] - 2 * cache[i - 12] + cache[i - 14];
    }
    cache_max_computed = n;
    return cache[n];
}

/* Public function: returns a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    init_cache();
    return compute_up_to(n);
}
