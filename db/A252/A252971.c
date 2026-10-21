/*
 * A252971  Number of nX3 nonnegative integer arrays with upper left 0 and
 *          lower right n+3-5 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^8 q_k * C(n+7-k, 9) where q = [1, 8, 18, -70, 94,
 *            -78, 43, -14, 2] and C(·,·) is the binomial coefficient. The
 *            generating function G(x) =
 *            x²(1+8x+18x²-70x³+94x⁴-78x⁵+43x⁶-14x⁷+2x⁸)/(x-1)¹⁰ yields this
 *            closed‑form
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 37, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   binom, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 (offset 1) */
    if (n < 2) return 0;

    /* coefficients q_k */
    static const int q[9] = {
        1, 8, 18, -70, 94, -78, 43, -14, 2
    };

    seq_t sum_pos = 0;   /* sum of positive contributions   */
    seq_t sum_neg = 0;   /* sum of absolute values of negative contributions */

    /* a + 7 - k ranges from n+7 down to n-1 */
    const idx_t base = n + 7;

    for (idx_t k = 0; k < 9; ++k) {
        int coef = q[k];
        if (coef == 0) continue;

        idx_t a_val = base - k;          /* n + 7 - k   */
        seq_t bin = binom(a_val, 9);     /* C(a_val,9)  */
        if (bin == 0) continue;          /* binomial zero when a_val < 9 */

        unsigned int abs_coef = (unsigned int)(coef > 0 ? coef : -coef);
        /* Multiply binomial by |coef| */
        seq_t prod = mul_small(bin, abs_coef);

        if (coef > 0)
            sum_pos += prod;
        else
            sum_neg += prod;
    }

    /* The final result is non‑negative (sequence values are ≥0) */
    return sum_pos - sum_neg;
}
