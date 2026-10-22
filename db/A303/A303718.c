/*
 * A303718  Number of n X 7 0..1 arrays with every element unequal to 0, 1 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 55, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) - a(n-5) - a(n-6)
   for n > 9, with the initial values
   a(1)=21, a(2)=23, a(3)=40, a(4)=161, a(5)=361,
   a(6)=741, a(7)=1592, a(8)=3469, a(9)=7416 */
#include <stdlib.h>

/* memoised cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;        /* allocated number of slots */
static size_t cache_computed = 0;        /* number of terms currently stored (>= 9 after first init) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0 */

    /* Ensure cache space is large enough for n terms */
    if (cache == NULL) {
        cache_capacity = (n < 16) ? 16 : n;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
    } else if ((size_t)n > cache_capacity) {
        cache_capacity = (size_t)n;
        cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
    }

    /* Initialise the first nine values on first use */
    if (cache_computed == 0) {
        cache[0] = (seq_t)21;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)40;
        cache[3] = (seq_t)161;
        cache[4] = (seq_t)361;
        cache[5] = (seq_t)741;
        cache[6] = (seq_t)1592;
        cache[7] = (seq_t)3469;
        cache[8] = (seq_t)7416;
        cache_computed = 9;
    }

    /* Return cached value if already computed */
    if (n <= (idx_t)cache_computed) return cache[n - 1];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)cache_computed; i < n; ++i) {
        /* i refers to a(i+1) in OEIS indexing (1‑based) */
        seq_t val = cache[i - 1] + cache[i - 2]                /* a(i) + a(i-1)   */
                  + cache[i - 3] * (seq_t)3                    /* + 3*a(i-2)      */
                  + cache[i - 4]                               /* + a(i-3)        */
                  - cache[i - 5]                               /* - a(i-4)        */
                  - cache[i - 6];                              /* - a(i-5)        */
        cache[i] = val;
    }
    cache_computed = (size_t)n;
    return cache[n - 1];
}
