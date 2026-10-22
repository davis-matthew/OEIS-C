/*
 * A363869  a(n) = A108625(3*n, 2*n).
 * https://oeis.org/A363869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..2*n} binomial(3*n, k)^2 * binomial(5*n - k, 3*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t t3 = 3 * n;   /* 3n   */
    idx_t t2 = 2 * n;   /* 2n   */
    idx_t t5 = 5 * n;   /* 5n   */

    /* binom5 = C(5n, 3n)  (k = 0) */
    seq_t binom5 = 1;
    for (idx_t i = 1; i <= t3; ++i)
        binom5 = binom5 * (seq_t)(t5 - (i - 1)) / (seq_t)i;

    seq_t binom3 = 1;   /* C(3n, 0)  */
    seq_t sum = 0;

    for (idx_t k = 0; k <= t2; ++k)
    {
        /* term = C(3n,k)^2 * C(5n-k,3n) */
        seq_t term = binom3 * binom3 * binom5;
        sum = sum + term;

        if (k == t2)
            break;

        /* update binom3 to C(3n, k+1) */
        binom3 = binom3 * (seq_t)(t3 - k) / (seq_t)(k + 1);

        /* update binom5 to C(5n-(k+1), 3n) */
        binom5 = binom5 * (seq_t)(t2 - k) / (seq_t)(t5 - k);
    }

    return sum;
}
