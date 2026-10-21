/*
 * A278151  Number of n X 2 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) (1,-1) or (1,0), with upper left element zero.
 * https://oeis.org/A278151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + a(n-3) for n>4 with a(1)=0, a(2)=2,
 *            a(3)=7, a(4)=22
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types provided by the build. */
seq_t a(idx_t n)
{
    /* Cache the values we have already computed.  Array indices are
       1‑based (0 is unused). */
    static seq_t *cache       = NULL;
    static idx_t cache_len    = 0;   /* allocated max index (≥0)   */
    static idx_t cache_filled = 0;   /* highest index computed    */

    /* Ensure we have room for index n. */
    if (n > cache_len) {
        idx_t new_len = cache_len ? cache_len * 2 : 4;
        if (new_len < n) new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }

    /* Initialise the first four terms if necessary. */
    if (cache_filled < 1) {
        cache[1] = (seq_t)0;
        cache_filled = 1;
    }
    if (cache_filled < 2) {
        cache[2] = (seq_t)2;
        cache_filled = 2;
    }
    if (cache_filled < 3) {
        cache[3] = (seq_t)7;
        cache_filled = 3;
    }
    if (cache_filled < 4) {
        cache[4] = (seq_t)22;
        cache_filled = 4;
    }

    /* Compute missing values up to n. */
    for (idx_t i = cache_filled + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        cache[i] = (seq_t)3 * a1 + a2 + a3;
    }
    cache_filled = n;

    return cache[n];
}
