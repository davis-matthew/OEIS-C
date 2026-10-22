/*
 * A305722  Crystal ball sequence for the lattice C_8.
 * https://oeis.org/A305722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..8} binomial(16, 2k)*binomial(n+k, 8) Initial
 *            terms: a(0) = 1, a(1) = 129, a(2) = 2945, a(3) = 29953, a(4) =
 *            187137, a(5) = 845185, a(6) = 3032705, a(7) = 9173505, a(8) =
 *            24331777, a(9) = 58161793, a(10) = 127791489, a(11) = 261902081,
 *            a(12) = 506298625, a(13) = 931299201, a(14) =...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 262,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute C(n,8) with reduction to avoid overflow */
static seq_t binom8(idx_t n)
{
    if (n < 8) return 0;
    seq_t res = 1;
    /* k = 8 */
    for (idx_t i = 1; i <= 8; ++i)
    {
        seq_t num = n - 8 + i;   /* numerator  (n-8+i) */
        seq_t den = i;           /* denominator i       */

        /* cancel common factors between num and den */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        if (den > 1) {
            /* try to cancel remaining denominator with current result */
            seq_t g2 = gcd(res, den);
            res /= g2;
            den /= g2;
        }
        /* now den should be 1 */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* binomial(16, 2k) for k=0..8 */
    static const seq_t coeff[9] = {1, 120, 1820, 8008, 12870,
                                   8008, 1820, 120, 1};
    seq_t sum = 0;
    for (idx_t k = 0; k < 9; ++k) {
        seq_t bin = binom8(n + k);      /* C(n+k, 8) */
        sum += coeff[k] * bin;          /* accumulate weighted term */
    }
    return sum;
}
