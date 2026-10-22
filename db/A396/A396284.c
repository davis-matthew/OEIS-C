/*
 * A396284  a(n) = Sum_{k=1..n} (n mod k)^3.
 * https://oeis.org/A396284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n mod k)^3
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k;
    for (k = 1; k <= n; ++k) {
        idx_t r = n % k;                    /* r < k <= n, fits in idx_t */
        seq_t r_seq = (seq_t)r;
        sum += r_seq * r_seq * r_seq;        /* (n mod k)^3 */
    }
    return sum;
}
