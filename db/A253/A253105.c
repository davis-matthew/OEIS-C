/*
 * A253105  A253104(2^n-1).
 * https://oeis.org/A253105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 4*a(n-1) + 6*a(n-3) - 29*a(n-4) + 12*a(n-5) * +
 *            8*a(n-6) - 8*a(n-7) for n >= 7 * Initial values: * a(0)=1,
 *            a(1)=8, a(2)=32, a(3)=140, a(4)=580, a(5)=2300, a(6)=9212
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure cache can hold entries up to index n. */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        grow_seq(&cache, &cache_cap, required);
    }

    /* Initialise the first seven terms, if not done yet. */
    if (cache_len == 0) {
        if (cache_cap < 7) {
            grow_seq(&cache, &cache_cap, 7);
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)32;
        cache[3] = (seq_t)140;
        cache[4] = (seq_t)580;
        cache[5] = (seq_t)2300;
        cache[6] = (seq_t)9212;
        cache_len = 7;
    }

    /* For indices below 7, the value is already present. */
    if (n < 7) {
        return cache[(size_t)n];
    }

    /* Compute terms up to n, using the linear recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Positive part: 4*a(i-1) + 6*a(i-3) + 12*a(i-5) + 8*a(i-6) */
        seq_t pos = (seq_t)4 * cache[i - 1]
                  + (seq_t)6 * cache[i - 3]
                  + (seq_t)12 * cache[i - 5]
                  + (seq_t)8  * cache[i - 6];

        /* Negative part: 29*a(i-4) + 8*a(i-7)            */
        seq_t neg = (seq_t)29 * cache[i - 4]
                  + (seq_t)8  * cache[i - 7];

        cache[i] = pos - neg;  /* Guaranteed non‑negative for this sequence. */
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
