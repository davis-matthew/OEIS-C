/*
 * A326120  a(n) is the concatenation of n^1, n^2, ..., n^n.
 * https://oeis.org/A326120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = concat_{k=1..n} n^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    seq_t result = 0;
    seq_t power  = 1;
    seq_t seq_n  = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        power *= seq_n;                     /* power = n^k */
        int len  = digit_len(power);        /* number of decimal digits of n^k */
        seq_t mult = pow10((idx_t)len);     /* 10^len */
        result = result * mult + power;     /* append n^k to result */
    }
    return result;
}
