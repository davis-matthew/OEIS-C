/*
 * A279511  Sierpinski square-based pyramid numbers: a(n) = 5*a(n-1) -
 *          (2^(n+1) + 7).
 * https://oeis.org/A279511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 5*a(n-1) - (2^(n+1)+7),  a(0) = 5
 * (where 2^(n+1) is computed by the provided pow2 utility)
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base case: a(0) = 5 */
    if (n == 0) {
        return (seq_t)5;
    }

    seq_t current = (seq_t)5;           /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t subtract = pow2(i + 1) + (seq_t)7; /* 2^(i+1) + 7 */
        current = current * (seq_t)5 - subtract; /* 5*a(i-1) - (2^(i+1)+7) */
    }
    return current;
}
