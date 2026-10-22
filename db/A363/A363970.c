/*
 * A363970  Expansion of Sum_{k>0} k^2 * x^(3*k-2) / (1 - x^(3*k-2)).
 * https://oeis.org/A363970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d==1 (mod 3)} ((d+2)/3)^2
 * Range:     u8 n <= 45, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t nn = n;
    seq_t sum = 0;
    for (uint64_t d = 1; d <= nn / d; ++d) {
        if (nn % d == 0) {
            if (d % 3 == 1) {
                uint64_t t = (d + 2) / 3;
                seq_t term = (seq_t)t;
                term *= term;
                sum += term;
            }
            uint64_t other = nn / d;
            if (other != d && other % 3 == 1) {
                uint64_t t = (other + 2) / 3;
                seq_t term = (seq_t)t;
                term *= term;
                sum += term;
            }
        }
    }
    return sum;
}
