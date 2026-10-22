/*
 * A386866  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(3*n+2,k) *
 *          binomial(3*n-k-1,n-k).
 * https://oeis.org/A386866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(3*n+2,k) *
 *            binomial(3*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* 2^n  */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    /* C1 = binomial(3*n+2, 0) = 1 */
    seq_t C1 = (seq_t)1;

    /* C2_0 = binomial(3*n-1, n)  (equals binomial(3*n-1, 2*n-1)) */
    seq_t C2 = (seq_t)1;
    seq_t three_n = (seq_t)3 * n;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = three_n - (seq_t)i;
        C2 = C2 * num / (seq_t)i;   /* exact division */
    }

    seq_t sum  = (seq_t)0;
    seq_t U    = (seq_t)3 * n - (seq_t)1;   /* U = 3*n - k - 1 for current k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow2 * C1 * C2;
        sum += term;

        if (k == n)
            break;

        /* Update C1: binomial(3*n+2, k+1) from binomial(3*n+2, k) */
        seq_t num1 = (seq_t)3 * n + (seq_t)2 - k;
        C1 = C1 * num1 / (seq_t)(k + 1);

        /* Update C2: binomial(3*n-(k+1)-1, 2*n-1) from current C2 */
        seq_t num2 = n - k;          /* n - k  > 0 because k < n */
        seq_t den2 = U;              /* current U = 3*n - k - 1 */
        C2 = C2 * num2 / den2;

        U--;                          /* next U is one less */

        /* Update power of two: divide by 2 */
        pow2 = pow2 / (seq_t)2;
    }

    return sum;
}
