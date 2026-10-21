/*
 * A292627  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. exp(k*x)*BesselI(0,2*x).
 * https://oeis.org/A292627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^i in (1 + j*x + x^2)^i where (i,j) are
 *            obtained by reading the square array A(n,k) by antidiagonals.
 *            Specifically, let s = floor((sqrt(8*n+1)-1)/2) and d = n -
 *            s*(s+1)/2. Then i = d and j = s - d, and a(n) = ∑_{m=0}^{⌊i/2⌋}
 *            j^(i-2m) * C(i,2m) * C(2m,m)
 * Range:     u8 n <= 39, u16 n <= 72, u32 n <= 181, u64 n <= 425,
 *            u128 n <= 1159, big any n
 * Helpers:   binom, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Find s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while (true) {
        __int128 next = (__int128)(s + 1) * (s + 2) / 2;
        if (next > n)
            break;
        ++s;
    }

    idx_t d = n - s * (s + 1) / 2;
    idx_t i = d;               /* row index */
    idx_t j = s - d;           /* column index */  

    seq_t result = 0;
    for (idx_t m = 0; m <= i / 2; ++m) {
        int exp = (int)(i - 2 * m);          /* exponent for j^(i-2m) */
        seq_t term = pow_seq((seq_t)j, exp);
        term = term * binom(i, 2 * m);
        term = term * binom(2 * m, m);
        result += term;
    }
    return result;
}
