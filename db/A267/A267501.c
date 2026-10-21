/*
 * A267501  Relative of Hofstadter Q-sequence: a(n) = 0 if n <= 0, a(1) = a(2)
 *          = a(3) = 1; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n >
 *          3.
 * https://oeis.org/A267501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 0 if n <= 0; a(1) = a(2) = a(3) = 1; a(n) = a(n-a(n-1)) +
 *            a(n-a(n-2)) + a(n-a(n-3)) for n > 3 Sequence terminates at n =
 *            416 with a(416) = 0
 * Range:     u8 n <= 131, u16 n <= 416, u32 n <= 416, u64 n <= 416,
 *            u128 n <= 416, big any n
 * Helpers:   safe_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/safe_sub.h"

static seq_t cache[417];      /* zero‑initialized */
static bool ready = false;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n > 416) return (seq_t)0;          /* undefined beyond 416 */

    if (!ready) {
        /* initialise base values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;

        for (idx_t i = 4; i <= 416; ++i) {
            idx_t idx1 = safe_sub(i, cache[i-1]);
            idx_t idx2 = safe_sub(i, cache[i-2]);
            idx_t idx3 = safe_sub(i, cache[i-3]);

            cache[i] = cache[idx1] + cache[idx2] + cache[idx3];
        }
        ready = true;
    }
    return cache[n];
}
