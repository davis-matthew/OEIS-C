/*
 * A375673  n and a(n) (with a(n) >= n) are the edges of the minimum-area
 *          rectangle such that its area is an integer multiple of its
 *          perimeter.
 * https://oeis.org/A375673
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = min{ m >= n | n*m is an integer multiple of 2*(n+m) } */
/* Equivalently, a(n) = m where m = 2*t*n/(n-2*t) with t=1..⌊(n-1)/2⌋ and n*m = 2t(n+m) */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Special case: n divisible by 4 gives trivial solution m = n */
    if ((n & 3ULL) == 0) {
        return (seq_t)n;
    }

    seq_t best_m = 0;  /* 0 used as sentinel for not yet found */
    idx_t max_t = (n - 1) / 2;              /* largest t with n-2*t > 0 */
    for (idx_t t = 1; t <= max_t; ++t) {
        idx_t denom_u = n - 2 * t;          /* positive */
        seq_t denom = (seq_t)denom_u;
        seq_t numer = (seq_t)2 * (seq_t)t * (seq_t)n;
        if (numer % denom == 0) {
            seq_t m = numer / denom;
            if (m >= (seq_t)n) {
                if (best_m == 0 || m < best_m) {
                    best_m = m;
                }
            }
        }
    }
    /* best_m is guaranteed to be set for n >= 3 */
    return best_m;
}
