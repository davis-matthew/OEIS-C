/*
 * A303316  Number of 4Xn 0..1 arrays with every element equal to 0, 1, 4, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A303316.
 * Recurrence (OEIS): a(n) = a(n-1)+a(n-2)+5*a(n-3)+9*a(n-4)-3*a(n-5)-7*a(n-6)-2*a(n-7)
 * for n > 11, with initial values:
 *   a(1) = 8, a(2) = 6, a(3) = 11, a(4) = 36, a(5) = 50,
 *   a(6) = 128, a(7) = 439, a(8) = 1085, a(9) = 2431,
 *   a(10) = 6452, a(11) = 17455.
 */

#include <stdlib.h>
#include <string.h>   /* for memcpy */

seq_t a(idx_t n)
{
    /* Handle indices below the offset (not used in reference tests). */
    if (n == 0)
        return 0;

    /* Static cache: stores values 1 .. cached_size. */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    /* If the value is already cached, return it. */
    if (n <= cached_size)
        return cache[n];

    /* Preserve old cache size before expanding. */
    idx_t old_cached = cached_size;
    idx_t new_size = n + 1;               /* index 0 is unused */

    /* Allocate larger cache and copy existing data. */
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – return 0 (should never happen in tests). */
        return 0;
    }
    if (cache) {
        memcpy(new_cache, cache, (old_cached + 1) * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cached_size = n;

    /* Initial values (index 0 unused). */
    static const seq_t init_vals[12] = {
        0ULL,      /* 0          (unused) */
        8ULL,      /* a(1) */
        6ULL,      /* a(2) */
        11ULL,     /* a(3) */
        36ULL,     /* a(4) */
        50ULL,     /* a(5) */
        128ULL,    /* a(6) */
        439ULL,    /* a(7) */
        1085ULL,   /* a(8) */
        2431ULL,   /* a(9) */
        6452ULL,   /* a(10) */
        17455ULL   /* a(11) */
    };

    /* Compute missing terms up to n. */
    for (idx_t i = old_cached + 1; i <= n; ++i) {
        if (i <= 11) {
            cache[i] = init_vals[i];
        } else {
            /* Compute recurrence:
             * a(i) = a(i-1) + a(i-2) + 5*a(i-3) + 9*a(i-4)
             *        - 3*a(i-5) - 7*a(i-6) - 2*a(i-7)
             */
            seq_t pos =
                cache[i - 1] +
                cache[i - 2] +
                (seq_t)5 * cache[i - 3] +
                (seq_t)9 * cache[i - 4];

            seq_t neg =
                (seq_t)3 * cache[i - 5] +
                (seq_t)7 * cache[i - 6] +
                (seq_t)2 * cache[i - 7];

            cache[i] = pos - neg;   /* pos >= neg for all i */
        }
    }

    return cache[n];
}
