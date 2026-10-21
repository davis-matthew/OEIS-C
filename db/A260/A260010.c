/*
 * A260010  Number of (n+2)X(3+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000101 00010101 or 01010101.
 * https://oeis.org/A260010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) + 37*a(n-4) - 13*a(n-6) - 124*a(n-8) +
 *            12*a(n-10) + 72*a(n-12) for n ≥ 14 Initial values: a(1)=109,
 *            a(2)=252, a(3)=889, a(4)=2492, a(5)=6944, a(6)=18356,
 *            a(7)=56137, a(8)=155304, a(9)=457083, a(10)=1237656,
 *            a(11)=3708013, a(12)=10156868, a(13)=30171658
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1u) return 0;     /* the sequence is defined for n≥1 */

    /* initial terms (indices 1…13) */
    static const seq_t init[13] = {
        109, 252, 889, 2492, 6944,
        18356, 56137, 155304, 457083, 1237656,
        3708013, 10156868, 30171658
    };

    /* static cache for memoisation */
    static seq_t *cache = NULL;     /* 1‑based index, cache[0] unused */
    static idx_t cache_len = 0;     /* current largest computed index */
    static idx_t cache_cap = 0;     /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 14;                     /* 13 terms + index 0 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 13;
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init[i - 1];
    }

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure – unlikely here */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n */
    if (n > cache_len) {
        /* start from the highest missing index */
        idx_t start = cache_len + 1;
        if (start < 14) start = 14;          /* recurrence begins at n = 14 */

        for (idx_t i = start; i <= n; ++i) {
            seq_t val = (seq_t)4 * cache[i - 2];
            val += (seq_t)37 * cache[i - 4];
            val -= (seq_t)13 * cache[i - 6];
            val -= (seq_t)124 * cache[i - 8];
            val += (seq_t)12 * cache[i - 10];
            val += (seq_t)72 * cache[i - 12];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
