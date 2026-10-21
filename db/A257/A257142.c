/*
 * A257142  Triangle read by rows: T(n,m) = Sum_{k=0..n}
 *          C(k-1,m-1)*C(k,m-1)*C(n+k-1,n-k)/m.
 * https://oeis.org/A257142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th entry of the triangle T(n,m) read by rows.
 *            T(r,c) = Sum_{k=max(1,c-1)..r} C(k-1,c-1) * C(k,c-1) *
 *            C(r+k-1,r-k) / c with r and c determined by n from the
 *            triangular indexing: r = smallest integer such that r(r+1)/2 >=
 *            n c = n - r(r-1)/2
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety, although n>=1 per spec */
    /* find row r */
    idx_t v = 1 + 8 * n;                     /* 8*n fits in 64-bit for n up to ~10^10 */
    idx_t root = _floor_isqrt_u64(v);        /* floor(sqrt(v))   */
    idx_t r = (root - 1) / 2;                /* approx floor((sqrt(1+8n)-1)/2) */
    if (r * (r + 1) / 2 < n) ++r;            /* ensure r*(r+1)/2 >= n */

    /* column c */
    idx_t start = r * (r - 1) / 2;
    idx_t c = n - start;                     /* 1‑based column index */

    /* sum the formula */
    seq_t sum = 0;
    idx_t k_start = (c == 1) ? 1 : c - 1;     /* avoid negative index for k */
    for (idx_t k = k_start; k <= r; ++k)
    {
        /* compute C(k-1,c-1)*C(k,c-1)*C(r+k-1,r-k) / c */
        seq_t term = binomial(k - 1, c - 1);
        term *= binomial(k, c - 1);
        term *= binomial(r + k - 1, r - k);
        term /= (seq_t)c;                     /* safe: division is exact */
        sum += term;
    }
    return sum;
}
