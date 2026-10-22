/*
 * A370469  Triangle read by columns where T(n,k) is the number of points in
 *          Z^n such that |x1| + ... + |xn| = k, |x1|, ..., |xn| > 0.
 * https://oeis.org/A370469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = binomial(k-1,r-1) * 2^r where k = ceil((sqrt(1+8n)-1)/2)
 *            and r = n - (k-1)k/2
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* offset is 1 – undefined for n < 1 */
    if (n < 1) return 0;

    /* Compute the column index k.
       k is the smallest integer with k(k+1)/2 >= n       */
    double d = sqrt((double)n * 8.0 + 1.0);          /* 1 + 8n fits in a double for all practical n */
    idx_t k = (idx_t)((d - 1.0) / 2.0);              /* floor of the expression */
    unsigned __int128 sum = (unsigned __int128)k * (k + 1) / 2;
    if (sum < (unsigned __int128)n) ++k;            /* adjust if floor was too small */

    /* row index r inside this column (1‑based) */
    unsigned __int128 prev = (unsigned __int128)(k - 1) * k / 2;
    idx_t r = (idx_t)(n - prev);                    /* 1 <= r <= k */

    /* T(r,k) = binomial(k-1, r-1) * 2^r  */
    seq_t bin = binomial(k - 1, r - 1);
    seq_t two = pow2(r);
    return bin * two;
}
