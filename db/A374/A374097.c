/*
 * A374097  a(n) = A196563(n)*A196564(n).
 * https://oeis.org/A374097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A196563(n)*A196564(n), where A196563(n) = number of even
 *            digits of n (a(0)=1), A196564(n) = number of odd digits of n
 *            (a(0)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 127 terms through n = 9869 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t even = 0;
    seq_t odd = 0;
    idx_t x = n;

    if (n == 0U)
        return (seq_t)0;

    while (x != 0U) {
        unsigned d = (unsigned)(x % 10U);
        if (d & 1U)
            odd++;
        else
            even++;
        x /= 10U;
    }
    return even * odd;
}
