/*
 * A385339  Consecutive states of the linear congruential pseudo-random number
 *          generator (1255*s + 6173) mod 29282 when started at s=1.
 * https://oeis.org/A385339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (1255 * a(n-1) + 6173) mod 29282 for n > 1
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PERIOD 29282u

seq_t a(idx_t n) {
    /* Reduce the index modulo the period, preserving 1‑based indexing. */
    if (n > PERIOD)
        n = ((n - 1) % PERIOD) + 1;

    /* Lazy initialization and memoisation of the sequence values. */
    static bool init = false;
    static idx_t computed_up_to = 0;
    static seq_t cache[PERIOD + 1];

    if (!init) {
        cache[1] = (seq_t)1;   /* a(1) = 1 */
        computed_up_to = 1;
        init = true;
    }

    if (n <= computed_up_to)
        return cache[n];

    /* Compute missing values up to the requested index. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = (seq_t)(( (uint64_t)1255 * (uint64_t)prev + 6173 ) % 29282);
    }
    computed_up_to = n;
    return cache[n];
}
