/*
 * A278060  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 8; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) for n > 8.
 * https://oeis.org/A278060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n for 1 <= n <= 8; a(n) = a(n-a(n-1)) + a(n-a(n-2)) for n
 *            > 8
 * Range:     u8 n <= 287, u16 n <= 420, u32 n <= 420, u64 n <= 420,
 *            u128 n <= 420, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t cache[421];   /* indices 0..420; 0 unused */
    if (!init) {
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = (seq_t)i;
        init = true;
    }

    if (n <= 8)
        return cache[n];

    if (n > 420)   /* sequence defined only up to 420 terms */
        return 0;

    if (cache[n] != 0)
        return cache[n];

    /* compute values up to n iteratively; all needed indices < n are already known */
    for (idx_t i = 9; i <= n; ++i) {
        if (cache[i] != 0)
            continue;   /* skip already computed entries */

        seq_t a_im1 = cache[i - 1];
        seq_t a_im2 = cache[i - 2];

        idx_t idx1 = (idx_t)i - (idx_t)a_im1;
        idx_t idx2 = (idx_t)i - (idx_t)a_im2;

        cache[i] = cache[idx1] + cache[idx2];
    }

    return cache[n];
}
