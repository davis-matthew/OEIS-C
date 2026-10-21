/*
 * A277374  a(n) = 2^n*floor(n/2)!*Gamma(ceiling((n+1)/2),-1/4)*exp(-1/4).
 * https://oeis.org/A277374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n*floor(n/2)!*Gamma(ceil((n+1)/2),-1/4)*exp(-1/4)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute a(2m) for m >= 0.  Uses factorial and an alternating sum. */
static seq_t compute_even(idx_t m)
{
    /* factorial m! */
    seq_t fact_m = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i)
        fact_m = fact_m * (seq_t)i;

    /* (m!)^2 */
    seq_t base = fact_m * fact_m;

    /* 4^m == 2^(2m) */
    seq_t pow4 = pow2(2 * m);

    /* T = base * 4^m */
    seq_t T = base * pow4;

    seq_t sum = (seq_t)0;
    int sign = 1;

    for (idx_t k = 0; k <= m; ++k) {
        if (sign > 0)
            sum = sum + T;
        else
            sum = sum - T;

        if (k == m)
            break;

        idx_t div = (k + 1) * 4;
        T = T / (seq_t)div;
        sign = -sign;
    }

    return sum;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t m = n >> 1;

    seq_t even = compute_even(m);

    if (n & 1)   /* n is odd: a(n) = 2 * a(n-1) */
        return even * (seq_t)2;

    return even;
}
