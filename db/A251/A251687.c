/*
 * A251687  G.f. satisfies: A(x) = exp( Sum_{n>=1} [Sum_{k=0..2*n} T(n,k)^2 *
 *          x^k] / A(x)^n * x^n/n ), where T(n,k) is the coefficient of x^k in
 *          (1 + x + 2*x^2)^n.
 * https://oeis.org/A251687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = (1 + x)*(1 + 4*x^3)*(1 - 3*x^2 + 4*x^4) / (1 - 2*x^2)^2
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Coefficients of the numerator N(x) = 1 + x -3x^2 + x^3 + 8x^4 -8x^5 -12x^6 +16x^7 +16x^8 */
    static const int N[9] = {1, 1, -3, 1, 8, -8, -12, 16, 16};

    seq_t pos = 0;   /* accumulate positive contributions */
    seq_t neg = 0;   /* accumulate negative contributions */

    for (int m = 0; m < 9; ++m) {
        if (n < (idx_t)m) continue;

        idx_t diff = n - (idx_t)m;
        if ((diff & 1u) != 0) continue;          /* n - m must be even */

        idx_t k = diff >> 1;                     /* k = (n - m)/2 */

        /* 2^k */
        seq_t pow2 = seq_pow((seq_t)2, k);

        /* (k+1) * 2^k */
        seq_t pk = pow2 * (seq_t)(k + 1);

        /* abs(N[m]) * ((k+1)*2^k) */
        int abs_nm = N[m] >= 0 ? N[m] : -N[m];
        seq_t term = pk * (seq_t)abs_nm;

        if (N[m] >= 0)
            pos += term;
        else
            neg += term;
    }

    /* final value = positive sum - negative sum */
    return pos - neg;
}
