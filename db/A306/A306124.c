/*
 * A306124  Number of n X 3 0..1 arrays with every element unequal to 0, 1, 3,
 *          4, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A306124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-3) - 7*a(n-4) + 4*a(n-5) - 4*a(n-6)
   - 7*a(n-8) + 9*a(n-9) + 14*a(n-10) + 11*a(n-11) - 7*a(n-12) - 6*a(n-13)
   for n > 13.
   Initial values:
     a(1)=3,  a(2)=7,  a(3)=9,  a(4)=19,  a(5)=39,
     a(6)=73, a(7)=147, a(8)=319, a(9)=681, a(10)=1451,
     a(11)=3127, a(12)=6789, a(13)=14887. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] unused; cache[i] holds a(i) for i>=1 */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of elements cached (including index 0) */

    /* 13 initial terms (1‑based) */
    static const seq_t init_terms[13] = {
        (seq_t)3,      /* a(1) */
        (seq_t)7,      /* a(2) */
        (seq_t)9,      /* a(3) */
        (seq_t)19,     /* a(4) */
        (seq_t)39,     /* a(5) */
        (seq_t)73,     /* a(6) */
        (seq_t)147,    /* a(7) */
        (seq_t)319,    /* a(8) */
        (seq_t)681,    /* a(9) */
        (seq_t)1451,   /* a(10) */
        (seq_t)3127,   /* a(11) */
        (seq_t)6789,   /* a(12) */
        (seq_t)14887   /* a(13) */
    };

    /* If we only ever query small n, no allocation is needed. */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;   /* +1 to keep index 0 unused */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failed */
        cache      = tmp;
        size_t old = cache_size;
        cache_size = new_size;

        /* initialise any newly added slots */
        for (size_t i = old; i < new_size; ++i) {
            if (i == 0) {                  /* unused slot */
                continue;
            }
            if (i <= 13) {
                cache[i] = init_terms[i - 1];
            } else {
                /* compute using recurrence */
                cache[i] =
                    (seq_t)3 * cache[i - 1]
                    - cache[i - 3]
                    - (seq_t)7 * cache[i - 4]
                    + (seq_t)4 * cache[i - 5]
                    - (seq_t)4 * cache[i - 6]
                    - (seq_t)7 * cache[i - 8]
                    + (seq_t)9 * cache[i - 9]
                    + (seq_t)14 * cache[i - 10]
                    + (seq_t)11 * cache[i - 11]
                    - (seq_t)7 * cache[i - 12]
                    - (seq_t)6 * cache[i - 13];
            }
        }
    }

    return cache[(size_t)n];
}
