/*
 * A330011  Number of length-n strings w over a 4-letter alphabet with the
 *          property that if x is a subword of w and |x| >= 2, then x reversed
 *          is not a subword of w.
 * https://oeis.org/A330011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5 a(n-3) - 3 a(n-4) - 2 a(n-5) - 8 a(n-6) +
 *            a(n-7) + 6 a(n-8) + 5 a(n-9) + 2 a(n-10) - 4 a(n-11) - 2 a(n-12)
 *            for n >= 17 with initial values: a(1)=1, a(2)=4, a(3)=12,
 *            a(4)=24, a(5)=48, a(6)=96, a(7)=168, a(8)=264, a(9)=456,
 *            a(10)=720, a(11)=1056, a(12)=1656, a(13)=2520, a(14)=3600...
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static idx_t max_index = 0;

    /* initialise cache with the 16 known terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * (16 + 1));
        if (cache == NULL) return 0; /* allocation failed */
        cache[0] = 0;               /* unused */
        cache[1] = 1;  /* a(1) */
        cache[2] = 4;  /* a(2) */
        cache[3] = 12; /* a(3) */
        cache[4] = 24; /* a(4) */
        cache[5] = 48; /* a(5) */
        cache[6] = 96; /* a(6) */
        cache[7] = 168;/* a(7) */
        cache[8] = 264;/* a(8) */
        cache[9] = 456;/* a(9) */
        cache[10] = 720;/* a(10) */
        cache[11] = 1056;/* a(11) */
        cache[12] = 1656;/* a(12) */
        cache[13] = 2520;/* a(13) */
        cache[14] = 3600;/* a(14) */
        cache[15] = 5352;/* a(15) */
        cache[16] = 7944;/* a(16) */
        max_index = 16;
    }

    if (n <= max_index)
        return cache[n];

    /* grow cache to hold the requested index */
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
    if (new_cache == NULL)
        return 0; /* allocation failed */
    cache = new_cache;

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        seq_t val = cache[i-1];
        val += (seq_t)5 * cache[i-3];
        val -= (seq_t)3 * cache[i-4];
        val -= (seq_t)2 * cache[i-5];
        val -= (seq_t)8 * cache[i-6];
        val += cache[i-7];
        val += (seq_t)6 * cache[i-8];
        val += (seq_t)5 * cache[i-9];
        val += (seq_t)2 * cache[i-10];
        val -= (seq_t)4 * cache[i-11];
        val -= (seq_t)2 * cache[i-12];
        cache[i] = val;
    }

    max_index = n;
    return cache[n];
}
