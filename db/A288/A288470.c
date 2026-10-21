/*
 * A288470  a(n) = Sum_{k=0..n} binomial(n,k)*binomial(2*n,2*k).
 * https://oeis.org/A288470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(2*n, 2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the build. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* binomial(n,0) = 1 and binomial(2*n,0) = 1 */
    seq_t result = (seq_t)0;
    seq_t c1     = (seq_t)1;   /* C(n,0)          */
    seq_t c2     = (seq_t)1;   /* C(2n,0)         */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* add the current term C(n,k) * C(2n,2k) */
        result += c1 * c2;

        /* finished when the last term has been added */
        if (k == n)
            break;

        /* update C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        seq_t num1 = (seq_t)(n - k);
        seq_t den1 = (seq_t)(k + 1);
        c1 = c1 * num1 / den1;

        /* update C(2n,2(k+1)) =
           C(2n,2k) * (2n-2k) * (2n-2k-1) / ((2k+2)*(2k+1)) */
        seq_t num2 = (seq_t)(2 * n - 2 * k) *
                     (seq_t)(2 * n - 2 * k - 1);
        seq_t den2 = (seq_t)(2 * k + 2) *
                     (seq_t)(2 * k + 1);
        c2 = c2 * num2 / den2;
    }

    return result;
}
