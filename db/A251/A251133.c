/*
 * A251133  Number of (n+1) X (4+1) 0..2 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3063 + 961*(-1)^n + 3872*2^n + 1726*n + 691*n^2 +
 *            146*n^3 + 11*n^4) / 12 (n>3)
 * Range:     u8 none, u16 n <= 5, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* first three values are given explicitly – a(1)=919, a(2)=2264, a(3)=4110 */
    if (n == 1) return 919;
    if (n == 2) return 2264;
    if (n == 3) return 4110;

    /*--- compute the polynomial part ------------------------------------*/
    seq_t s  = (seq_t)n;
    seq_t s2 = s * s;             /* n^2          */
    seq_t s3 = s2 * s;            /* n^3          */
    seq_t s4 = s3 * s;            /* n^4          */

    seq_t term = 3063;             /* 3063 */

    /* add or subtract 961 depending on the parity of n */
    if ((n & 1ULL) == 0)          /* n even */
        term += 961;
    else
        term -= 961;

    term += (seq_t)1726 * s;       /* 1726*n    */
    term += (seq_t)691  * s2;      /* 691*n^2   */
    term += (seq_t)146  * s3;      /* 146*n^3   */
    term += (seq_t)11   * s4;      /* 11*n^4    */

    seq_t sdiv = term / 12;        /* floor(S0/12)   */
    seq_t rmod = term % 12;        /* remainder      */

    /*--- compute the 2^n part ------------------------------------------*/
    seq_t pow2n = pow_uint(2, n);  /* 2^n          */

    /* 322*2^n = (2^n << 8) + (2^n << 6) + (2^n << 1)  (all shifts are safe) */
    seq_t part322 = (pow2n << 8) + (pow2n << 6) + (pow2n << 1);

    /* (8*2^n + rmod)/12  is an integer, avoid overflow by using small shifts */
    seq_t temp = (pow2n << 3) + rmod;   /* 8*2^n + rmod */
    seq_t part8  = temp / 12;           /* integer division */

    /* final result -----------------------------------------------------*/
    return sdiv + part322 + part8;
}
