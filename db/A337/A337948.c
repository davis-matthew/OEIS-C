/*
 * A337948  L.g.f.: -log( Sum_{n=-oo..+oo} (-p)^n * (p*x)^(n^2) ) = Sum_{n>=1}
 *          a(n) * x^n/n, where p = sqrt(2).
 * https://oeis.org/A337948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m>=1} [ [n % (2m) == 0] * 2^{m*(n/(2m))} * (2m) + [n
 *            % (2m-1) == 0] * ( 2^{m*(n/(2m-1))} + 2^{(m-1)*(n/(2m-1))} ) *
 *            (2m-1) ]
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The formula follows from the product representation
 *  exp(-L(x)) = ∏_{m>=1} (1-2^m x^{2m})(1-2^m x^{2m-1})(1-2^{m-1} x^{2m-1})  */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term */
    if (n == 0)
        return 0;           /* undefined for n<1, but guard anyway */

    seq_t sum = 0;

    /* The largest m needed satisfies 2m-1 ≤ n  →  m ≤ (n+1)/2  */
    idx_t max_m = (n + 1) / 2;

    for (idx_t m = 1; m <= max_m; ++m) {
        /* even exponent 2*m */
        idx_t e_even = 2 * m;
        if (e_even <= n && n % e_even == 0) {
            idx_t k = n / e_even;
            seq_t term = pow2(m * k);   /* 2^{m*k}   */
            term *= e_even;             /* multiply by e_even */
            sum += term;
        }

        /* odd exponent 2*m-1 */
        idx_t e_odd = 2 * m - 1;
        if (e_odd <= n && n % e_odd == 0) {
            idx_t k = n / e_odd;

            seq_t term1 = pow2(m * k);          /* 2^{m*k}   */
            term1 *= e_odd;                     /* multiply by e_odd */
            sum += term1;

            seq_t term2 = pow2((m - 1) * k);    /* 2^{(m-1)*k}  */
            term2 *= e_odd;                     /* multiply by e_odd */
            sum += term2;
        }
    }

    return sum;
}
