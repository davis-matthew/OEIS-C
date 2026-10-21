/*
 * A279496  Number of square pyramidal numbers dividing n.
 * https://oeis.org/A279496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of square‐pyramidal numbers dividing n
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at n = 1 */
    seq_t nn = (seq_t)n;
    seq_t count = 0;
    for (seq_t k = 1; ; ++k) {
        /* compute P(k) = k(k+1)(2k+1)/6 using only seq_t operations */
        seq_t a0 = k;
        seq_t b0 = k + 1;
        seq_t c0 = k * 2 + 1;

        /* divide by 2 once */
        if (a0 % (seq_t)2 == 0)
            a0 /= 2;
        else if (b0 % (seq_t)2 == 0)
            b0 /= 2;
        else
            c0 /= 2;

        /* divide by 3 once */
        if (a0 % (seq_t)3 == 0)
            a0 /= 3;
        else if (b0 % (seq_t)3 == 0)
            b0 /= 3;
        else
            c0 /= 3;

        seq_t p = a0 * b0;
        p = p * c0;                /* p == k(k+1)(2k+1)/6 */

        if (p > nn) break;
        if (nn % p == 0)
            ++count;
    }
    return count;
}
