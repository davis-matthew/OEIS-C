/*
 * A373187  Expansion of Sum_{k>=0} x^(4^k) / (1 - x^(4^k))^4.
 * https://oeis.org/A373187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+2,3) + (n%4==0 ? a(n/4) : 0) (equivalently:
 *            a(n) = Σ_{i≥0} C(n/4^i + 2, 3) while n/4^i > 0)
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – not needed for OEIS offset */
    seq_t result = 0;
    idx_t cur = n;
    while (1) {
        seq_t x = (seq_t)cur;
        /* C(x+2,3) = (x+2)(x+1)*x / 6 */
        result += (x * (x + 1) * (x + 2)) / 6;
        if (cur % 4 != 0) break;
        cur /= 4;
    }
    return result;
}
