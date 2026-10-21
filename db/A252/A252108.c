/*
 * A252108  Number of (n+2) X (2+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum equal to 0 2 3 6 or 7 and every 3 X 3 diagonal and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - 8*a(n-6) for n>13; Initial values: a(1)=1929,
 *            a(2)=1424, a(3)=724, a(4)=584, a(5)=624, a(6)=788, a(7)=1180,
 *            a(8)=1668, a(9)=2472, a(10)=3888, a(11)=5640, a(12)=8592,
 *            a(13)=13920 This implementation stores computed values and
 *            extends them as needed, while keeping memory usage mod...
 * Range:     u8 none, u16 n <= 16, u32 n <= 40, u64 n <= 88, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

extern seq_t a(idx_t n) {
    /* Static cache for computed terms.  Index 0 is unused. */
    static seq_t cache[20005];
    static idx_t cache_max = 0;  /* number of terms currently stored */

    /* Initialize the cache with the first 13 terms on the first call. */
    if (cache_max == 0) {
        cache[1]  = (seq_t)1929;
        cache[2]  = (seq_t)1424;
        cache[3]  = (seq_t)724;
        cache[4]  = (seq_t)584;
        cache[5]  = (seq_t)624;
        cache[6]  = (seq_t)788;
        cache[7]  = (seq_t)1180;
        cache[8]  = (seq_t)1668;
        cache[9]  = (seq_t)2472;
        cache[10] = (seq_t)3888;
        cache[11] = (seq_t)5640;
        cache[12] = (seq_t)8592;
        cache[13] = (seq_t)13920;
        cache_max = 13;
    }

    /* If the requested term is already computed, return it. */
    if (n <= cache_max) {
        return cache[n];
    }

    /* For indices up to 20004 we extend the static cache. */
    if (n <= 20004) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* a(i) = 6*a(i-3) - 8*a(i-6) */
            cache[i] = (cache[i-3] * (seq_t)6) -
                       (cache[i-6] * (seq_t)8);
        }
        cache_max = n;
        return cache[n];
    }

    /* For very large n we avoid allocating a huge array.
       We use a sliding window of the last six terms. */
    {
        /* Load the last six terms from the cache (must exist because
           cache_max >= 13). */
        seq_t last[6];
        for (idx_t k = 0; k < 6; ++k) {
            last[k] = cache[cache_max - 5 + k];
        }

        /* Compute forward from cache_max+1 up to n. */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* last[0] = a(i-6), last[3] = a(i-3) */
            seq_t next = (last[3] * (seq_t)6) - (last[0] * (seq_t)8);
            /* Shift the window */
            for (idx_t k = 0; k < 5; ++k) last[k] = last[k+1];
            last[5] = next;
        }
        return last[5];
    }
}
