/*
 * A358050  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} binomial(k*j,j) *
 *          binomial(k*(n-j),n-j).
 * https://oeis.org/A358050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry T(i,j) on the antidiagonal * T(i,j) =
 *            Σ_{t=0..i} (j‑1)^(i‑t) · binomial(j*i+1, t) * with the square
 *            array read by diagonals downwards. * (For j == 0, T(i,0) = 1 if
 *            i == 0, otherwise 0.)
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t e = 0; e < exp; ++e)
        res *= base;
    return res;
}

static seq_t compute_T(idx_t i, idx_t j)
{
    if (j == 0)
        return i == 0 ? 1 : 0;

    seq_t sum = 0;
    idx_t m = j * i + 1;              /* fits in idx_t for i,j <= 1e5         */
    for (idx_t t = 0; t <= i; ++t)
    {
        seq_t power = pow_int((seq_t)(j - 1), i - t);
        seq_t bin   = binomial(m, t);
        sum += power * bin;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Find diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while ((__int128)(s + 1) * (s + 2) <= (__int128)2 * n)
        ++s;

    idx_t total_before = s * (s + 1) / 2;   /* number of elements before this diagonal */
    idx_t pos          = n - total_before;  /* position inside the diagonal          */
    idx_t i            = pos;               /* row index (n)                         */
    idx_t j            = s - pos;           /* column index (k)                      */

    return compute_T(i, j);
}
