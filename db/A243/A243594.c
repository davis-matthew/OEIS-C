/*
 * A243594  Triangle read by rows: T(n,k) = coefficient of [x^(n-k)] in the
 *          expansion of the polynomial (x+n)^n.
 * https://oeis.org/A243594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = ⌊(√(1+8*n)-1)/2⌋ and k = n - r*(r+1)/2
 *            and T(r, k) = binomial(r, k) * r^k. This reads the triangle
 *            (x+r)^r row-wise and returns the n-th element
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"   /* _floor_isqrt_u64 */
#include "../utils/binomial.h"           /* binomial */
#include "../utils/pow_int.h"            /* pow_int */

seq_t a(idx_t n)
{
    /* find row r and column k such that n = Σ_{i=0}^{r-1} (i+1) + k  (0 ≤ k ≤ r) */
    idx_t val = 8 * n + 1;
    idx_t s   = _floor_isqrt_u64(val);          /* floor sqrt(1+8*n) */
    idx_t r   = (s - 1) / 2;                    /* row index */
    idx_t start = r * (r + 1) / 2;              /* first index of this row */
    idx_t k     = n - start;                    /* column index (0‑based) */

    /* compute binomial(r, k) * r^k  */
    seq_t bin  = binomial(r, k);
    seq_t pow  = pow_int((seq_t)r, k);          /* r^k */

    return bin * pow;
}
