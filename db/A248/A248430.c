/*
 * A248430  Number of length n+2 0..5 arrays with every three consecutive
 *          terms having the sum of some two elements equal to twice the
 *          third.
 * https://oeis.org/A248430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-3) - 6*a(n-4) - 4*a(n-6) + 4*a(n-7) (n≥8)
 *            Initial values: a(1)=42, a(2)=70, a(3)=118, a(4)=198, a(5)=342,
 *            a(6)=590, a(7)=1014
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 34, u64 n <= 74, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[i] holds a(i+1) */
static idx_t cache_size = 0;    /* number of entries in cache */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1, return 0 */

    /* Extend cache if necessary */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure */
        cache = new_cache;

        /* Fill new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) cache[i] = (seq_t)42;          /* a(1) */
            else if (i == 1) cache[i] = (seq_t)70;     /* a(2) */
            else if (i == 2) cache[i] = (seq_t)118;    /* a(3) */
            else if (i == 3) cache[i] = (seq_t)198;    /* a(4) */
            else if (i == 4) cache[i] = (seq_t)342;    /* a(5) */
            else if (i == 5) cache[i] = (seq_t)590;    /* a(6) */
            else if (i == 6) cache[i] = (seq_t)1014;   /* a(7) */
            else {
                /* recurrence: i corresponds to n = i+1 */
                seq_t term =
                    cache[i-1]          /* a(n-1) */
                  + 6 * cache[i-3]      /* +6*a(n-3) */
                  - 6 * cache[i-4]      /* -6*a(n-4) */
                  - 4 * cache[i-6]      /* -4*a(n-6) */
                  + 4 * cache[i-7];     /* +4*a(n-7) */
                cache[i] = term;
            }
        }
        cache_size = new_size;
    }
    return cache[n-1];
}
