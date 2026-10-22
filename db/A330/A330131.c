/*
 * A330131  Number of length-n binary words with no even palindrome of length
 *          > 6 and no odd palindrome of length > 3.
 * https://oeis.org/A330131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) + 2*a(n-8) + 3*a(n-10) + a(n-14) for n >= 21 a(0)
 *            = 1, a(1) = 2, a(2) = 4, a(3) = 8, a(4) = 16, a(5) = 24, a(6) =
 *            36, a(7) = 52, a(8) = 66, a(9) = 84, a(10) = 104, a(11) = 128,
 *            a(12) = 160, a(13) = 200, a(14) = 248, a(15) = 308, a(16) = 384,
 *            a(17) = 476, a(18) = 592, a(19) = 740, a(...
 * Range:     u8 n <= 14, u16 n <= 39, u32 n <= 90, u64 n <= 191,
 *            u128 n <= 394, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Clear cache on first call, build values on demand, keep them memoized. */
seq_t a(idx_t n) {
    /* Initial values for 0..20.  We store them once. */
    static const seq_t init[21] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8, (seq_t)16,
        (seq_t)24, (seq_t)36, (seq_t)52, (seq_t)66, (seq_t)84,
        (seq_t)104, (seq_t)128, (seq_t)160, (seq_t)200, (seq_t)248,
        (seq_t)308, (seq_t)384, (seq_t)476, (seq_t)592, (seq_t)740,
        (seq_t)922
    };

    /* Cache the computed terms so subsequent calls are O(1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of terms already stored */

    if (n < cache_len) {
        return cache[n];
    }

    /* Need to grow the cache up to index n. */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* If allocation fails, terminate.  The build system will not allow
           this, but we must provide a guard. */
        exit(1);
    }
    cache = new_cache;

    /* Compute new terms from cache_len up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 21) {
            val = init[(size_t)i];
        } else {
            /* a(i) = a(i-6) + 2*a(i-8) + 3*a(i-10) + a(i-14)  (for i>=21) */
            val = cache[i - 6]
                + ((seq_t)2) * cache[i - 8]
                + ((seq_t)3) * cache[i - 10]
                + cache[i - 14];
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
