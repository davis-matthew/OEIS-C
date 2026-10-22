/*
 * A387649  a(n) = Sum_{k=0..floor(n/2)} 2^(n-2*k) * binomial(2*k+1,2*n-4*k).
 * https://oeis.org/A387649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387649
 * a(n) = Sum_{k=0..floor(n/2)} 2^(n-2*k) * binomial(2*k+1, 2*n-4*k)
 * Recurrence: a(n) = 2*a(n-2) + 4*a(n-3) - a(n-4) + 4*a(n-5) - 4*a(n-6),
 *   with a(0)=1, a(1)=0, a(2)=1, a(3)=6, a(4)=1, a(5)=20.
 */

#include <stdlib.h>   /* malloc, free, size_t */
#ifdef __cplusplus
#include <cstddef>
#endif

/* forward declaration of helper binomial function */
extern seq_t binomial(idx_t n, idx_t k);

/* static cache of computed values */
#ifdef __cplusplus
static seq_t *cache = nullptr;
#else
static seq_t *cache = NULL;
#endif
static size_t cache_size = 0;

/* Utility to free old cache when reallocating */
static void free_cache(void) {
#if defined(__cplusplus)
    delete[] cache;
#else
    free(cache);
#endif
}

/* Function to compute a(n) */
seq_t a(idx_t n) {
    /* Ensure cache is large enough and filled up to n */
    if (n >= cache_size) {
        /* Determine new size: at least 6 to hold initial values */
        size_t needed = (size_t)n + 1;
        if (needed < 6) needed = 6;

        /* Allocate new array */
        seq_t *new_cache;
#if defined(__cplusplus)
        new_cache = new seq_t[needed];
#else
        new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
#endif
        if (!new_cache) { /* Allocation failed */
#if defined(__cplusplus)
            throw std::bad_alloc();
#else
            return (seq_t)0;
#endif
        }

        /* Copy existing data */
        size_t i;
        for (i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Initialize first six values if they weren't set yet */
        if (cache_size == 0) {
            new_cache[0] = (seq_t)1;
            new_cache[1] = (seq_t)0;
            new_cache[2] = (seq_t)1;
            new_cache[3] = (seq_t)6;
            new_cache[4] = (seq_t)1;
            new_cache[5] = (seq_t)20;
            i = 6; /* next index to compute */
        } else {
            i = cache_size; /* continue from where we left off */
        }

        /* Compute missing terms using recurrence */
        for (; i <= (size_t)n; ++i) {
            /* a(i) = 2*a(i-2) + 4*a(i-3) - a(i-4) + 4*a(i-5) - 4*a(i-6) */
            new_cache[i] = new_cache[i-2] * (seq_t)2
                         + new_cache[i-3] * (seq_t)4
                         - new_cache[i-4]
                         + new_cache[i-5] * (seq_t)4
                         - new_cache[i-6] * (seq_t)4;
        }

        /* Free old array and hand over new one */
        free_cache();
        cache = new_cache;
        cache_size = needed;
    }
    /* Return requested value (guaranteed computed) */
    return cache[(size_t)n];
}
