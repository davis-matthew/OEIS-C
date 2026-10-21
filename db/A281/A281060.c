/*
 * A281060  Number of 5Xn 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 28, u64 n <= 71, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A281060 – Number of 5×n 0..1 arrays with the given constraints.
    Empirical linear recurrence (valid for n > 18):
        a(n) = 6*a(n-1) -12*a(n-2) +5*a(n-3) +12*a(n-4)
               -12*a(n-5) -3*a(n-6) +6*a(n-7) - a(n-9)
    Initial values (n = 1 … 18):
        a(1)=0 , a(2)=122 , a(3)=1492 , a(4)=3416 , a(5)=7803 ,
        a(6)=16875 , a(7)=32370 , a(8)=59669 , a(9)=107693 ,
        a(10)=191953 , a(11)=339219 , a(12)=595824 , a(13)=1041681 ,
        a(14)=1814253 , a(15)=3149435 , a(16)=5451162 , a(17)=9409735 ,
        a(18)=16202663
*/

#include <stdlib.h>

/* Static cache for memoised values: index 0 is unused. */
static seq_t *cache = NULL;   /* points to array of size cache_size+1 */
static idx_t cache_size = 0;  /* highest index stored in cache */

/* Pre‑stored initial 18 values, index 0 is a dummy to keep indices
   1‑based. */
static const seq_t init_vals[19] = {
    0,          /* dummy for index 0; unused */
    0, 122, 1492, 3416, 7803, 16875, 32370,
    59669, 107693, 191953, 339219, 595824,
    1041681, 1814253, 3149435, 5451162,
    9409735, 16202663
};

/* Compute the sequence value a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* OEIS offset is 1; values only defined for n>=1 */

    /* Initialise cache on the first call. */
    if (cache == NULL) {
        /* Allocate space for indices 0 … 18 (size 19). */
        cache_size = 18;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failed */
        for (idx_t i = 1; i <= cache_size; ++i)
            cache[i] = init_vals[i];
    }

    /* If value is already cached, return it. */
    if (n <= cache_size) return cache[n];

    /* Grow the cache to hold indices 0 … n. */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (tmp == NULL) return 0;  /* allocation failed */
    cache = tmp;

    /* Compute new terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 18) {
            /* Safety check – should not happen, but keep values from table. */
            cache[i] = init_vals[i];
        } else {
            /*  a(i) = 6*a(i-1) -12*a(i-2) +5*a(i-3) +12*a(i-4)
                    -12*a(i-5) -3*a(i-6) +6*a(i-7) - a(i-9) */
            seq_t term = cache[i-1] * 6
                       + cache[i-3] * 5
                       + cache[i-4] * 12
                       + cache[i-7] * 6;
            seq_t sub  = cache[i-2] * 12
                       + cache[i-5] * 12
                       + cache[i-6] * 3
                       + cache[i-9];
            cache[i] = term - sub;
        }
    }
    cache_size = n;
    return cache[n];
}
