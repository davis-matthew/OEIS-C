/*
 * A254596  Convert number n to each of the bases from 2 to 10. a(n) is the
 *          minimum base that maximizes the sum of digits.
 * https://oeis.org/A254596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest base b (2 ≤ b ≤ 10) for which the sum of the
 *            digits * of n in base b is maximal; ties are broken in favour of
 *            the smaller base
 * Range:     u8 n <= 352, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t best_sum = 0;
    int best_base = 2;   /* a(n) is always between 2 and 10 */

    for (int b = 2; b <= 10; ++b) {
        seq_t sum = 0;
        seq_t m   = (seq_t)n;
        seq_t base = (seq_t)b;

        while (m > 0) {
            sum += m % base;
            m   /= base;
        }

        if (sum > best_sum || (sum == best_sum && b < best_base)) {
            best_sum = sum;
            best_base = b;
        }
    }
    return (seq_t)best_base;
}
