/*
 * A338036  Triangle T(n,m) = Sum_{k=1..m}
 *          C(2*m-k-1,m-k)*C(2*(2*m-k),n-2*m+k), n>0, m>0.
 * https://oeis.org/A338036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle formula: T(r,c) = Sum_{k=1..c} C(2c-k-1, c-k) * C(2*(2c-k), r-2c+k) */
/* Sequence a(n) enumerates the triangle in row‑major order:
   a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), … */

#include <stdint.h>

/* multiplicative binomial coefficient C(n,k) computed with seq_t arithmetic.
 * It assumes 0 ≤ k ≤ n and returns 0 when k > n.  For seq_t values that
 * do not fit in the underlying type the behaviour is as with the usual
 * operator *, / on that type. */
static inline seq_t comb(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Computes a(n) according to the OEIS enumeration described above. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 0;
    while ((r * (r + 1) / 2) < n)
        ++r;

    /* Column index within the row. */
    idx_t prev = (r - 1) * r / 2;
    idx_t c = n - prev;          /* 1 ≤ c ≤ r */

    /* Compute T(r,c) = Σ_{k=1..c} C(2c-k-1, c-k) * C(2*(2c-k), r-2c+k). */
    seq_t sum = 0;
    for (idx_t k = 1; k <= c; ++k)
    {
        idx_t a1 = 2 * c - k - 1;
        idx_t b1 = c - k;
        seq_t term1 = comb(a1, b1);

        idx_t a2 = 2 * (2 * c - k);
        idx_t b2 = r - 2 * c + k;
        seq_t term2 = comb(a2, b2);

        sum += term1 * term2;
    }
    return sum;
}
