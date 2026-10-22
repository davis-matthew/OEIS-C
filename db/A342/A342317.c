/*
 * A342317  T(n, k) = [x^n] 2^n*P(n, x), where P(n, x) = (1 + 4*x)^(n + 1) +
 *          (1 - 2^(-2*n-1))*(2 + 4*x)^(n + 1). Triangle read by rows, T(n, k)
 *          for 0 <= k <= n+1 if n >= 0 and by convention T(-1, 0) = 0.
 * https://oeis.org/A342317
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binom(n+1,k) * (2^(n+2k) + 2^(2n+1+k) - 2^k)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the sequence a(-1)=0, then the triangle rows read left to right: */
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n < -1) return 0;
    if (n == -1) return 0;

    /* i = n is the index in the flattened triangle (0‑based after the -1 term) */
    idx_t i = n;

    /* binary search for the row r such that cum(r-1) <= i < cum(r)   */
    /* cum(m) = (m+1)(m+4)/2  (total entries up to row m)           */
    idx_t lo = 0, hi = i;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t cum_mid = (mid + 1) * (mid + 4) / 2;
        if (cum_mid > i)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                                   /* row index (>=0)        */
    idx_t prev = (r * (r + 3)) / 2;                 /* cum(r-1)                */
    idx_t j = i - prev;                             /* column index 0..r+1    */

    /* compute T(r,j) */
    seq_t bin = binomial(r + 1, j);
    seq_t term1 = pow_uint(2, r + 2 * j);           /* 2^(n+2k)  */
    seq_t term2 = pow_uint(2, 2 * r + 1 + j);       /* 2^(2n+1+k) */
    seq_t term3 = pow_uint(2, j);                   /* 2^k       */
    seq_t inside = term1 + term2;
    inside -= term3;

    return bin * inside;
}
