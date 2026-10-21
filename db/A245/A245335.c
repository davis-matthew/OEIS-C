/*
 * A245335  Sum of digits of n in fractional base 5/4.
 * https://oeis.org/A245335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A024634(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = sum of digits of n in fractional base 5/4. */
/*   Digits are obtained by: d = n mod 5, n = (n - d)/5 * 4. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    uint64_t cur = (uint64_t)n;

    while (cur > 0) {
        uint32_t d = (uint32_t)(cur % 5ULL);       /* 0 <= d <= 4 */
        sum += d;
        cur = ((cur - d) / 5ULL) * 4ULL;          /* next term in representation */
    }

    return sum;
}
