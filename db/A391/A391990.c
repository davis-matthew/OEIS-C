/*
 * A391990  a(n) = Sum_{k=0..floor(n/2)} (k+1) * binomial(n,k) *
 *          binomial(n-k,k).
 * https://oeis.org/A391990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (k+1) * binomial(n,k) *
 *            binomial(n-k,k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t C1   = 1;          /* binom(n,0) */
    seq_t C2   = 1;          /* binom(n,0) */
    idx_t limit = n / 2;

    for (idx_t k = 0; k <= limit; ++k) {
        if (k > 0) {
            /* Update C1: binom(n,k) = binom(n,k-1) * (n-k+1) / k */
            C1 = C1 * (seq_t)(n - k + 1) / (seq_t)k;

            /* Update C2 using the derived recurrence */
            idx_t num1 = n - 2 * (k - 1);       /* n - 2*(k-1) */
            idx_t num2 = num1 - 1;              /* n - 2*(k-1) - 1 */
            idx_t den1 = k;                     /* k */
            idx_t den2 = n - (k - 1);           /* n - (k-1) */
            C2 = C2 * (seq_t)num1 * (seq_t)num2 / ((seq_t)den1 * (seq_t)den2);
        }
        /* term = (k+1) * C1 * C2 */
        seq_t term = (seq_t)(k + 1) * C1 * C2;
        sum += term;
    }
    return sum;
}
