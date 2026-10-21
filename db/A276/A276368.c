/*
 * A276368  G.f. A(x) satisfies: A(x - 3*x^3) = 1/(1 - 3*x).
 * https://oeis.org/A276368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 3^n * C(3*n, n) a(2*n+1) = 3^(n+1)* C(3*n+1, n) where
 *            C(a,b) is the binomial coefficient
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow3_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3_of.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                  /* even index */
        idx_t m = n >> 1;                /* m = n/2 */
        seq_t bin = binomial(3 * m, m);  /* C(3m, m) */
        seq_t p   = pow3_of(m);           /* 3^m       */
        return bin * p;
    } else {                              /* odd index */
        idx_t m = (n - 1) >> 1;           /* m = (n-1)/2 */
        seq_t bin = binomial(3 * m + 1, m);/* C(3m+1, m) */
        seq_t p   = pow3_of(m + 1);       /* 3^(m+1)   */
        return bin * p;
    }
}
