/*
 * A250304  Four-column array read by rows: T(n,k) = the coefficient of x^k in
 *          the expanded polynomial x^3 + (x+1)^3 + ... + (x+n-1)^3, for 0 <=
 *          k <= 3.
 * https://oeis.org/A250304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = coefficient of x^{(n-1) mod 4} in sum_{i=0}^{ceil(n/4)-1}
 *            (x+i)^3, i.e. the row-major array of coefficients of the cubic
 *            sums
 * Range:     u8 n <= 24, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute the row index r = ceil(n/4) */
    seq_t r = (seq_t)(n / 4 + (n % 4 != 0));

    /* t = sum_{j=0}^{r-1} j = r(r-1)/2 */
    seq_t t = r * (r - 1) / 2;

    /* coefficients of the polynomial for this row */
    seq_t c0 = t * t;            /* constant term  = sum j^3   */
    seq_t c1 = t * (2 * r - 1);  /* x coefficient  = r(r-1)(2r-1)/2  */
    seq_t c2 = 3 * t;            /* x^2 coefficient= 3 r(r-1)/2        */
    seq_t c3 = r;                /* x^3 coefficient= r                */

    /* offset within the 4‑term row */
    idx_t k = (n - 1) & 3u; /* same as (n-1)%4 */

    switch (k) {
        case 0: return c0; /* coefficient of x^0 */
        case 1: return c1; /* coefficient of x^1 */
        case 2: return c2; /* coefficient of x^2 */
        case 3: return c3; /* coefficient of x^3 */
        default: /* never reached */ return 0;
    }
}
