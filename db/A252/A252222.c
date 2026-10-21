/*
 * A252222  Number of (n+2) X (2+2) 0..2 arrays with every 3 X 3 subblock row
 *          and column sum 2 3 or 4 and every diagonal and antidiagonal sum
 *          not 2 3 or 4.
 * https://oeis.org/A252222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 3*a(n-1) - a(n-2) + 3*a(n-3) +
 *            3*a(n-4) - a(n-5) - a(n-6) , n > 9 Initial values: a(1)=368,
 *            a(2)=1028, a(3)=3066, a(4)=7724, a(5)=22256, a(6)=70284,
 *            a(7)=219824, a(8)=675264, a(9)=2072804
 * Range:     u8 none, u16 n <= 5, u32 n <= 15, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* initial terms */
static const seq_t init_vals[9] = {
    (seq_t)368, (seq_t)1028, (seq_t)3066, (seq_t)7724, (seq_t)22256,
    (seq_t)70284, (seq_t)219824, (seq_t)675264, (seq_t)2072804
};

/* grow cache so that it can hold terms up to 'n' (1‑based) */
static void grow_cache(size_t n)
{
    if (n <= cache_size) return;

    size_t new_size = n;
    seq_t *new_cache;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
    } else {
        new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    }
    /* If realloc fails, keep old cache to avoid loss of data - but
       for this problem we assume success. */
    cache = new_cache;
    /* compute missing values */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i < 9) {
            cache[i] = init_vals[i];
        } else {
            /* recurrence: a[i+1] = 3*a[i] - a[i-1] + 3*a[i-2] + 3*a[i-3]
                             - a[i-4] - a[i-5]   (i is zero‑based, so
                             a[i] corresponds to a(i+1) ) */
            seq_t v = ((seq_t)3 * cache[i-1])                /* 3*a(n-1) */
                      - cache[i-2]                            /* -a(n-2)  */
                      + ((seq_t)3 * cache[i-3])               /* +3*a(n-3)*/
                      + ((seq_t)3 * cache[i-4])               /* +3*a(n-4)*/
                      - cache[i-5]                            /* -a(n-5)  */
                      - cache[i-6];                           /* -a(n-6)  */
            cache[i] = v;
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                  /* undefined for n<1, return 0 */
    size_t idx = (size_t)n - 1;                   /* zero‑based index */
    if (idx + 1 > cache_size) grow_cache(idx + 1);
    return cache[idx];
}
