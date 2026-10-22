/*
 * A389921  Number of subsets of {1..n} that contain n but do not form a
 *          finite arithmetic progression.
 * https://oeis.org/A389921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - (1 + Σ_{d=1}^{n-1} floor((n-1)/d))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts subsets of {1..n} containing n that are NOT finite arithmetic progressions. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    idx_t m = n - 1;                       /* use n-1 to simplify calculations */
    seq_t total = pow2(m);                 /* 2^(n-1) */
    seq_t ap_count = 1;                    /* the singleton set {n} */

    for (idx_t d = 1; d <= m; ++d) {
        ap_count += (seq_t)(m / d);       /* floor((n-1)/d) */
    }

    return total - ap_count;
}
