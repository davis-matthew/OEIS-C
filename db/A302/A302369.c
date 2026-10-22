/*
 * A302369  Number of 3Xn 0..1 arrays with every element equal to 0, 2, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A302369: Number of 3×n 0/1 arrays with distance‑≤1 restriction
   (OEIS A302369).  Computed by the 18‑term linear recurrence

   a(n) = 6*a(n-1) + a(n-2) – 47*a(n-3) + 13*a(n-4) + 158*a(n-5)
            – 30*a(n-6) – 242*a(n-7) – 62*a(n-8) + 224*a(n-9)
            + 26*a(n-10) + 11*a(n-11) + 64*a(n-12) – 114*a(n-13)
            – 27*a(n-14) + 26*a(n-15) – 6*a(n-16)   for n > 18

   Initial values:
   a(1)=4, a(2)=2, a(3)=31, a(4)=20, a(5)=154, a(6)=284,
   a(7)=1109, a(8)=3472, a(9)=12763, a(10)=48721, a(11)=190256,
   a(12)=770983, a(13)=3134572, a(14)=12931018, a(15)=53433431,
   a(16)=221779682, a(17)=921445395, a(18)=3833216521 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[18] = {
    4ULL,  2ULL, 31ULL, 20ULL, 154ULL, 284ULL, 1109ULL, 3472ULL,
   12763ULL, 48721ULL, 190256ULL, 770983ULL, 3134572ULL,
   12931018ULL, 53433431ULL, 221779682ULL, 921445395ULL,
   3833216521ULL
};

static seq_t *cache = NULL;     /* 1‑based array, cache[0] unused */
static size_t cache_size = 0;    /* number of elements allocated (excluding 0) */

/* Ensure that cache contains values up to index `n` (1‑based).  */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n;
    if (need <= cache_size)
        return;

    /* We always keep at least 18 elements so that the recurrence
       has enough history. */
    size_t new_size = (need > 18 ? need : 18);
    seq_t *new_cache = NULL;
    size_t old_size = cache_size;

#ifdef __cplusplus
    new_cache = new seq_t[new_size + 1];
    /* copy existing data */
    for (size_t i = 1; i <= old_size; ++i)
        new_cache[i] = cache[i];
#else
    new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
#endif
    if (!new_cache) abort();  /* allocation failed */
    cache = new_cache;
    cache_size = new_size;

    /* initialise or extend the initial 18 values */
    if (old_size == 0) {  /* first allocation */
        for (size_t i = 1; i <= 18; ++i)
            cache[i] = init_vals[i - 1];
        for (size_t i = 19; i <= new_size; ++i)
            cache[i] = 0;
    } else {
        /* copy any newly allocated slots from the previous array
           (handled already by realloc or new).  Fill or extend the
           initial segment if we crossed the boundary of 18. */
        if (old_size < 18) {
            for (size_t i = old_size + 1; i <= 18 && i <= new_size; ++i)
                cache[i] = init_vals[i - 1];
        }
    }

#ifndef __cplusplus
    /* In C we used realloc, which already preserves the contents.
       No need for a delete[].  In C++ we delete the old array
       after the new one was allocated and data copied. */
#ifdef __cplusplus
    delete[] cache;  /* the old array is freed after copying */
#endif
#endif

    /* Compute new values using the recurrence.  */
    for (size_t k = old_size + 1; k <= need; ++k) {
        if (k <= 18) {
            /* initial terms were already set */
            continue;
        }
        seq_t pos = (seq_t)6   * cache[k - 1] +
                     (seq_t)1   * cache[k - 2] +
                     (seq_t)13  * cache[k - 4] +
                     (seq_t)158 * cache[k - 5] +
                     (seq_t)224 * cache[k - 9] +
                     (seq_t)26  * cache[k -10] +
                     (seq_t)11  * cache[k -11] +
                     (seq_t)64  * cache[k -12] +
                     (seq_t)26  * cache[k -15];
        seq_t neg = (seq_t)47  * cache[k - 3] +
                     (seq_t)30  * cache[k - 6] +
                     (seq_t)242 * cache[k - 7] +
                     (seq_t)62  * cache[k - 8] +
                     (seq_t)114 * cache[k -13] +
                     (seq_t)27  * cache[k -14] +
                     (seq_t)6   * cache[k -16];
        cache[k] = pos - neg;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n < 1, guard against misuse   */
    ensure_cache(n);
    return cache[(size_t)n];
}
