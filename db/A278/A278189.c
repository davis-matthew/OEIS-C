/*
 * A278189  Number of 2 X n 0..3 arrays with every element plus 1 mod 4 equal
 *          to some element at offset (-1,-1) (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 16*a(n-2) + 9*a(n-3) - 12*a(n-4) + 5*a(n-5) *
 *            + 7*a(n-6) - 4*a(n-7) for n > 7 * Initial values (a(1) … a(7)):
 *            * a(1)=0, a(2)=3, a(3)=19, a(4)=136, a(5)=935, * a(6)=6381,
 *            a(7)=43478
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h> /* for memcpy */

/* static cache for memoization */
static seq_t *cache = NULL;      /* cache[0] unused, cache[1] = a(1) */
static size_t cache_used = 0;    /* largest index that has been computed */
static size_t cache_cap  = 0;    /* allocated size (>= cache_used+1) */

/* Grow the cache array to hold at least 'needed' elements (index 0..needed-1) */
static void ensure_capacity(size_t needed)
{
    if (!cache) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = needed;
        cache_used = 0;   /* nothing computed yet */
    } else if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = needed;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset is 1; values below 1 are irrelevant */

    /* Ensure cache is large enough for index n (plus the unused 0 entry) */
    ensure_capacity((size_t)n + 1);

    /* Compute any missing values up to n */
    for (size_t i = cache_used + 1; i <= (size_t)n; ++i) {
        seq_t val;
        /* Initial terms (handle n <= 7 explicitly) */
        if (i == 1)       val = (seq_t)0;
        else if (i == 2)  val = (seq_t)3;
        else if (i == 3)  val = (seq_t)19;
        else if (i == 4)  val = (seq_t)136;
        else if (i == 5)  val = (seq_t)935;
        else if (i == 6)  val = (seq_t)6381;
        else if (i == 7)  val = (seq_t)43478;
        else {
            /* recurrence for i > 7 */
            val =   (seq_t)9  * cache[i-1]
                  - (seq_t)16 * cache[i-2]
                  + (seq_t)9  * cache[i-3]
                  - (seq_t)12 * cache[i-4]
                  + (seq_t)5  * cache[i-5]
                  + (seq_t)7  * cache[i-6]
                  - (seq_t)4  * cache[i-7];
        }
        cache[i] = val;
    }
    cache_used = (size_t)n;
    return cache[(size_t)n];
}
