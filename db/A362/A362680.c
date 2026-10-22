/*
 * A362680  a(n) is the number of decimal digits in A173426(n).
 * https://oeis.org/A362680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A058183(n) + A058183(n-1) (A058183(n) = sum_{k=1}^n
 *            digits(k))
 * Range:     u8 n <= 68, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_count.h"

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n=0 is irrelevant */
    seq_t sum = 0;
    seq_t d = 0;

    for (idx_t i = 1; i <= n; ++i) {
        d = (seq_t)digit_count((uint64_t)i);
        sum += d;
    }

    /* sum   = A058183(n)
       sum-d = A058183(n-1) */
    seq_t sum_prev = sum - d;
    return sum + sum_prev;           /* a(n) */
}
