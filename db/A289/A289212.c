/*
 * A289212  a(n) = n! * Laguerre(n,-6).
 * https://oeis.org/A289212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Laguerre(n,-6) Recurrence: a(0)=1, a(1)=7, a(n) =
 *            (2n-1+7)*a(n-1) - (n-1)^2*a(n-2) (n≥2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a0 = 1;          /* a(0) */
    seq_t a1 = 7;          /* a(1) */
    if (n == 0) return a0;
    if (n == 1) return a1;

    seq_t prev2 = a0;      /* a(n-2) */
    seq_t prev1 = a1;      /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i) {
        idx_t k = i - 1;                      /* n-1 in the recurrence */
        seq_t coeff = (seq_t)(2 * k + 7);     /* (2k+7) */
        seq_t term1 = coeff * prev1;          /* (2k+7)*a(n-1) */
        seq_t term2 = (seq_t)(k * k) * prev2; /* k^2 * a(n-2) */
        seq_t curr = term1 - term2;           /* a(n) */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
