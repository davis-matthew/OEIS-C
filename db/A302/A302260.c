/*
 * A302260  Number of n X 3 0..1 arrays with every element equal to 0, 1, 2 or
 *          3 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 2*a(n-2) + 2*a(n-3) - 54*a(n-4) + 16*a(n-5)
 *            for n>6 Initial values: a(1)=4, a(2)=29, a(3)=153, a(4)=818,
 *            a(5)=4386, a(6)=23516
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0)
        return 0;   /* undefined for n<1, return 0 as a safe value */

    /* initialise cache with first 6 terms (indices 1..6) */
    if (cache_len == 0) {
        size_t init = 7;          /* allocate indices 0..6, cache[0] unused */
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        cache[0] = (seq_t)0;      /* placeholder for 0 */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)29;
        cache[3] = (seq_t)153;
        cache[4] = (seq_t)818;
        cache[5] = (seq_t)4386;
        cache[6] = (seq_t)23516;
        cache_len = init;
    }

    /* extend cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            /* compute a(i) via the recurrence */
            seq_t part1 = 6 * cache[i - 1];
            part1 += 2 * cache[i - 3];
            part1 += 16 * cache[i - 5];

            seq_t part2 = 2 * cache[i - 2];
            part2 += 54 * cache[i - 4];

            cache[i] = part1 - part2;
        }
        cache_len = new_len;
    }

    return cache[n];
}
