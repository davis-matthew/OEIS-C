/*
 * A376730  Expansion of (1 - x^3 - x^4)/((1 - x^3 - x^4)^2 - 4*x^7).
 * https://oeis.org/A376730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-3) + 2*a(n-4) - a(n-6) + 2*a(n-7) - a(n-8)
 * Range:     u8 n <= 19, u16 n <= 33, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=0, a(6)=1,
   a(7)=6, a(8)=1 */

#include <stdlib.h>   /* for exit */

#define MAX_CACHE 200000   /* sufficient for n up to ~10000 */

/* Static cache for memoisation */
static seq_t cache[MAX_CACHE];
static idx_t cached_size = 0;

/* Base values for n = 0 .. 8 */
static const seq_t base_vals[9] = {
    (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)1,
    (seq_t)0, (seq_t)1, (seq_t)6, (seq_t)1
};

seq_t a(idx_t n) {
    /* Allocate or extend cache */
    if (n >= MAX_CACHE) {
        /* n too large for the static cache – abort */
        exit(1);
    }

    /* Initialise cache up to index 8 if not done yet */
    if (cached_size == 0) {
        for (idx_t i = 0; i <= 8; ++i)
            cache[i] = base_vals[i];
        cached_size = 9;
    }

    /* Compute any missing values */
    if (n >= cached_size) {
        for (idx_t i = cached_size; i <= n; ++i) {
            /* Use the recurrence for i >= 9, but for i == 9...8 we have base already */
            if (i <= 8) {
                /* already initialised */
                continue;
            }
            cache[i] = 2 * cache[i - 3]
                     + 2 * cache[i - 4]
                     - cache[i - 6]
                     + 2 * cache[i - 7]
                     - cache[i - 8];
        }
        cached_size = n + 1;
    }

    return cache[n];
}
