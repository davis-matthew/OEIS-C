/*
 * A363291  Sum of divisors of 4*n-1 of form 4*k+1.
 * https://oeis.org/A363291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors d of 4*n-1 with d ≡ 1 (mod 4)
 * Range:     u8 n <= 64, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t m = (seq_t)4 * (seq_t)n - 1;
    seq_t sum = 0;
    idx_t d;
    for (d = 1; (seq_t)d * (seq_t)d <= m; ++d) {
        if (m % (seq_t)d == 0) {
            seq_t q = m / (seq_t)d;
            if ((d & 3) == 1) sum += (seq_t)d;
            if (q != (seq_t)d && (q & 3) == 1) sum += q;
        }
    }
    return sum;
}
