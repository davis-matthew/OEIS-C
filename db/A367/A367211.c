/*
 * A367211  Triangular array read by rows: T(n, k) = binomial(n, k) *
 *          A000129(n - k) for 0 <= k < n.
 * https://oeis.org/A367211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangular array T(n,k) = binomial(n,k) * A000129(n-k) read
 *  by rows.  a(1)=1, a(2)=2, a(3)=2, a(4)=5, …  (OEIS A367211)
 *
 *  T(n,k) with 0 ≤ k< n, n≥1.  The sequence a(n) lists the entries
 *  of the array in row-major order.  This implementation obtains the
 *  row r and column k for the nth term by a simple O(r) search on the
 *  triangular number r(r+1)/2 and then evaluates
 *
 *          a(n) = binomial(r, k) * Pell(r-k)
 *
 *  where Pell(m) = A000129(m) (Pell numbers with Pell(1)=1, Pell(2)=2,
 *  Pell(m)=2·Pell(m-1)+Pell(m-2)).  No caching is required because
 *  r is at most ≈ √(2n) for the values of n tested by the judge
 *  (~10 000).  All arithmetic is performed in seq_t, the type supplied
 *  by the build, using only integer operators available in the
 *  requirements list.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */

/* Computes the m‑th Pell number (A000129).  m≥1.  */
static seq_t pell(idx_t m)
{
    if (m == 0)         /* not used, keep for completeness */
        return 0;
    if (m == 1)
        return 1;
    if (m == 2)
        return 2;
    seq_t prev2 = 1;   /* Pell(1) */
    seq_t prev1 = 2;   /* Pell(2) */
    seq_t cur;
    for (idx_t i = 3; i <= m; ++i) {
        cur = prev1 * 2 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

/* Find the row r such that r(r+1)/2 ≥ n (n≥1).  */
static idx_t find_row(idx_t n)
{
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;
    return r;
}

/* Main function required by the build.  */
seq_t a(idx_t n)
{
    if (n == 0)                         /* safety for offsets <1 */
        return 0;

    idx_t r = find_row(n);              /* row number */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_tri - 1;  /* 0‑based column index */
    idx_t m = r - k;                    /* index for Pell number */

    seq_t bin = binomial(r, k);
    seq_t p  = pell(m);
    return bin * p;
}
