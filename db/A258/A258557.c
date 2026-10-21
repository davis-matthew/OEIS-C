/*
 * A258557  Number of (4+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A258557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4)
                  - 3*a(n-5) + 3*a(n-6) - a(n-7)   (for n > 11) */
/* Initial values: a(1)=228, a(2)=652, a(3)=1244, a(4)=1891, a(5)=2509,
   a(6)=3314, a(7)=4313, a(8)=5356, a(9)=6331, a(10)=7470, a(11)=8820 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External integer types provided by the build system */
typedef uint64_t idx_t;             /* unsigned 64‑bit index */

/* Sequence function implementation */
seq_t a(idx_t n)
{
    /* Initial values for indices 1..11 */
    static const seq_t init[12] = {
        0,          /* index 0 unused */
        228,        /* a(1) */
        652,        /* a(2) */
        1244,       /* a(3) */
        1891,       /* a(4) */
        2509,       /* a(5) */
        3314,       /* a(6) */
        4313,       /* a(7) */
        5356,       /* a(8) */
        6331,       /* a(9) */
        7470,       /* a(10) */
        8820        /* a(11) */
    };

    /* Static cache to store computed terms */
    static seq_t *cache = NULL;   /* allocated array, index 0 unused */
    static idx_t max_n  = 0;      /* highest index computed so far */

    /* Grow cache if necessary */
    if (n > max_n) {
        /* Determine new size (at least 12 to hold initial values) */
        idx_t new_size = max_n ? max_n * 2 : 16;
        while (new_size < n + 1) new_size <<= 1;
        if (new_size < 12) new_size = 12;

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;

        /* Initialize initial values if they were not present before */
        for (idx_t i = 1; i <= 11 && i <= n; ++i) {
            if (i > max_n) cache[i] = init[i];
        }

        /* Compute new terms using the recurrence */
        idx_t start = max_n + 1;
        if (start < 12) start = 12;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] =
                (seq_t)3 * cache[i-1] - (seq_t)3 * cache[i-2] + cache[i-3] +
                cache[i-4] - (seq_t)3 * cache[i-5] + (seq_t)3 * cache[i-6] -
                cache[i-7];
        }
        max_n = n;
    }

    return cache[n];
}
