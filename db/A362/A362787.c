/*
 * A362787  Triangle read by rows, T(n, k) = (-1)^k * RisingFactorial(n, k) *
 *          FallingFactorial(k - n, k).
 * https://oeis.org/A362787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2,
 *            T(r,k) = (-1)^k * rising(r,k) * falling(k-r,k)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 41, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Here the value is computed as product of two rising factorials:   */
/*   T(r,k) = rising(r,k) * rising(r-k,k) (except T(r,r)=0)          */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* compute row index r */
    double d = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) * 0.5);

    /* adjust due to floating-point rounding */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* position k within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* T(r,k) = rising(r,k) * rising(r-k,k)   (k==r gives 0) */
    seq_t rise1 = 1;
    for (idx_t i = 0; i < k; ++i)
        rise1 *= (seq_t)(r + i);

    seq_t rise2 = 1;
    for (idx_t i = 0; i < k; ++i)
        rise2 *= (seq_t)(r - k + i);

    return rise1 * rise2;
}
