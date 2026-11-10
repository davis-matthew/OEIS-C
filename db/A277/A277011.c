/*
 * A277011  Left inverse of A277012.
 * https://oeis.org/A277011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A276075(A005940(1+n)); computed directly: scan n's
 *            binary, with i=1, f=1!; if m odd: a += i!, m=(m-1)/2; else m/=2,
 *            i++ (f *= i)
 * Range:     u8 n <= 31, u16 n <= 255, u32 n <= 4095, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 100 terms through n = 6665 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t res = 0;
    seq_t f = 1;      /* i! */
    idx_t i = 1;
    while (m > 0) {
        if (m & 1) {
            res += f;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
            f *= (seq_t)i;
        }
    }
    return res;
}
