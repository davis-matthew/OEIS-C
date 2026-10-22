/*
 * A375016  Number of 1-unbordered words of length n over a 3-letter alphabet
 *          beginning with a fixed letter.
 * https://oeis.org/A375016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 1 * a(2*n+1) = 3 * a(2*n) for n > 0 * a(2*n) = 3 *
 *            a(2*n-1) - 2*a(n) for n > 0 * * This function computes a(n) for
 *            n ≥ 1 by memoised recurrence. * It allocates a cache dynamically
 *            in a static array and fills it * up to the requested index if
 *            necessary. * The implementation is compatible...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* cache[0] holds a(1) */
    static idx_t cache_size = 0;    /* number of entries stored */

    if (n == 0) return 0;           /* undefined for offset 1, but safe-guard */

    /* Allocate or expand cache up to index n (1-based) */
    if (n > cache_size) {
        size_t new_bytes = (size_t)n * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, new_bytes);
        if (tmp == NULL) {          /* allocation failed */
            /* In a real program we would handle this, but for sequence
               computation we can simply abort or return zero. */
            return 0;
        }
        cache = tmp;

        /* Compute missing values in increasing order */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else if (i & 1U) {                 /* odd i: i = 2*k+1 */
                idx_t k = (i - 1) / 2;
                val = (seq_t)3 * cache[2 * k - 1];
            } else {                              /* even i: i = 2*k */
                idx_t k = i / 2;
                val = (seq_t)3 * cache[i - 2] - (seq_t)2 * cache[k - 1];
            }
            cache[i - 1] = val;
        }
        cache_size = n;
    }
    return cache[n - 1];
}
