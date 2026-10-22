/*
 * A304257  Number of nX2 0..1 arrays with every element unequal to 2, 3, 5 or
 *          7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+3*a(n-2)-4*a(n-3)-6*a(n-4)+4*a(n-5) (n>6)
 *            Initial values: a(1)=0, a(2)=3, a(3)=4, a(4)=9, a(5)=18, a(6)=33
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of computed values */
static seq_t *cache = NULL;       /* holds a(1)..a(cache_len) */
static idx_t cache_len = 0;       /* number of terms currently stored */

/* literal base values for a(1)..a(6) */
static const seq_t base_values[6] = {0, 3, 4, 9, 18, 33};

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* out of range guard */

    /* if we already have the requested term, return it immediately */
    if (n <= cache_len) return cache[n - 1];

    /* ensure enough space for the new value(s) */
    size_t needed = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) return 0;                /* allocation failure */
    cache = tmp;

    /* initialise base values if we haven't yet */
    if (cache_len < 6) {
        for (idx_t i = cache_len; i < 6 && i < n; ++i)
            cache[i] = base_values[i];
        cache_len = 6;
    }

    /* compute the recurrence for indices >= 6 */
    if (n > 6) {
        /* start from the first not-yet-computed index */
        idx_t i = cache_len;
        if (i < 6) i = 6;              /* safety, though cache_len>=6 above */
        for (; i < n; ++i) {
            seq_t v = 2 * cache[i - 1];
            v += 3 * cache[i - 2];
            v -= 4 * cache[i - 3];
            v -= 6 * cache[i - 4];
            v += 4 * cache[i - 5];
            cache[i] = v;
        }
        cache_len = n;
    } else {                           /* cache_len >= n already handled */
        cache_len = n;                  /* keep cache length up to date */
    }

    return cache[n - 1];
}
