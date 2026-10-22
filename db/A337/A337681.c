/*
 * A337681  Number of distinct positive integer pairs, (s,t), such that s < t
 *          < n where neither s nor t divides n, and (t - s) | (t * s).
 * https://oeis.org/A337681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Count pairs (s, t) with 1 ≤ s < t < n, neither s nor t divides
 *            n, * and (t - s) divides t * s
 * Range:     u8 n <= 60, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t s = 1; s < n - 1; ++s) {
        if (n % s == 0) continue;                 /* s divides n → skip */
        for (idx_t t = s + 1; t < n; ++t) {
            if (n % t == 0) continue;             /* t divides n → skip */
            idx_t diff = t - s;
            if ((t * s) % diff == 0) {            /* (t - s) | (t * s) */
                count += 1;
            }
        }
    }
    return count;
}
