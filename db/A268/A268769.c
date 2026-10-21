/*
 * A268769  Number of n X 3 0..2 arrays with some element plus some
 *          horizontally, vertically, diagonally or antidiagonally adjacent
 *          neighbor totalling two exactly once.
 * https://oeis.org/A268769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 16*a(n-3) - a(n-4) + 12*a(n-5) -
 *            4*a(n-6) for n > 8, with initial values: a(1)=12, a(2)=32,
 *            a(3)=112, a(4)=446, a(5)=1524, a(6)=5214, a(7)=17000, a(8)=54822
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _a0[9] = {
    0,          /* unused (a(0) not defined) */
    12,         /* a(1) */
    32,         /* a(2) */
    112,        /* a(3) */
    446,        /* a(4) */
    1524,       /* a(5) */
    5214,       /* a(6) */
    17000,      /* a(7) */
    54822       /* a(8) */
};

seq_t a(idx_t n)
{
    /* Return 0 for the undefined a(0); only n ≥ 1 are valid. */
    if (n == 0)
        return 0;

    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;   /* cache[0] unused, cache[i] = a(i) */
    static idx_t cache_size = 0;  /* largest index cached so far */

    /* If the requested n is beyond the current cache, enlarge it. */
    if (n > cache_size) {
        idx_t new_size = n + 1;          /* indices 0 .. n inclusive */
        seq_t *tmp;

        if (cache == NULL) {
            tmp = (seq_t*)malloc(new_size * sizeof(seq_t));
        } else {
            tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        }
        if (tmp == NULL) {
            /* Allocation failed; fall back to returning 0. */
            return 0;
        }
        cache = tmp;

        /* Compute new entries from cache_size+1 up to n. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 8) {
                cache[i] = _a0[i];
            } else {
                /* Recurrence:
                 * a(i) = 4*a(i-1) + 2*a(i-2) - 16*a(i-3) - a(i-4) + 12*a(i-5) - 4*a(i-6)
                 */
                seq_t pos = 4 * cache[i-1] + 2 * cache[i-2] + 12 * cache[i-5];
                seq_t neg = 16 * cache[i-3] + cache[i-4] + 4 * cache[i-6];
                cache[i] = pos - neg;   /* values remain non‑negative for n ≥ 9 */
            }
        }
        cache_size = n;
    }

    return cache[n];
}
