/*
 * A397286  Triangle read by rows: T(n, k) = 2^(n - k) * P_{n, k}(1/2) where
 *          P_{n, k}(x) = x * (n - k + x)^(n - k - 1) * binomial(n, k).
 * https://oeis.org/A397286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element in the triangle read by rows; row r =
 *            floor((sqrt(8n+1)-1)/2), column k = n - r(r+1)/2. T(r,k) =
 *            binomial(r,k) * (2r-2k+1)^(r-k-1) for k < r, else 1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 119, u128 n <= 299,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Determine row r.  r satisfies r(r+1)/2 <= n < (r+1)(r+2)/2.
       Solve r^2 + r - 2n <= 0, giving  r = floor((sqrt(8n+1)-1)/2). */
    idx_t m = 8*n + 1;                 /* 8n+1 fits in 64 bits for n < 10^18 */
    idx_t s = _floor_isqrt_u64(m);     /* floor sqrt of m */
    idx_t r = (s - 1) / 2;

    /* Find column k within row r */
    idx_t start = r * (r + 1) / 2;     /* first index of this row */
    idx_t k = n - start;               /* column index */

    /* Diagonal entries are 1 */
    if (k == r)
        return (seq_t)1;

    /* Compute T(r,k) = binomial(r,k) * (2r-2k+1)^(r-k-1) */
    seq_t base = (seq_t)(2*r - 2*k + 1);
    idx_t exp = r - k - 1;             /* exponent >= 0 */
    seq_t pow_val = pow_seq(base, (int)exp);
    seq_t binom = binomial(r, k);
    return binom * pow_val;
}
