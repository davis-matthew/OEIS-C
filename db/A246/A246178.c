/*
 * A246178  Expansion of 1/(1 - 3*x + x^2)^3.
 * https://oeis.org/A246178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 30*a(n-2) + 45*a(n-3) - 30*a(n-4) + 9*a(n-5) -
 *            a(n-6) for n ≥ 6, with initial values a(0)=1, a(1)=9, a(2)=51,
 *            a(3)=234, a(4)=951, a(5)=3573
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the first six terms directly */
    if (n == 0) return 1;
    if (n == 1) return 9;
    if (n == 2) return 51;
    if (n == 3) return 234;
    if (n == 4) return 951;
    if (n == 5) return 3573;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t last = SIZE_MAX;   /* index of the largest computed term */

    /* Convert idx_t to size_t safely – n is ≤ 10000 in tests */
    size_t idx = (size_t)n;
    size_t needed = idx + 1;

    /* Initialise cache on first use */
    if (cache == NULL) {
        if (needed < 6) needed = 6;          /* ensure room for initial terms */
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 9;
        cache[2] = 51;
        cache[3] = 234;
        cache[4] = 951;
        cache[5] = 3573;
        last = 5;
    }

    /* Expand cache if necessary */
    if (needed > last + 1) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    }

    /* Compute new terms up to idx */
    for (size_t i = last + 1; i <= idx; ++i) {
        seq_t pos = 9 * cache[i - 1] + 45 * cache[i - 3] + 9 * cache[i - 5];
        seq_t sub = 30 * cache[i - 2] + 30 * cache[i - 4] + cache[i - 6];
        cache[i] = pos - sub;
    }
    last = idx;

    return cache[idx];
}
