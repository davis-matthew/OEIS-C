/*
 * A259291  Number of (n+1) X (2+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0001 0101 or 0111.
 * https://oeis.org/A259291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) - 8*a(n-3) for n ≥ 4 Initial values:
 *            a(1) = 26, a(2) = 94, a(3) = 322
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)94;
    if (n == 3) return (seq_t)322;

    /* static cache for memoisation; index 0 unused, indices start at 1 */
    static seq_t *cache = NULL;   /* cache[i] holds a(i) for i≥1 */
    static idx_t max_n = 0;       /* largest n for which cache is valid   */

    /* Initialise cache if this is the first call */
    if (!cache) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));      /* indices 0..3 */
        if (!cache) exit(1);                             /* allocation check */
        cache[0] = (seq_t)0;        /* unused */
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)94;
        cache[3] = (seq_t)322;
        max_n   = 3;
    }

    /* Extend cache if needed */
    if (n > max_n) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;

        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t t1 = (seq_t)3 * cache[i - 1];
            seq_t t2 = (seq_t)4 * cache[i - 2];
            seq_t t3 = (seq_t)8 * cache[i - 3];
            cache[i] = t1 + t2 - t3;
        }
        max_n = n;
    }

    return cache[n];
}
