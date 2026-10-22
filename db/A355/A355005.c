/*
 * A355005  Table read by rows. T(n, k) = n*((k + n)!)^2/((k + n)*(n!)^2*k!)
 *          for n > 0 and T(0, 0) = 1.
 * https://oeis.org/A355005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = n * ((k+n)!)^2 / ((k+n) * (n!)^2 * k!) for n>0, with
 *            T(0,0)=1. The sequence a(n) is obtained by reading the triangle
 *            (n,k) row by row; so a(n) = T(r,k) where r =
 *            floor((sqrt(1+8n)-1)/2) and k = n - r(r+1)/2
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 34, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are defined by the build system. */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* T(0,0) */
        return (seq_t)1;

    /* Find row r: r(r+1)/2 <= n < (r+1)(r+2)/2. */
    double disc = 1.0 + 8.0 * (double)n;
    idx_t r = (idx_t)((sqrt(disc) - 1.0) / 2.0);

    /* Correct any rounding error from double sqrt. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t start = r * (r + 1) / 2;   /* first index of row r */
    idx_t k = n - start;             /* column in row r */

    /* For r == 0 we would have returned earlier. */
    seq_t res = 1;

    /* Compute binomial part: C(r+k-1, r-1) = ∏_{i=1}^{r-1} (k+i)/i */
    for (idx_t i = 1; i <= r - 1; ++i)
        res = res * (seq_t)(k + i) / (seq_t)i;

    /* Multiply by the remaining product ∏_{i=1}^{k} (r+i) */
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(r + i);

    return res;
}
