/*
 * A391637  Partial sums of A094345.
 * https://oeis.org/A391637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + Sum_{i=1..n} A053735(i) A053735(i) = sum of
 *            digits of i in base 3 a(0) = 0
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t total = 0;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* compute sum of base‑3 digits of i */
        idx_t x = i;
        seq_t digit_sum = 0;
        while (x != (idx_t)0) {
            digit_sum += (seq_t)(x % (idx_t)3);
            x /= (idx_t)3;
        }
        /* weight by (n – i + 1) */
        seq_t weight = (seq_t)(n - i + (idx_t)1);
        total += digit_sum * weight;
    }
    return total;
}
