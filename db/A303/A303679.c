/*
 * A303679  Number of nX5 0..1 arrays with every element unequal to 0, 1 or 3
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-4) + a(n-6) for n>10 * Initial values
 *            (offset 1): * a(1)=8, a(2)=31, a(3)=25, a(4)=44, a(5)=76, *
 *            a(6)=110, a(7)=150, a(8)=212, a(9)=306, a(10)=458 * * The
 *            function uses a static cache that is extended on demand. * It
 *            works with any build where seq_t is an unsigned integer typ...
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 51, u64 n <= 109, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_values[11] = {
    (seq_t)0,   /* unused index 0 to keep indices 1‑based */
    (seq_t)8,   /* a(1) */
    (seq_t)31,  /* a(2) */
    (seq_t)25,  /* a(3) */
    (seq_t)44,  /* a(4) */
    (seq_t)76,  /* a(5) */
    (seq_t)110, /* a(6) */
    (seq_t)150, /* a(7) */
    (seq_t)212, /* a(8) */
    (seq_t)306, /* a(9) */
    (seq_t)458  /* a(10) */
};

seq_t a(idx_t n)
{
    if (n <= 10)
        return base_values[n];

    /* Lazy static cache */
    static seq_t *cache = NULL;
    static idx_t  max_cached = 0;

    /* Initialise cache with base values if needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(11 * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;           /* allocation failure */
        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = base_values[i];
        max_cached = 10;
    }

    /* Extend cache if necessary */
    if (n > max_cached) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;           /* allocation failure */
        cache = tmp;
        for (idx_t i = max_cached + 1; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 4] + cache[i - 6];
        max_cached = n;
    }

    return cache[n];
}
