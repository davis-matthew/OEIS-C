/*
 * A329708  Triangle, read by rows, where the n-th row lists the (2n+1)
 *          coefficients of (1+2*x+...+(n+1)*x^n)^2.
 * https://oeis.org/A329708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^{n - r^2} in (1 + 2x + … + (r+1)x^r)^2
 *            where r = floor(sqrt(n))
 * Range:     u8 n <= 90, u16 n <= 3800, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Integer square‑root: largest r such that r*r <= n */
static inline idx_t int_sqrt(idx_t n)
{
    /* Initial approximation using floating point */
    idx_t r = (idx_t) floor(sqrt((long double)n));

    /* Correct possible off‑by‑one errors without using multiplication
       that may overflow.  r is int; use integer division for safety. */
    while ((r + 1) <= n / (r + 1)) ++r;        /* ensure (r+1)^2 > n   */
    while (r > 0 && r > n / r) --r;            /* ensure r^2 <= n      */
    return r;
}

seq_t a(idx_t n)
{
    /* Locate the row r and the position k within that row.                */
    idx_t r = int_sqrt(n);                 /*  r^2 <= n < (r+1)^2    */
    idx_t k = n - r * r;                   /* 0 <= k <= 2r           */

    /* Sum the convolution (i+1)*(k-i+1) over the valid i range.           */
    idx_t start = k > r ? k - r : 0;        /* max(0, k-r)            */
    idx_t end   = k < r ? k   : r;          /* min(k, r)              */

    seq_t sum = 0;
    for (idx_t i = start; i <= end; ++i)
    {
        seq_t term = ((seq_t)(i + 1)) * ((seq_t)(k - i + 1));
        sum += term;
    }
    return sum;
}
