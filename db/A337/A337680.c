/*
 * A337680  Number of distinct positive integer pairs, (s,t), such that s < t
 *          < n where neither s nor t divides n, and (t - s) | (t + s).
 * https://oeis.org/A337680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count pairs (s,t), s < t < n, neither s nor t divides n, and (t-s)|(t+s). */
#include <stdbool.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build.  */
seq_t a(idx_t n)
{
    /* The sequence is a(n) = # { (s,t) : 1 <= s < t < n,
       n%s != 0, n%t != 0, (t-s) | (t+s) }  */
    seq_t count = 0;
    if (n < 3) return 0;       /* no such pairs when n < 3 */

    for (idx_t s = 1; s + 2 <= n; ++s) {
        if (n % s == 0) continue;          /* s must not divide n */

        idx_t two_s = 2 * s;
        for (idx_t d = 1; d * d <= two_s; ++d) {
            if (two_s % d == 0) {
                /* first divisor d */
                idx_t t1 = s + d;
                if (t1 < n && n % t1 != 0) { count++; }

                /* second divisor two_s/d (if distinct) */
                idx_t d2 = two_s / d;
                if (d2 != d) {
                    idx_t t2 = s + d2;
                    if (t2 < n && n % t2 != 0) { count++; }
                }
            }
        }
    }
    return count;
}
