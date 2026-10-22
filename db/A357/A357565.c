/*
 * A357565  a(n) = 3*Sum_{k = 0..n} binomial(n+k-1,k)^2 + 2*Sum_{k = 0..n}
 *          binomial(n+k-1,k)^3.
 * https://oeis.org/A357565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*Sum_{k=0..n} binomial(n+k-1,k)^2 + 2*Sum_{k=0..n}
 *            binomial(n+k-1,k)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* only for completeness */

seq_t a(idx_t n)
{
    seq_t b = 1;         /* binomial(n+k-1, k) for current k */
    seq_t sum_sq = 0;
    seq_t sum_cu = 0;

    for (idx_t k = 0; k <= n; ++k) {
        if (k != 0) {
            /* b = b * (n + k - 1) / k;  exact division */
            b = b * ((seq_t)n + k - 1) / k;
        }
        sum_sq += b * b;
        sum_cu += b * b * b;
    }

    return sum_sq * 3 + sum_cu * 2;
}
