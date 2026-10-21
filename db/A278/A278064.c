/*
 * A278064  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 12; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) for n > 12.
 * https://oeis.org/A278064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n for 1 <= n <= 12; a(n) = a(n-a(n-1)) + a(n-a(n-2)) for
 *            n > 12
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache for the 69 defined terms (indices 0..69, 0 unused). */
    enum { MAX_N = 69 };
    static seq_t cache[MAX_N + 1];
    static bool init = false;

    if (!init) {
        /* Initial values 1..12 */
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = (seq_t)i;

        /* Compute terms 13..69 by the recursion. */
        for (idx_t i = 13; i <= MAX_N; ++i) {
            idx_t j1 = i - (idx_t)cache[i - 1];
            idx_t j2 = i - (idx_t)cache[i - 2];
            if (j1 < 1 || j2 < 1)
                cache[i] = (seq_t)0;               /* undefined outside the range */
            else
                cache[i] = cache[j1] + cache[j2];
        }
        init = true;
    }

    if (n > MAX_N)               /* sequence is defined only up to 69 */
        return (seq_t)0;

    return cache[n];
}
