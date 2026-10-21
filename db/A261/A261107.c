/*
 * A261107  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010001 00010101 or 01010101.
 * https://oeis.org/A261107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 29, u64 n <= 62, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 2*a(n-3) + 2*a(n-4) + 2*a(n-5) + a(n-6)  for n>7
   Initial values: a(1)=33, a(2)=52, a(3)=106, a(4)=206, a(5)=402, a(6)=790, a(7)=1546 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;          /* cache of computed terms */
    static size_t cache_len = 0;          /* allocated length of cache */
    static size_t computed_max = 0;       /* highest n computed so far */

    if (n < 1) return 0;                  /* undefined for n < 1 */

    /* Ensure the cache can hold terms up to n */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();               /* out‑of‑memory */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms up to n */
    if ((size_t)n > computed_max) {
        /* Initial values for n = 1 .. 7 */
        static const seq_t init_vals[8] = {
            (seq_t)0,     /* unused, keeps indices 1..7 aligned */
            (seq_t)33,
            (seq_t)52,
            (seq_t)106,
            (seq_t)206,
            (seq_t)402,
            (seq_t)790,
            (seq_t)1546
        };

        size_t i;
        for (i = computed_max + 1; i <= (size_t)n; ++i) {
            if (i <= 7) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = cache[i-1] + cache[i-6]
                         + (seq_t)2 * cache[i-3]
                         + (seq_t)2 * cache[i-4]
                         + (seq_t)2 * cache[i-5];
            }
        }
        computed_max = (size_t)n;
    }

    return cache[n];
}
