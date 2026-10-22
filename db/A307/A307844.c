/*
 * A307844  Constant term in the expansion of (n/x + 1 + n*x)^n.
 * https://oeis.org/A307844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} n^(2*k) * binomial(n,2*k) *
 *            binomial(2*k,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Precompute values used in recurrence */
    const seq_t n_seq = n;
    const seq_t n2 = n_seq * n_seq;        /* n^2 for powering */
    const idx_t maxk = n / 2;              /* floor(n/2) */

    seq_t sum   = 0;                       /* result accumulator */
    seq_t power = 1;                       /* n^(2*k), starts with k=0 */
    seq_t bin1  = 1;                       /* binomial(n,2*k), starts with k=0 */
    seq_t bin2  = 1;                       /* binomial(2*k,k), starts with k=0 */

    for (idx_t k = 0; k <= maxk; ++k) {
        /* term = n^(2*k) * C(n,2*k) * C(2*k,k) */
        seq_t term = power;
        term = term * bin1;
        term = term * bin2;
        sum += term;

        if (k == maxk) break;   /* No more terms; avoid unnecessary updates */

        /* Update bin1 to C(n,2*k+2) using two exact steps:
           C(n,2k+1)  = C(n,2k)   * (n-2k)   / (2k+1)
           C(n,2k+2)  = C(n,2k+1) * (n-2k-1) / (2k+2)   */
        seq_t mult1 = n_seq - 2*k;
        seq_t div1  = 2*k + 1;
        bin1 = bin1 * mult1 / div1;

        seq_t mult2 = n_seq - 2*k - 1;
        seq_t div2  = 2*k + 2;
        bin1 = bin1 * mult2 / div2;

        /* Update bin2 to C(2k+2,k+1) using the formula:
           C(2k+2,k+1) = C(2k,k) * (2k+2)*(2k+1) / (k+1)^2   */
        seq_t num = (2*k + 2) * (2*k + 1);
        seq_t den = (k + 1) * (k + 1);
        bin2 = bin2 * num / den;

        /* Update power for next k (k -> k+1)
           n^(2*(k+1)) = n^(2*k) * n^2  */
        power = power * n2;
    }

    return sum;
}
