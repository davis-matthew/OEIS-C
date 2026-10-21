/*
 * A252873  Number of n X 5 nonnegative integer arrays with upper left 0 and
 *          lower right n+5-4 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{11} coeff[k] * C(n - k + 10, 10)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients are from the generating function:
   a(x) = x(6 - 5x + 72x² - 225x³ + 460x⁴ - 612x⁵ + 537x⁶ - 315x⁷
                    + 121x⁸ - 28x⁹ + 3x¹⁰) / (1 - x)¹¹
   This expansion yields the integer sequence A252873. */

#include <stdint.h>

static const int coeff[12] = {0, 6, -5, 72, -225, 460, -612, 537, -315,
                               121, -28, 3};

/* Compute the binomial coefficient C(n, 10) for n ≥ 10. */
static seq_t binom10(idx_t n) {
    seq_t r = 1;
    for (idx_t i = 1; i <= 10; ++i) {
        r = r * (n - 10 + i);  /* multiply by (n - 10 + i) */
        r = r / i;             /* divide by i (always divisible) */
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t result = 0;

    for (int k = 1; k <= 11; ++k) {
        idx_t t = n + 10 - (idx_t)k;          /* n - k + 10   */
        if (t < 10) continue;                 /* binomial zero   */

        seq_t bin = binom10(t);               /* C(t, 10)       */
        unsigned int ac = (coeff[k] < 0) ? (unsigned int)(-coeff[k])
                                         : (unsigned int)coeff[k];
        seq_t term = bin * (seq_t)ac;         /* bin * |coeff|   */

        if (coeff[k] >= 0)
            result += term;
        else
            result -= term;
    }
    return result;
}
