/*
 * A337682  Number of distinct positive integer pairs, (s,t), such that s < t
 *          < n where neither s nor t divides n, and (s + t) | (s * t).
 * https://oeis.org/A337682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{(s,t): 1 ≤ s < t < n, n mod s ≠ 0, n mod t ≠ 0, (s*t)
 *            mod (s+t) = 0}|
 * Range:     u8 n <= 21, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n <= 2) return count;               /* no pairs possible */
    for (idx_t s = 1; s < n - 1; ++s) {
        if (n % s == 0) continue;           /* s divides n – skip */
        for (idx_t t = s + 1; t < n; ++t) {
            if (n % t == 0) continue;       /* t divides n – skip */
            seq_t prod = (seq_t)s * t;      /* safe cast to seq_t */
            seq_t sum  = (seq_t)s + t;
            if (prod % sum == 0) ++count;   /* (s+t) divides (s*t) */
        }
    }
    return count;
}
