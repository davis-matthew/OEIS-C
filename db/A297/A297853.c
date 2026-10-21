/*
 * A297853  Number of n X 3 0..1 arrays with every element equal to 1, 2 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-4) - a(n-5) - a(n-6) + a(n-7) + a(n-8) for
 *            n>9 Initial values: a(1)=1, a(2)=7, a(3)=15, a(4)=19, a(5)=21,
 *            a(6)=33, a(7)=53, a(8)=77, a(9)=111
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifndef __cplusplus
#include <stdlib.h>
#endif

#ifdef __cplusplus
#include <vector>
#endif

/* Static storage for the sequence */
#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
#else
static seq_t *cache = NULL;
static idx_t cache_len = 0;
#endif

seq_t a(idx_t n)
{
    /* Indexing follows the OEIS convention: a(1) is the first element */
    if (n < 1)  /* safety: behaviour undefined for n=0 */
        return (seq_t)0;

    /*-----------------------------------------------------------*/
    /* 1.  Initialise cache with the first 9 terms                */
    /*-----------------------------------------------------------*/
#ifdef __cplusplus
    if (cache_vec.empty()) {
        cache_vec.resize(9);
        cache_vec[0] = (seq_t)1;   /* a(1)  */
        cache_vec[1] = (seq_t)7;   /* a(2)  */
        cache_vec[2] = (seq_t)15;  /* a(3)  */
        cache_vec[3] = (seq_t)19;  /* a(4)  */
        cache_vec[4] = (seq_t)21;  /* a(5)  */
        cache_vec[5] = (seq_t)33;  /* a(6)  */
        cache_vec[6] = (seq_t)53;  /* a(7)  */
        cache_vec[7] = (seq_t)77;  /* a(8)  */
        cache_vec[8] = (seq_t)111; /* a(9)  */
    }
#else
    if (!cache) {
        cache_len = 9;
        cache = (seq_t*)malloc((size_t)cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(1)  */
        cache[1] = (seq_t)7;   /* a(2)  */
        cache[2] = (seq_t)15;  /* a(3)  */
        cache[3] = (seq_t)19;  /* a(4)  */
        cache[4] = (seq_t)21;  /* a(5)  */
        cache[5] = (seq_t)33;  /* a(6)  */
        cache[6] = (seq_t)53;  /* a(7)  */
        cache[7] = (seq_t)77;  /* a(8)  */
        cache[8] = (seq_t)111; /* a(9)  */
    }
#endif

    /*-----------------------------------------------------------*/
    /* 2.  Return result if it is already computed              */
    /*-----------------------------------------------------------*/
#ifdef __cplusplus
    if (n <= (idx_t)cache_vec.size())
        return cache_vec[(size_t)(n - 1)];
#else
    if (n <= cache_len)
        return cache[(size_t)(n - 1)];
#endif

    /*-----------------------------------------------------------*/
    /* 3.  Extend cache up to n, computing new terms via recurrence */
    /*-----------------------------------------------------------*/
#ifdef __cplusplus
    const idx_t old_size = (idx_t)cache_vec.size();
    cache_vec.resize(n);
    for (idx_t i = old_size; i < n; ++i) {          /* i is 0‑based, i = n-1 */
        /* Recurrence is only needed for i >= 9  (n >= 10) */
        if (i >= 9) {
            cache_vec[(size_t)i] =
                (seq_t)2 * cache_vec[(size_t)(i - 1)]   /* 2*a(n-1) */
                - cache_vec[(size_t)(i - 4)]             /* -a(n-4) */
                - cache_vec[(size_t)(i - 5)]             /* -a(n-5) */
                - cache_vec[(size_t)(i - 6)]             /* -a(n-6) */
                + cache_vec[(size_t)(i - 7)]             /* +a(n-7) */
                + cache_vec[(size_t)(i - 8)];            /* +a(n-8) */
        }
        /* for i < 9 the values were already set during initialisation */
    }
#else
    idx_t old_len = cache_len;
    cache = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
    for (idx_t i = old_len; i < n; ++i) {          /* i is 0‑based, i = n-1 */
        if (i >= 9) {
            cache[(size_t)i] =
                (seq_t)2 * cache[(size_t)(i - 1)]   /* 2*a(n-1) */
                - cache[(size_t)(i - 4)]             /* -a(n-4) */
                - cache[(size_t)(i - 5)]             /* -a(n-5) */
                - cache[(size_t)(i - 6)]             /* -a(n-6) */
                + cache[(size_t)(i - 7)]             /* +a(n-7) */
                + cache[(size_t)(i - 8)];            /* +a(n-8) */
        }
        /* values for i < 9 were already initialised */
    }
    cache_len = n;
#endif

    /*-----------------------------------------------------------*/
    /* 4.  Return the requested value                          */
    /*-----------------------------------------------------------*/
#ifdef __cplusplus
    return cache_vec[(size_t)(n - 1)];
#else
    return cache[(size_t)(n - 1)];
#endif
}
