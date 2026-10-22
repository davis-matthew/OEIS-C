/*
 * A395611  Triangle read by rows: coefficients of (1+x)*(1+(n+1)*x)^(n-1).
 * https://oeis.org/A395611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = coefficient of x^k in (1+x)*(1+(n+1)x)^(n-1) * read by
 *            rows of the triangle T(r,k) = (r+1-k)*C(r,k)*(r+1)^(k-1), *
 *            where row r starts at index r(r+1)/2 in the flattened sequence
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------ */
/*  Compute binomial coefficient C(n,k) as seq_t                  */
/* ------------------------------------------------------------ */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply by (n - k + i) and divide by i; exact division */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/* ------------------------------------------------------------ */
/*  Compute r such that r(r+1)/2 <= n < (r+1)(r+2)/2              */
/* ------------------------------------------------------------ */
static idx_t _row_of(idx_t n)
{
    /* linear search is fine for n up to ~10⁵; for larger n a binary
     * search would be preferable, but the problem guarantees n <= 10⁴.
     */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    return r;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find row r and position k within that row */
    idx_t r = _row_of(n);
    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;

    if (k == 0)          /* first element of every row is 1 */
        return (seq_t)1;

    /* Compute T(r,k) = (r+1-k) * C(r,k) * (r+1)^(k-1) */
    seq_t mult   = (seq_t)(r + 1 - k);
    seq_t binom  = _binom(r, k);
    seq_t powe   = 1;
    for (idx_t i = 1; i < k; ++i)
        powe *= (seq_t)(r + 1);

    return mult * binom * powe;
}
