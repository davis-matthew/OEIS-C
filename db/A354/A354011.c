/*
 * A354011  Number of tilings of a 3 X n rectangle using 2 X 2 tiles, right
 *          trominoes and dominoes.
 * https://oeis.org/A354011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=0, a(2)=7, a(3)=8, a(4)=81, a(5)=184, a(6)=1051
   a(n)=2*a(n-1)+9*a(n-2)-8*a(n-3)+3*a(n-4)+6*a(n-5)-3*a(n-6)  for n≥7 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 7;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)7;   /* a(2) */
        cache[3] = (seq_t)8;   /* a(3) */
        cache[4] = (seq_t)81;  /* a(4) */
        cache[5] = (seq_t)184; /* a(5) */
        cache[6] = (seq_t)1051;/* a(6) */
        last_computed = 6;
    }

    /* Grow cache if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute terms up to n if they are not yet computed */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t t = 2 * cache[i - 1] + 9 * cache[i - 2];
            t      = t - 8 * cache[i - 3];
            t      = t + 3 * cache[i - 4];
            t      = t + 6 * cache[i - 5];
            t      = t - 3 * cache[i - 6];
            cache[i] = t;
        }
        last_computed = n;
    }

    return cache[n];
}
