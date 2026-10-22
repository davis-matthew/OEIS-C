/*
 * A323970  Number of 6 X n integer matrices (m_{i,j}) such that m_{1,1}=0,
 *          m_{6,n}=2, and all rows, columns, and falling diagonals are
 *          (weakly) monotonic without jumps of 2.
 * https://oeis.org/A323970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^{13} c[i] * binom(n + 12 - i, 12) where c = {1,
 *            -3, 33, 21, -308, 1145, -2253, 2958, -2728, 1788, -822, 253,
 *            -47, 4}
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 21, u64 n <= 152, u128 n <= 6292,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) with 0 <= k <= n.
   Uses an iterative multiplicative formula and works for
   arbitrary-precision seq_t as well as standard unsigned types. */
static seq_t binom_u64(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial N(x) in OEIS entry
       (N(x) / (1 - x)^13).  Negative coefficients are handled
       separately to avoid sign problems with unsigned seq_t. */
    static const int64_t coeff[14] = {
        1,  -3,   33,   21,  -308,  1145, -2253,  2958,
      -2728, 1788, -822,  253,  -47,     4
    };

    idx_t max_i = n < 13 ? n : 13;   /* Only terms with i <= n contribute */
    seq_t pos = 0, neg = 0;          /* Accumulate positive and negative parts separately */

    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t b = binom_u64(n + 12 - i, 12);
        int64_t ci = coeff[i];
        if (ci >= 0)
            pos += b * (seq_t)ci;
        else
            neg += b * (seq_t)(-ci);
    }
    return pos - neg;  /* pos >= neg for all n due to sequence construction */
}
