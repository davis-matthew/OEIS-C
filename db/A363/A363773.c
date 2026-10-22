/*
 * A363773  a(n) = (4^(n+1) + (-1)^n + 5)/10.
 * https://oeis.org/A363773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(n+1) + (-1)^n + 5) / 10 Recurrence used: a(0) = 1
 *            a(n) = 4*a(n-1) - 1 if n is even a(n) = 4*a(n-1) - 2 if n is odd
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }

    seq_t val = (seq_t)1;   // a(0)
    for (idx_t i = 1; i <= n; ++i) {
        if ((i & (idx_t)1) == 0) {          // n even
            val = (val << 2) - (seq_t)1;    // 4 * val - 1
        } else {                            // n odd
            val = (val << 2) - (seq_t)2;    // 4 * val - 2
        }
    }
    return val;
}
