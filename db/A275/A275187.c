/*
 * A275187  Number of 7 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the empirical recurrence from the OEIS
   definition:
   a(n) = 2*a(n-1) + 20*a(n-2) + 43*a(n-3)
          - 20*a(n-4) - 92*a(n-5) - 176*a(n-6)
          - 16*a(n-7) - 16*a(n-8) + 64*a(n-9)
   for n > 10, with the initial values
   a(1)=32, a(2)=65, a(3)=389, a(4)=2386, a(5)=14643,
   a(6)=90793, a(7)=561044, a(8)=3472521,
   a(9)=21488129, a(10)=132962186.   */

#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
#  include <vector>
#endif

/* The first ten terms of the sequence */
static const seq_t init_vals[10] = {
    32ULL, 65ULL, 389ULL, 2386ULL, 14643ULL,
    90793ULL, 561044ULL, 3472521ULL, 21488129ULL, 132962186ULL
};

/* ---------------------------------------------------------------------- */
/*  Sequential computation with memoisation                                */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 only */
    if (n < 1) return 0;

#if defined(__cplusplus)

    /* ----- C++ version --------------------------------------------------- */
    /*  Use std::vector for automatic memory management of arbitrary‐
        precision seq_t values.  The vector stores values 1‑based;
        index 0 is unused.  */
    static std::vector<seq_t> cache;   /* cache[0] unused */
    static idx_t cache_n = 0;          /* highest computed index */

    if (n > cache_n) {
        /* Grow the vector if necessary.  New elements are default
           constructed to zero.  */
        cache.resize((size_t)n + 1);

        /* Initialise first ten terms if they are missing.  */
        if (cache_n < 10) {
            for (idx_t i = cache_n + 1; i <= 10 && i <= n; ++i)
                cache[(size_t)i] = init_vals[i - 1];
        }

        /* Recurrence for the remaining positions.  */
        idx_t start = (cache_n < 10) ? 11 : cache_n + 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t pos = 2ULL * cache[(size_t)i - 1]
                      + 20ULL * cache[(size_t)i - 2]
                      + 43ULL * cache[(size_t)i - 3]
                      + 64ULL * cache[(size_t)i - 9];
            seq_t neg = 20ULL * cache[(size_t)i - 4]
                      + 92ULL * cache[(size_t)i - 5]
                      + 176ULL * cache[(size_t)i - 6]
                      + 16ULL * cache[(size_t)i - 7]
                      + 16ULL * cache[(size_t)i - 8];
            cache[(size_t)i] = pos - neg;   /* pos >= neg for all n > 10 */
        }

        cache_n = n;
    }

    return cache[(size_t)n];

#else

    /* ----- C version ----------------------------------------------------- */
    static seq_t *cache_arr = NULL;   /* 1‑based array, cache_arr[0] unused */
    static idx_t  cache_n  = 0;       /* highest index cached */

    if (n > cache_n) {
        /* Grow the array.  Remember the old size so we can zero the new
           part.  */
        idx_t old_size = (cache_n == 0) ? 1 : cache_n + 1;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache_arr,
                                      (size_t)new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */

        /* If expanding, zero the newly allocated elements.  */
        if (new_size > old_size) {
            size_t elems_to_zero = (size_t)(new_size - old_size);
            memset(tmp + old_size, 0, elems_to_zero * sizeof(seq_t));
        }

        cache_arr = tmp;

        /* Initialise the first ten terms if they are missing.  */
        if (cache_n < 10) {
            for (idx_t i = cache_n + 1; i <= 10 && i <= n; ++i)
                cache_arr[(size_t)i] = init_vals[i - 1];
        }

        /* Recurrence for the remaining positions.  */
        idx_t start = (cache_n < 10) ? 11 : cache_n + 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t pos = 2ULL * cache_arr[(size_t)i - 1]
                      + 20ULL * cache_arr[(size_t)i - 2]
                      + 43ULL * cache_arr[(size_t)i - 3]
                      + 64ULL * cache_arr[(size_t)i - 9];
            seq_t neg = 20ULL * cache_arr[(size_t)i - 4]
                      + 92ULL * cache_arr[(size_t)i - 5]
                      + 176ULL * cache_arr[(size_t)i - 6]
                      + 16ULL * cache_arr[(size_t)i - 7]
                      + 16ULL * cache_arr[(size_t)i - 8];
            cache_arr[(size_t)i] = pos - neg;
        }

        cache_n = n;
    }

    return cache_arr[(size_t)n];

#endif
}
